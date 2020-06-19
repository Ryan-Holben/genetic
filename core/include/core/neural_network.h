#pragma once

#include "types.h"

namespace core {

struct Neuron {
    Tuple weights;
    Number bias;
};

using Layer = std::vector<Neuron>;

class NeuralNetwork {
  public:
    // Primary methods
    Tuple compute(const Tuple& input) const;

    // Utils.  See net_utils.h/.cpp
    friend NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);

    // Topology
    size_t getWidth() const;
    size_t getDepth() const;
    size_t getNumNeurons() const;
    size_t getNumConnections() const;

  private:
    std::vector<Layer> _layers;
};

NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);

} // namespace core