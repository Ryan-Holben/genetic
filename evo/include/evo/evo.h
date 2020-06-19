#pragma once

#include <core/math.h>
#include <core/neural_network.h>
#include <core/platform.h>
#include <evo/generation.h>

#include <vector>

using namespace core;

namespace evo {

class Evolver {
  public:
    void runAlgorithm(size_t initialPopulation, size_t maxGenerations, Number targetScore,
                      const std::vector<size_t>& defaultTopology);

    void installTrainingData(Dataset&& dataset);

  private:
    // Auxilary functions called exclusively by runAlgorithm()
    friend void RunAndScoreGeneration(Dataset& dataset, Generation* gen);
    friend void SortGenerationByScores(Generation* gen);

    Dataset _trainingData;
    std::vector<Generation> _generations;
};

// Gotta re-declare friend functions in C++
void RunAndScoreGeneration(Dataset& dataset, Generation* gen);
void SortGenerationByScores(Generation* gen);

} // namespace evo
