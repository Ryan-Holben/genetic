#include <core/math.h>
#include <core/neural_network.h>
#include <core/platform.h>

namespace core {

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
    ASSERT(!_layers.empty());
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

void NeuralNetwork::AddRandomNeuron() {
    // We can't modify the first or last layer, as they define the input & output
    IndexSelector.setDist(1, getDepth() - 2);
    size_t layerIdx = IndexSelector.get();

    // We place the neuron at the back of this layer, because it's fully connected to its adjacent
    // layers anyway, so order doesn't matter.
    Neuron node;
    node.bias = RandomBias.get();
    node.weights.reserve(_layers[layerIdx + 1].size());
    std::generate_n(std::back_inserter(node.weights), _layers[layerIdx + 1].size(),
                    []() { return RandomWeight.get(); });
    _layers[layerIdx].push_back(node);

    // Then we add connections to this neuron from each neuron in the previous layer
    for (size_t i = 0; i < _layers[layerIdx - 1].size(); i++) {
        _layers[layerIdx - 1][i].weights.push_back(RandomWeight.get());
    }
}

void NeuralNetwork::AddRandomLayer() {
    // Places the new layer at position layerIdx, pushing the data there back.
    // Ex: If we insert at index 1, [a, b, c, d] becomes [a, NEW, b, c, d]
    IndexSelector.setDist(1, getDepth() - 1);
    size_t layerIdx = IndexSelector.get();

    Layer newLayer = _layers[layerIdx - 1];
    const size_t layerSize = newLayer.size();
    for (size_t i = 0; i < _layers[layerIdx - 1].size(); i++) {
        _layers[layerIdx - 1][i].bias = 0.0;
        _layers[layerIdx - 1][i].weights = Tuple(layerSize, 0.0);
        _layers[layerIdx - 1][i].weights[i] = 1.0;
    }
    _layers.insert(_layers.begin() + layerIdx - 1, newLayer);
}

void NeuralNetwork::MutateBiasesAndWeights(size_t* numBiasMutations, size_t* numWeightMutations) {
    OUTPUT_PARAM(numBiasMutations);
    OUTPUT_PARAM(numWeightMutations);

    // Actually let's not clear this, and let the parent function accumulate over multiple calls
    // *numBiasMutations = 0;
    // *numWeightMutations = 0;

    for (auto& layer : _layers) {
        for (auto& neuron : layer) {
            if (ChanceOfBiasMutation.roll()) {
                neuron.bias += AmountOfBiasMutation.get();
                (*numBiasMutations)++;
            }
            for (auto& weight : neuron.weights) {
                if (ChanceOfWeightMutation.roll()) {
                    weight += AmountOfWeightMutation.get();
                    (*numWeightMutations)++;
                }
            }
        }
    }

    // std::cout << "Mutated " << numBiasMutations << " biases and " << numWeightMutations << "
    // weights.\n";
}

} // namespace core