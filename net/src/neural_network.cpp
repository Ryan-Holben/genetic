#include <core/math.h>
#include <core/platform.h>
#include <net/neural_network.h>

#include <algorithm>
#include <set>

namespace net {

bool inline isbad(Number val) { return std::isnan(val) || std::isinf(val); }

// Apply the network to an input tuple, return the output tuple
Tuple NeuralNetwork::compute(const Tuple& input) const {
    ASSERT_WITH_MSG(input.size() == _layers.front().size(),
                    "Input was size " << input.size() << " but the input layer has "
                                      << _layers.front().size() << " neurons.");
    for (const auto& x : input) {
        ASSERT_WITH_MSG(!isbad(x), "Input contained a NaN!");
    }

    Tuple prev, cur;
    const size_t width = getWidth();
    prev.reserve(width);
    cur.reserve(width);

    // Load up the input
    for (const auto& x : input) {
        prev.push_back(x);
    }

    // Loop through each layer
    auto iter = _layers.begin();
    for (iter++; iter != _layers.end(); iter++) {
        const auto& curLayer = *iter;
        const auto& prevLayer = *std::prev(iter);

        // Loop through all neurons in this layer
        for (size_t i = 0; i < curLayer.size(); i++) {
            // Compute current neuron's bias + all prev neuron's computed values multiplied by the
            // weight of the synapse connecting them to this neuron.
            Number value = curLayer[i].bias;
            for (size_t j = 0; j < prevLayer.size(); j++) {
                value += prevLayer[j].weights[i] * prev[j];
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
    auto iter = _layers.begin();
    for (size_t i = 0; i < _layers.size() - 1; i++) {
        num += iter->size() * std::next(iter)->size();
        iter++;
    }
    return num;
}

std::vector<size_t> NeuralNetwork::getTopology() const {
    std::vector<size_t> topology;
    for (const auto& layer : _layers) {
        topology.push_back(layer.size());
    }
    return topology;
}

void NeuralNetwork::AddRandomNeuron() {
    // We can't modify the first or last layer, as they define the input & output
    if (const size_t depth = getDepth(); depth < 3) {
        return;
    } else {
        IndexSelector.setDist(1, depth - 2);
    }
    const size_t layerIdx = IndexSelector.get();
    auto prevLayer = std::next(_layers.begin(), layerIdx - 1);
    auto curLayer = std::next(prevLayer);
    const auto nextLayer = std::next(curLayer);

    // We place the neuron at the back of this layer, because it's fully connected to its adjacent
    // layers anyway, so order doesn't matter.
    Neuron node;
    node.bias = RandomBias.get();
    node.weights.reserve(nextLayer->size());
    std::generate_n(std::back_inserter(node.weights), nextLayer->size(),
                    []() { return RandomWeight.get(); });
    curLayer->push_back(node);

    // Then we add connections to this neuron from each neuron in the previous layer
    for (size_t i = 0; i < prevLayer->size(); i++) {
        (*prevLayer)[i].weights.push_back(RandomWeight.get());
    }
}

void NeuralNetwork::RemoveRandomNeuron() {
    // We can't modify the first or last layer, as they define the input & output
    if (const size_t depth = getDepth(); depth < 3) {
        return;
    } else {
        IndexSelector.setDist(1, depth - 2);
    }
    const size_t layerIdx = IndexSelector.get();
    auto prevLayer = std::next(_layers.begin(), layerIdx - 1);
    auto curLayer = std::next(prevLayer);

    // If this layer has only 1 neuron, remove the layer.  The previous layer will be given random
    // weights connecting it to the next layer.
    if (curLayer->size() == 1) {
        auto nextLayer = std::next(curLayer);
        for (auto& node : *prevLayer) {
            node.weights.reserve(nextLayer->size());
            std::generate_n(std::back_inserter(node.weights), nextLayer->size(),
                            []() { return RandomWeight.get(); });
        }
        _layers.erase(curLayer);
        return;
    }
    // Choose a random neuron in this layer to remove
    IndexSelector.setDist(0, curLayer->size() - 1);
    const size_t neuronIdx = IndexSelector.get();

    // Remove connections to this neuron from the previous layer
    for (auto& neuron : *prevLayer) {
        neuron.weights.erase(neuron.weights.begin() + neuronIdx);
    }

    // Remove the neuron itself
    curLayer->erase(curLayer->begin() + neuronIdx);
}

void NeuralNetwork::AddRandomLayer() {
    // Choose an index that cannot be 0.  Push the layer there back to the next index.  E.g. if
    // layerIdx == 1, then [a, b, c, d] becomes [a, NEW, b, c, d].
    IndexSelector.setDist(1, getDepth() - 1);
    const size_t layerIdx = IndexSelector.get();

    // Here's the adjacent layers that we're inserting inbetween
    // auto& prevLayer = _layers[layerIdx - 1];     // Not actually used
    // auto& nextLayer = _layers[layerIdx];
    const auto nextLayer = std::next(_layers.begin(), layerIdx);

    // Build the new layer.  It will have the size and biases of nextLayer.  However, its weights
    // will be a kronecker delta.  That is, at first this new layer is almost trivial, so that
    // introducing it doesn't create a huge jump in behavior.
    Layer newLayer;
    const size_t newLayerSize = nextLayer->size();
    newLayer.reserve(newLayerSize);
    for (size_t i = 0; i < newLayerSize; i++) {
        // Connect newLayer to the nextLayer with the kronecker delta function
        newLayer.push_back({.bias = 1.0, .weights = Tuple(newLayerSize, 0.0)});
        newLayer.back().weights[i] = 1.0;
    }

    // Insert the new layer
    _layers.insert(nextLayer, newLayer);
}

// Mutate biases & weights throughout the neural network randomly.  Rather than rolling RNG for
// every single bias and weight, we frontload the RNG to figure out how many objects should be
// mutated.  We then generate the lists of objects to mutate and then loop through the networks,
// skipping ahead wherever we can.
void NeuralNetwork::MutateBiasesAndWeights(size_t* numBiasMutations, size_t* numWeightMutations) {
    OUTPUT_PARAM(numBiasMutations);
    OUTPUT_PARAM(numWeightMutations);

    // This version attempts to be cheaper by using RNG to decide when the next time will be that we
    // mutate.
    const size_t numNeurons = getNumNeurons();
    const size_t numConnections = getNumConnections();

    // The number of mutations we're targeting
    const size_t targetNumBiasMutations = numNeurons * BiasMutationRate.get();
    const size_t targetNumWeightMutations = numConnections * WeightMutationRate.get();

    // Choose indices for each to mutate.  Note that they may overlapping, giving us an actual
    // number of mutations below the target number.
    BiasIndexSelector.setDist(0, numNeurons - 1);
    WeightIndexSelector.setDist(0, numConnections - 1);
    std::set<size_t> biasIndices;
    std::set<size_t> weightIndices;
    for (size_t i = 0; i < targetNumBiasMutations; i++) {
        biasIndices.insert(BiasIndexSelector.get());
    }
    for (size_t i = 0; i < targetNumWeightMutations; i++) {
        weightIndices.insert(WeightIndexSelector.get());
    }

    // Loop through the neural network's structure and do mutations.  Wherever possible, we skip
    // ahead rather than looping through one object at a time.
    size_t neuronIdx = 0;
    size_t weightIdx = 0;
    auto nextBiasIter = biasIndices.begin();
    auto nextWeightIter = weightIndices.begin();
    for (auto& layer : _layers) {
        for (auto& neuron : layer) {
            // While there are biases left to mutate, do so.
            if (nextBiasIter != biasIndices.end()) {
                // If we've reached the next neuron to mutate, pop off the index record and do the
                // mutation.
                if (neuronIdx == *nextBiasIter) {
                    biasIndices.erase(nextBiasIter);
                    neuron.bias += AmountOfBiasMutation.get();
                    (*numBiasMutations)++;
                }
            }

            // While there are weights left to mutate, do so
            if (nextWeightIter != weightIndices.end()) {
                // Loop through all weight mutations that happen within this neuron
                size_t relativeIdx = *nextWeightIter - weightIdx;
                while (relativeIdx < neuron.weights.size()) {
                    weightIndices.erase(nextWeightIter);
                    neuron.weights[relativeIdx] += AmountOfWeightMutation.get();
                    (*numWeightMutations)++;

                    nextWeightIter = weightIndices.cbegin();
                    relativeIdx = *nextWeightIter - weightIdx;
                }
                // Then advance our weightIdx forward by the total number of weights that belong to
                // this neuron.
                weightIdx += neuron.weights.size();
            }

            // Exit early if we can.
            if (biasIndices.empty() && weightIndices.empty()) {
                return;
            }

            neuronIdx++;
        }
    }
}

} // namespace net