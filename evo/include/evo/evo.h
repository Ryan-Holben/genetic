#pragma once

#include <core/core.h>
#include <evo/generation.h>

#include <vector>

using namespace core;

namespace evo {

class Evolver {
public:
	void runAlgorithm(size_t initialPopulation, size_t maxGenerations, Number targetScore, const std::vector<size_t>& defaultTopology);

	void installTrainingData(Dataset&& dataset);
private:


	Dataset _trainingData;
	std::vector<Generation> _generations;
};

} // namespace evo
