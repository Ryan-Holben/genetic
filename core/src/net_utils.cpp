#include <core/neural_network.h>
#include <core/platform.h>

#include <algorithm>

namespace core {

// Build a network with the provided layer sizes, where all weights and biases are set to 1.0.
// Useful for testing purposes or cheaply initializing a blank network that can still propagate
// values.
NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes) {
    NeuralNetwork net;
    for (size_t i = 0; i + 1 < layerSizes.size(); i++) {
        net._layers.push_back(
            Layer(layerSizes[i], {.weights = Tuple(layerSizes[i + 1], 1.0), .bias = 1.0}));
    }
    net._layers.push_back(Layer(layerSizes.back(), {.weights = Tuple(), .bias = 1.0}));

    return net;
}

// Build a network with the provided layer sizes, where all weights and biases are randomized to be
// within specified ranges.
NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes) {
    NeuralNetwork net;
    net._layers.reserve(layerSizes.size());

    for (size_t i = 0; i + 1 < layerSizes.size(); i++) {
        Layer layer;
        layer.reserve(layerSizes[i]);
        for (size_t nodeIdx = 0; nodeIdx < layerSizes[i]; nodeIdx++) {
            Neuron node;
            node.bias = RandomBias.get();
            node.weights.reserve(layerSizes[i + 1]);
            std::generate_n(std::back_inserter(node.weights), layerSizes[i + 1],
                            []() { return RandomWeight.get(); });
            layer.push_back(std::move(node));
        }
        net._layers.push_back(std::move(layer));
    }
    net._layers.push_back(Layer(layerSizes.back(), {.weights = Tuple(), .bias = 1.0}));

    return net;
}

} // namespace core