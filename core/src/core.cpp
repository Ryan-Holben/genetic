#include <core/core.h>

namespace core {

Number inline ActivationFunctionReLu(Number input) { return input < 0.0 ? 0.0 : input; }

// Apply the network to an input tuple, return the output tuple
Tuple NeuralNetwork::compute(const Tuple& input) const {

    const size_t width = getWidth();

    Tuple prev, cur;
    prev.reserve(width);
    cur.reserve(width);

    // Load up the input
    for (const auto& x : input) {
        prev.push_back(x);
    }

    // Loop through each layer
    for (size_t layerIdx = 1; layerIdx < _layers.size(); layerIdx++) {
        const auto& curLayer = _layers[layerIdx];
        const auto& prevLayer = _layers[layerIdx - 1];

        // Loop through all neurons in this layer
        for (size_t i = 0; i < curLayer.size(); i++) {
            // Compute current neuron's bias + all prev neuron's computed values multiplied by the
            // weight of the synapse connecting them to this neuron.
            Number value = curLayer[i].bias;
            for (size_t j = 0; j < prevLayer.size(); j++) {
                const auto& prevNeuron = prevLayer[j];
                value += prevNeuron.weights[i] * prev[j];
            }
            cur.push_back(ActivationFunctionReLu(value));
        }

        std::swap(cur, prev);
        cur.clear();
    }

    return prev;
}

// TODO: Maybe cache this value in the future
// Returns the maximum length of all layers
size_t NeuralNetwork::getWidth() const {
    return std::max_element(_layers.begin(), _layers.end(),
                            [](const Layer& a, const Layer& b) { return a.size() < b.size(); })
        ->size();
}

// Returns the number of layers
size_t NeuralNetwork::getDepth() const { return _layers.size(); }

// Returns the number of neurons in the network
size_t NeuralNetwork::getNumNeurons() const {
    size_t num = 0;
    for (const auto& layer : _layers) {
        num += layer.size();
    }
    return num;
}

// Returns the number of connections/synapses between the neurons
size_t NeuralNetwork::getNumConnections() const {
    size_t num = 0;
    for (size_t layerIdx = 0; layerIdx + 1 < _layers.size(); layerIdx++) {
        num += _layers[layerIdx].size() * _layers[layerIdx + 1].size();
    }
    return num;
}

} // namespace core