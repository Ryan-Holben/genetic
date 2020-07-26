#pragma once

#include <core/core.h>
#include <net/neural_network.h>

using namespace core;

namespace evo {

struct Agent {
    net::NeuralNetwork brain;
    Number score = std::numeric_limits<Number>::infinity();
    size_t age = 0;

    // Number of mutations of each type since the last generation
    size_t numRemoveNeuron = 0;
    size_t numAddNeuron = 0;
    size_t numAddLayer = 0;
    size_t numMutateBias = 0;
    size_t numMutateWeight = 0;

    Agent() {}
    Agent(net::NeuralNetwork&& brain) : brain(brain) {}
};

} // namespace evo