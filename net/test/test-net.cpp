#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <core/math.h>
#include <net/neural_network.h>

using namespace testing;
using namespace core;
using namespace net;

TEST(NeuralNetwork, topologyOnesNetwork) {
    NeuralNetwork net = BuildOnesNetwork({2, 3, 4, 1});
    EXPECT_EQ(net.getWidth(), 4);
    EXPECT_EQ(net.getDepth(), 4);
    EXPECT_EQ(net.getNumNeurons(), 10);
    EXPECT_EQ(net.getNumConnections(), 22);

    net = BuildOnesNetwork({1, 1, 1, 1, 1, 1});
    EXPECT_EQ(net.getWidth(), 1);
    EXPECT_EQ(net.getDepth(), 6);
    EXPECT_EQ(net.getNumNeurons(), 6);
    EXPECT_EQ(net.getNumConnections(), 5);

    net = BuildOnesNetwork({10});
    EXPECT_EQ(net.getWidth(), 10);
    EXPECT_EQ(net.getDepth(), 1);
    EXPECT_EQ(net.getNumNeurons(), 10);
    EXPECT_EQ(net.getNumConnections(), 0);
}

TEST(NeuralNetwork, topologyRandomNetwork) {
    NeuralNetwork net = BuildRandomNetwork({2, 3, 4, 1});
    EXPECT_EQ(net.getWidth(), 4);
    EXPECT_EQ(net.getDepth(), 4);
    EXPECT_EQ(net.getNumNeurons(), 10);
    EXPECT_EQ(net.getNumConnections(), 22);

    net = BuildRandomNetwork({1, 1, 1, 1, 1, 1});
    EXPECT_EQ(net.getWidth(), 1);
    EXPECT_EQ(net.getDepth(), 6);
    EXPECT_EQ(net.getNumNeurons(), 6);
    EXPECT_EQ(net.getNumConnections(), 5);

    net = BuildRandomNetwork({10});
    EXPECT_EQ(net.getWidth(), 10);
    EXPECT_EQ(net.getDepth(), 1);
    EXPECT_EQ(net.getNumNeurons(), 10);
    EXPECT_EQ(net.getNumConnections(), 0);
}

TEST(NeuralNetwork, computeBasicOnesNetwork) {
    NeuralNetwork net = BuildOnesNetwork({2, 3, 10, 2});

    EXPECT_THAT(net.compute({0, 0}), ElementsAre(41, 41));
    EXPECT_THAT(net.compute({1, 0}), ElementsAre(71, 71));
    EXPECT_THAT(net.compute({0, 1}), ElementsAre(71, 71));
    EXPECT_THAT(net.compute({1, 1}), ElementsAre(101, 101));
    EXPECT_THAT(net.compute({-1, -1}), ElementsAre(11, 11));
    EXPECT_THAT(net.compute({-1, 0}), ElementsAre(11, 11));
    EXPECT_THAT(net.compute({0, -1}), ElementsAre(11, 11));
}

TEST(ReLu, basic) {
    EXPECT_EQ(ActivationFunctionReLu(-1.0), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(-1.e-6), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(-0.0), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(0.0), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(1.e6), 1.e6);
    EXPECT_EQ(ActivationFunctionReLu(1.0), 1.0);
}

// TEST(NeuralNetwork, doMutations) {
//     NeuralNetwork net = BuildOnesNetwork({2, 3, 10, 2});
//     size_t numBiasMutations = 0;
//     size_t numWeightMutations = 0;

//     net.MutateBiasesAndWeights(&numBiasMutations, &numWeightMutations);
//     std::cout << "biases: " << numBiasMutations << "\n";
//     std::cout << "weights: " << numWeightMutations << "\n";
// }

TEST(NeuralNetwork, RemoveRandomNeuron) {
    NeuralNetwork net = BuildOnesNetwork({2, 3, 10, 2});

    size_t startNumNeurons = net.getNumNeurons();
    net.RemoveRandomNeuron();
    ASSERT_EQ(net.getNumNeurons() + 1, startNumNeurons);
}

TEST(NeuralNetwork, RemoveRandomNeuronAndLayer) {
    NeuralNetwork net = BuildOnesNetwork({2, 1, 1, 2});

    size_t startNumNeurons = net.getNumNeurons();
    size_t startNumLayers = net.getDepth();
    net.RemoveRandomNeuron();
    ASSERT_EQ(net.getNumNeurons() + 1, startNumNeurons);
    ASSERT_EQ(net.getDepth() + 1, startNumLayers);
}

TEST(NeuralNetwork, AddRandomNeuron) {
    NeuralNetwork net = BuildOnesNetwork({2, 3, 10, 2});

    size_t startNumNeurons = net.getNumNeurons();
    net.AddRandomNeuron();
    ASSERT_EQ(net.getNumNeurons(), startNumNeurons + 1);
}

// TEST(What, theHeck) {
//     auto net = BuildRandomNetwork({2, 5, 5, 5, 3});
//     size_t numBiasMutations = 0;
//     size_t numWeightMutations = 0;
//     net.MutateBiasesAndWeights(&numBiasMutations, &numWeightMutations);
// }