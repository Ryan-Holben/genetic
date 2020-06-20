#pragma once

#include <core/core.h>

using namespace core;

namespace evo {

struct Agent {
    NeuralNetwork brain;
    Number score = std::numeric_limits<Number>::infinity();

    Agent() {}
    Agent(NeuralNetwork&& brain) : brain(brain) {}
};

} // namespace evo