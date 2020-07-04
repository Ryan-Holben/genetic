#pragma once

#include <core/math.h>
#include <core/platform.h>
#include <evo/generation.h>
#include <net/neural_network.h>

#include <vector>

using namespace core;

namespace evo {

class Evolver {
  public:
    void runAlgorithm(size_t initialPopulation, size_t maxGenerations, Number targetScore,
                      const std::vector<size_t>& defaultTopology);

    void installTrainingData(Dataset&& dataset);
    void saveOutput(const std::string& folder) const;

  private:
    void RecordGenerationToHistory(Generation& gen);

    Dataset _trainingData;
    std::vector<Generation> _generations;
};

// Auxilary functions called exclusively by runAlgorithm()
void RunAndScoreGeneration(Dataset& dataset, Generation* gen);
void SortGenerationByScores(Generation* gen);
void KeepOnlyFirstAgents(const size_t numToKeep, Generation* gen);
void ComputeGenerationStats(Generation* gen);
Generation SpawnNextGeneration(const Generation& gen);

size_t GetNumChildren(Number score, Number minScore, Number maxScore);

} // namespace evo
