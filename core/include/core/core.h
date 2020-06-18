#pragma once

#include <vector>

namespace core {
using Number = float;
using Tuple = std::vector<Number>;
using Datum = std::pair<Tuple, Tuple>;
using Dataset = std::vector<Datum>;

struct Neuron {
    // std::vector<Number> weights;
    Tuple weights;
    Number bias;
};

using Layer = std::vector<Neuron>;

class NeuralNetwork {
  public:
    // Primary methods
    Tuple compute(const Tuple& input) const;

    // Utilities
    void buildOnesNetwork(const std::vector<size_t>& layerSizes);

    // Topology
    size_t getWidth() const;
    size_t getDepth() const;
    size_t getNumNeurons() const;
    size_t getNumConnections() const;

  private:
    std::vector<Layer> _layers;
};

Number inline ActivationFunctionReLu(Number input);

} // namespace core
