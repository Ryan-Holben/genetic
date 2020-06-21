#pragma once

#include "types.h"

#include "random.h"

namespace core {

static RandRealDistribution RandomBias(-1.0, 1.0);
static RandRealDistribution RandomWeight(-1.0, 1.0);

struct Neuron {
    Tuple weights;
    Number bias;
};

using Layer = std::vector<Neuron>;

class NeuralNetwork {
  public:
    // Primary methods
    Tuple compute(const Tuple& input) const;

    // Utils.  See net_utils.cpp
    friend NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);
    friend NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes);

    // Topology
    size_t getWidth() const;
    size_t getDepth() const;
    size_t getNumNeurons() const;
    size_t getNumConnections() const;

  private:
    std::vector<Layer> _layers;
};

NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);
NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes);

} // namespace core