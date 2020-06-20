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
            agent.score += L2_SQUARED(datum.second, result);
        }
    }
}

// Sort the generation's agents from least to greatest score (best to worst).
void SortGenerationByScores(Generation* gen) {
    OUTPUT_PARAM(gen);
    const size_t startSize = gen->agents.size();

    std::sort(gen->agents.begin(), gen->agents.end(),
              [](Agent& a, Agent& b) { return a.score > b.score; });

    ASSERT_WITH_MSG(startSize == gen->agents.size(), "Sorting changed the number of agents from "
                                                         << startSize << " to "
                                                         << gen->agents.size());
}

// Culls any agents beyond the first numToKeep agents.
void KeepOnlyFirstAgents(const size_t numToKeep, Generation* gen) {
    OUTPUT_PARAM(gen);
    const size_t startCapacity = gen->agents.capacity();
    const size_t startSize = gen->agents.size();

    if (gen->agents.size() > numToKeep) {
        gen->agents.resize(numToKeep);
    }

    ASSERT_WITH_MSG(startCapacity == gen->agents.capacity(),
                    "Capacity changed from " << startCapacity << " to " << gen->agents.capacity())
    if (startSize <= numToKeep) {
        ASSERT_WITH_MSG(
            startSize == gen->agents.size(),
            "We resized when we were already under numToKeep and should have done nothing; from "
                << startSize << " to " << gen->agents.size());
    } else {
        ASSERT_WITH_MSG(startSize > gen->agents.size(), "We needed to resize but didn't")
    }
}

// Fill out various stats about this generation.  Assumes the agents are ordered from best to worst
// scores.
void ComputeGenerationStats(Generation* gen) {
    OUTPUT_PARAM(gen);

    ASSERT_WITH_MSG(!gen->agents.empty(),
                    "The current generation has no agents!  Yes we could gracefully handle this, "
                    "but this probably has broken the algorithm anyway.");

    const auto bestAgent = gen->agents.front();
    const auto worstAgent = gen->agents.back();

    gen->bestScore = bestAgent.score;
    gen->worstScore = worstAgent.score;

    gen->averageScore = 0.0;
    for (const auto& agent : gen->agents) {
        gen->averageScore += agent.score;
    }
    gen->averageScore /= gen->agents.size();
}

} // namespace evo