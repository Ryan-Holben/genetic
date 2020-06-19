#pragma once

#include "types.h"

namespace core {

// Simple activation function for use at each neuron in a forward computation of an input
Number inline ActivationFunctionReLu(Number input) { return input < 0.0 ? 0.0 : input; }

Number L2_SQUARED(const Tuple& x, const Tuple& y);

} // namespace core