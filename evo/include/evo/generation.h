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

    // Demographics
    size_t minAge = 0;
    size_t maxAge = 0;
    Number meanAge = 0.0;

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

    // Statistics on mutations across the surviving population-- hopefully should signal which
    // mutations were most impactful.
    size_t mutationsEffectiveNumNewLayers = 0;
    size_t mutationsEffectiveNumNewNeurons = 0;
    size_t mutationsEffectiveNumRemovedNeurons = 0;
    size_t mutationsEffectiveNumBiasChanges = 0;
    size_t mutationsEffectiveNumWeightChanges = 0;

    // Model complexity statistics
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
