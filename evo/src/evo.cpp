#include <evo/constants.h>
#include <evo/evo.h>

#include <iostream>

using namespace core;

// Return the first index that's equal to 1.0
int vecToInt(const std::vector<Number>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == 1.0) {
            return i;
        }
    }
    return -1;
}

// Pretty print a vector
template <typename T> std::string vecToStr(const std::vector<T>& vec) {
    std::string s = "[";
    for (size_t i = 0; i < vec.size() - 1; i++) {
        s += std::to_string(vec[i]) + ", ";
    }
    s += std::to_string(vec.back()) + "]";
    return s;
}

namespace evo {

void Evolver::runAlgorithm(size_t initialPopulation, size_t maxGenerations, Number targetScore,
                           const std::vector<size_t>& defaultTopology) {
    std::cout << "\n\x1b[1;31mBeginning evolution\x1b[0m with settings:\n";
    std::cout << "\t- Initial population of " << initialPopulation << " agents\n";
    std::cout << "\t- Maximum of " << maxGenerations << " generations\n";
    std::cout << "\t- Target score of " << targetScore << " \n";
    std::cout << "\n";

    // Instantiate the first generation.
    Generation currentGen;
    currentGen.GenerateRandomNetworks(initialPopulation, defaultTopology);
    std::cout << "Seeded population pool with " << currentGen.agents.size() << " agents.\n";

    // Run through generations, reproducing, evolving, and culling agents until one meets the target
    // fitness score.
    size_t generationIdx = 0;
    while (true) {
        std::cout << "\n-------- \x1b[1;32mIteration " << generationIdx << "\x1b[0m --------\n";
        generationIdx++;

        ASSERT_WITH_MSG(!currentGen.agents.empty(), "Generation is empty!  Life has ended.");

        // Run each agent against a subset of the training set and score it.
        RunAndScoreGeneration(_trainingData, &currentGen);
        // for (const auto & agent : currentGen.agents) {
        //     std::cout << agent.score << "\n";
        // }

        // Order the current gen from least to greatest (best to worst) score.
        SortGenerationByScores(&currentGen);

        // Cull the worst-scoring agents if they exceed our population max size
        KeepOnlyFirstAgents(constants::population::MAX_NUM_AGENTS, &currentGen);
        std::cout << "Culled " << currentGen.numCulled << " agents, population is "
                  << currentGen.agents.size() << ".\n";

        // Record stats about the best and worst scorers.
        ComputeGenerationStats(&currentGen);
        std::cout << "\x1b[1;33mScore range: " << currentGen.bestScore << ", "
                  << currentGen.worstScore << "\x1b[0m\n";

        const auto& best = currentGen.agents[0];
        for (size_t i = 0; i < 5; i++) {
            const auto output = best.brain.compute(_trainingData[i].first);
            std::cout << "expected: " << _trainingData[i].second[0] << ", got: " << output[0]
                      << "\n";
            // std::cout << "expected: " << vecToInt(_trainingData[i].second) << ", got: " <<
            // vecToStr(output) << "\n";
        }
        const auto topo = best.brain.getTopology();
        std::cout << "Best topology: " << vecToStr(topo) << "\n";

        // MOVE the current gen to the annals of history.
        // CRITICAL NOTES:
        // * Philosophically, at this point the current generation has stopped changing, and it has
        //      been saved to history.
        // * Until SpawnNExtGeneration, there is no currentGen, and accessing that variable gives
        //      undefined behavior.  Instead, we refer to lastGen in the interim.
        RecordGenerationToHistory(currentGen);
        const auto& lastGen = _generations.back();

        // If we're on the final generation, or if we've reached our target, terminate early instead
        // of producing a new gen.
        if (lastGen.bestScore <= targetScore) {
            std::cout << "\n\x1b[1;31mFinished!\x1b[0m\n";
            std::cout << "Beat target score of " << targetScore << " with a final score of "
                      << lastGen.bestScore << " after " << generationIdx << " generations.\n";
            break;
        } else if (generationIdx >= maxGenerations) {
            std::cout << "\n\x1b[1;31mFinished!\x1b[0m\n";
            std::cout << "Reached final generation " << maxGenerations
                      << " without reaching target score of " << targetScore
                      << ". Final best score was " << lastGen.bestScore << ".\n";
            break;
        }

        // Let the generation reproduce and create the next generation, introducing random
        // mutations.  This operation will be done in-place on `currentGen`, since we already saved
        // a copy of the previous generation.
        currentGen = SpawnNextGeneration(lastGen);
        std::cout << "Spawned " << currentGen.numChildren << " children, population is "
                  << currentGen.agents.size() << ".\n";
    }
}

// Makes a COPY of the provided generation
void Evolver::RecordGenerationToHistory(Generation& gen) { _generations.push_back(std::move(gen)); }

} // namespace evo