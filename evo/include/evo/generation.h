#pragma once

#include <core/core.h>
#include <evo/agent.h>

using namespace core;

namespace evo {

struct Generation {
    // The pool of neural networks
    std::vector<Agent> agents;

    // Statistics on population change
    size_t numChildren = 0;
    size_t numSurvivors = 0;
    size_t numCulled = 0;
    size_t numLifesavers = 0;

    // Fitness statistics
    Number worstScore = std::numeric_limits<Number>::infinity();
    Number averageScore = std::numeric_limits<Number>::infinity();
    Number bestScore = std::numeric_limits<Number>::infinity();

    // Statistics on mutations across the population
    size_t mutationsNumNewLayers = 0;
    size_t mutationsNumNewNeurons = 0;
    size_t mutationsNumRemovedNeurons = 0;
    size_t mutationsNumBiasChanges = 0;
    size_t mutationsNumWeightChanges = 0;

    // Model complexity
    size_t bestNumNeurons = 0;
    size_t worstNumNeurons = 0;

    // Runtime statistics
    double duration = 0.0;

    void GenerateOnesNetworks(size_t numAgents, const std::vector<size_t>& defaultTopology) {
        agents.clear();
        for (size_t i = 0; i < numAgents; i++) {
            agents.push_back(Agent(net::BuildOnesNetwork(defaultTopology)));
        }
    }

    void GenerateRandomNetworks(size_t numAgents, const std::vector<size_t>& defaultTopology) {
        agents.clear();
        for (size_t i = 0; i < numAgents; i++) {
            agents.push_back(Agent(net::BuildRandomNetwork(defaultTopology)));
        }
    }
};

} // namespace evo
