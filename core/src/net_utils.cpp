#include <core/neural_network.h>

namespace core {

// Build a network with the provided layer sizes, where all weights and biases are set to 1.0.
// Useful for testing purposes or cheaply initializing a blank network that can still propagate
// values.
NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes) {
    NeuralNetwork net;
    net._layers.clear();
    for (size_t i = 0; i + 1 < layerSizes.size(); i++) {
        net._layers.push_back(
            Layer(layerSizes[i], {.weights = Tuple(layerSizes[i + 1], 1.0), .bias = 1.0}));
    }
    net._layers.push_back(Layer(layerSizes.back(), {.weights = Tuple(), .bias = 1.0}));

    return net;
}

} // namespace core