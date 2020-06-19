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

    std::sort(gen->agents.begin(), gen->agents.end(),
              [](Agent& a, Agent& b) { return a.score > b.score; });
}

} // namespace evo