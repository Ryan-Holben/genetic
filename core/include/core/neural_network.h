#pragma once

#include "types.h"

#include "random.h"

namespace core {

static RandIntDistribution IndexSelector(0, 10); // Used for choosing indices in vectors

static RandRealDistribution RandomBias(-1.0, 1.0);
static RandRealDistribution RandomWeight(-1.0, 1.0);

static Dice ChanceOfBiasMutation(0.2);
static Dice ChanceOfWeightMutation(0.1);
static RandRealNormalDistribution AmountOfBiasMutation(-0.2, 0.2);
static RandRealNormalDistribution AmountOfWeightMutation(-0.2, 0.2);

struct Neuron {
    Tuple weights;
    Number bias;
};

using Layer = std::vector<Neuron>;

class NeuralNetwork {
  public:
    // Primary methods
    Tuple compute(const Tuple& input) const;

    // Topology
    size_t getWidth() const;
    size_t getDepth() const;
    size_t getNumNeurons() const;
    size_t getNumConnections() const;

    // Mutations
    void AddRandomNeuron();
    void AddRandomLayer();
    void MutateBiasesAndWeights(size_t* numBiasMutations, size_t* numWeightMutations);

    // Utils.  See net_utils.cpp
    friend NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);
    friend NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes);

  private:
    std::vector<Layer> _layers;
};

NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);
NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes);

} // namespace core