#include <evo/constants.h>
#include <evo/evo.h>

namespace evo {

void Evolver::installTrainingData(Dataset&& dataset) { _trainingData = std::move(dataset); }

// Run the input data through each agent in gen and compute and store the resulting scores.
void RunAndScoreGeneration(Dataset& dataset, Generation* gen) {
    OUTPUT_PARAM(gen);

    for (auto& agent : gen->agents) {
        agent.score = 0.0;
        for (const auto& datum : dataset) {
            const auto result = agent.brain.compute(datum.first);
            for (const auto& element : result) {
                ASSERT_WITH_MSG(!std::isnan(element), "Neural net output included a NaN!");
            }
            agent.score += L2_SQUARED(datum.second, result);
        }
    }
}

// Sort the generation's agents from least to greatest score (best to worst).
void SortGenerationByScores(Generation* gen) {
    OUTPUT_PARAM(gen);
    const size_t startSize = gen->agents.size();

    std::sort(gen->agents.begin(), gen->agents.end(),
              [](Agent& a, Agent& b) { return a.score < b.score; });

    ASSERT_WITH_MSG(startSize == gen->agents.size(), "Sorting changed the number of agents from "
                                                         << startSize << " to "
                                                         << gen->agents.size());
    ASSERT_WITH_MSG(gen->agents.front().score <= gen->agents.back().score,
                    "Failed to order agents by score.  Front is "
                        << gen->agents.front().score << ", back is " << gen->agents.back().score);
}

// Culls any agents beyond the first numToKeep agents.
void KeepOnlyFirstAgents(const size_t numToKeep, Generation* gen) {
    OUTPUT_PARAM(gen);
    const size_t startCapacity = gen->agents.capacity();
    const size_t startSize = gen->agents.size();

    if (gen->agents.size() > numToKeep) {
        gen->agents.resize(numToKeep);
    }

    const size_t endCapacity = gen->agents.capacity();
    const size_t endSize = gen->agents.size();
    gen->numCulled = startSize - endSize;

    ASSERT_WITH_MSG(startCapacity == endCapacity,
                    "Capacity changed from " << startCapacity << " to " << endCapacity)
    if (startSize <= numToKeep) {
        ASSERT_WITH_MSG(
            startSize == endSize,
            "We resized when we were already under numToKeep and should have done nothing; from "
                << startSize << " to " << endSize);
    } else {
        ASSERT_WITH_MSG(startSize > endSize, "We needed to resize but didn't")
    }
}

// Fill out various stats about this generation.  Assumes the agents are ordered from best to worst
// scores.
void ComputeGenerationStats(Generation* gen) {
    OUTPUT_PARAM(gen);

    ASSERT_WITH_MSG(!gen->agents.empty(),
                    "The current generation has no agents!  Yes we could gracefully handle this, "
                    "but this probably has broken the algorithm anyway.");

    // Record some info about the best and worst agents
    const auto bestAgent = gen->agents.front();
    const auto worstAgent = gen->agents.back();
    gen->bestScore = bestAgent.score;
    gen->worstScore = worstAgent.score;
    gen->bestNumNeurons = bestAgent.brain.getNumNeurons();
    gen->worstNumNeurons = worstAgent.brain.getNumNeurons();

    ASSERT_WITH_MSG(gen->bestScore <= gen->worstScore,
                    "Best & worst scores don't make sense.  bestScore = "
                        << gen->bestScore << ", worstScore = " << gen->worstScore);

    gen->averageScore = 0.0;
    for (const auto& agent : gen->agents) {
        // Average score
        gen->averageScore += agent.score;

        // How impactful were the various mutations on the survivors?
        gen->mutationsEffectiveNumNewLayers += agent.numAddLayer;
        gen->mutationsEffectiveNumNewNeurons += agent.numAddNeuron;
        gen->mutationsEffectiveNumRemovedNeurons += agent.numRemoveNeuron;
        gen->mutationsEffectiveNumBiasChanges += agent.numMutateBias;
        gen->mutationsEffectiveNumWeightChanges += agent.numMutateWeight;
    }
    gen->averageScore /= gen->agents.size();

    // Population demographics: min, max, and mean ages
    if (!gen->agents.empty()) {
        gen->minAge = std::numeric_limits<size_t>::max();
        gen->maxAge = 0;
        gen->meanAge = 0.0;
        for (const auto& agent : gen->agents) {
            if (agent.age < gen->minAge) {
                gen->minAge = agent.age;
            }
            if (agent.age > gen->maxAge) {
                gen->maxAge = agent.age;
            }
            gen->meanAge += agent.age;
        }
        gen->meanAge /= gen->agents.size();
    }
}

// Create a new generation from the last one.  In doing so we introduce random mutations.
Generation SpawnNextGeneration(const Generation& lastGen) {
    Generation newGen;

    if (!constants::flags::PARENTS_DIE_EACH_GENERATION) {
        newGen.agents.insert(newGen.agents.begin(), lastGen.agents.begin(), lastGen.agents.end());
        // Each of these agents increased in age without seeing any mutation
        for (auto& agent : newGen.agents) {
            agent.age++;
            agent.numRemoveNeuron = 0;
            agent.numAddNeuron = 0;
            agent.numAddLayer = 0;
            agent.numMutateBias = 0;
            agent.numMutateWeight = 0;
        }
    }

    newGen.mutationsNumNewLayers = 0;
    newGen.mutationsNumNewNeurons = 0;
    newGen.mutationsNumRemovedNeurons = 0;
    newGen.mutationsNumBiasChanges = 0;
    newGen.mutationsNumWeightChanges = 0;

    for (auto agent : lastGen.agents) {
        // 1. Get number of children for the agent
        const size_t numChildren =
            GetNumChildren(agent.score, lastGen.bestScore, lastGen.worstScore);

        // 2. Loop through that number; to each, spawn a child with mutations
        for (size_t i = 0; i < numChildren; i++) {
            // 2-a. Copy the child
            auto child = agent;
            child.age = 0;
            child.score = 0.0;

            // 2-b. Mutate the child

            // Remove a neuron?
            if (constants::mutation::chance::RemoveNeuron.roll()) {
                agent.brain.RemoveRandomNeuron();
                agent.numRemoveNeuron++;
                newGen.mutationsNumRemovedNeurons++;
            }

            // Add a new neuron?
            if (constants::mutation::chance::NewNeuron.roll()) {
                agent.brain.AddRandomNeuron();
                agent.numAddNeuron++;
                newGen.mutationsNumNewNeurons++;
            }

            // Add a new layer?
            if (constants::mutation::chance::NewLayer.roll()) {
                agent.brain.AddRandomLayer();
                agent.numAddLayer++;
                newGen.mutationsNumNewLayers++;
            }

            // Mutate weights and biases
            agent.brain.MutateBiasesAndWeights(&agent.numMutateBias, &agent.numMutateWeight);
            newGen.mutationsNumBiasChanges += agent.numMutateBias;
            newGen.mutationsNumWeightChanges += agent.numMutateWeight;

            // 2-c. Push the child to the output
            newGen.agents.push_back(child);
        }
        newGen.numChildren += numChildren;
    }
    std::cout << "Added " << newGen.mutationsNumNewNeurons << " neurons & "
              << newGen.mutationsNumNewLayers << " layers, removed "
              << newGen.mutationsNumRemovedNeurons << ", mutated " << newGen.mutationsNumBiasChanges
              << " biases & " << newGen.mutationsNumWeightChanges << " weights.\n";

    return newGen;
}

// Given an input score for an agent, and the min and max score defining its generation, return the
// number of children we should spawn.  Don't forget that minScore and maxScore correspond with the
// best and worst scores, respectively!

// TODO: For a fixed generation all of these calculations are fixed.  So precompute it once for a
// minor perf boost.
size_t GetNumChildren(Number score, Number minScore, Number maxScore) {
    ASSERT_WITH_MSG(minScore <= score && score <= maxScore,
                    "Score values don't make sense.  minScore = "
                        << minScore << ", score = " << score << ", maxScore = " << maxScore);
    if (minScore == maxScore) {
        return constants::population::MAX_NUM_CHILDREN;
    } else {
        const double ratio = (score - minScore) / (maxScore - minScore);
        // Remember to subtract the ratio from 1.0, because a low ratio of 0.0 is best, and a high
        // ratio of 1.0 is worst.
        return (1.0 - ratio) * constants::population::MAX_NUM_CHILDREN;
    }
}

} // namespace evo
