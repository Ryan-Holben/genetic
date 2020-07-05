#pragma once

#include <core/core.h>
#include <net/neural_network.h>

using namespace core;

namespace evo {

struct Agent {
    net::NeuralNetwork brain;
    Number score = std::numeric_limits<Number>::infinity();
    size_t age = 0;

    Agent() {}
    Agent(net::NeuralNetwork&& brain) : brain(brain) {}
};

} // namespace evo