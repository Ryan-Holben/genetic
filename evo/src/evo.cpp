#include <evo/constants.h>
#include <evo/evo.h>

#include <iostream>

using namespace core;

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
    currentGen.GenerateOnesNetworks(initialPopulation, defaultTopology);
    std::cout << "Built " << currentGen.agents.size() << " agents.\n";

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
        KeepOnlyFirstAgents(constants::MAX_NUM_AGENTS, &currentGen);

        // Record stats about the best and worst scorers.
        ComputeGenerationStats(&currentGen);

        // Save the current gen to the annals of history.
        RecordGenerationToHistory(currentGen);

        // If we're on the final generation, or if we've reached our target, terminate early instead
        // of producing a new gen.
        if (currentGen.bestScore <= targetScore) {
            std::cout << "\n\x1b[1;31mFinished!\x1b[0m\n";
            std::cout << "Exceeded target score of " << targetScore << " with a final score of "
                      << currentGen.bestScore << " after " << generationIdx << " generations.\n";
            break;
        } else if (generationIdx >= maxGenerations) {
            std::cout << "\n\x1b[1;31mFinished!\x1b[0m\n";
            std::cout << "Reached final generation " << maxGenerations
                      << " without reaching target score of " << targetScore
                      << ". Final best score was " << currentGen.bestScore << ".\n";
            break;
        }

        // Let this gen reproduce.  In doing so, the children will be mutations of their parents.
    }
}

void Evolver::RecordGenerationToHistory(Generation& gen) { _generations.push_back(gen); }

} // namespace evo