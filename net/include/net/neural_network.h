#pragma once

#include <core/random.h>
#include <core/types.h>

#include <list>

using namespace core;

namespace net {

static RandIntDistribution IndexSelector(0, 10); // Used for choosing indices in vectors

static RandRealDistribution RandomBias(-1.0, 1.0);
static RandRealDistribution RandomWeight(-1.0, 1.0);

// static Dice ChanceOfBiasMutation(0.20);
// static Dice ChanceOfWeightMutation(0.20);

// These density values MUST be in the open interval (0.0, 0.5)
constexpr Number biasMutationMean = 0.05;
constexpr Number biasMutationStdev = 0.341 * biasMutationMean;
constexpr Number weightMutationMean = 0.05;
constexpr Number weightMutationStdev = 0.341 * weightMutationMean;
// 1. First choose how many edits will occur
static RandGaussianDistribution BiasMutationRate(biasMutationMean, biasMutationStdev, 0.0, 1.0);
static RandGaussianDistribution WeightMutationRate(weightMutationMean, weightMutationStdev, 0.0,
                                                   1.0);
// 2. Run this a bunch of times to choose *where* to edit neurons and weights
static RandIntDistribution BiasIndexSelector(0, 10);
static RandIntDistribution WeightIndexSelector(0, 10);

static RandGaussianDistribution AmountOfBiasMutation(0.0, 0.3, -2.0, 2.0);
static RandGaussianDistribution AmountOfWeightMutation(0.0, 0.3, -2.0, 2.0);

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
    std::vector<size_t> getTopology() const;

    // Mutations
    void AddRandomNeuron();
    void AddRandomLayer();
    void MutateBiasesAndWeights(size_t* numBiasMutations, size_t* numWeightMutations);

    // Utils.  See net_utils.cpp
    friend NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);
    friend NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes);

  private:
    std::list<Layer> _layers;
};

NeuralNetwork BuildOnesNetwork(const std::vector<size_t>& layerSizes);
NeuralNetwork BuildRandomNetwork(const std::vector<size_t>& layerSizes);

} // namespace net