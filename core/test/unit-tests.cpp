#include "core/math.h"
#include "core/neural_network.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace core;

TEST(NeuralNetwork, topology) {
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

TEST(NeuralNetwork, computeBasic) {
    NeuralNetwork net = BuildOnesNetwork({2, 3, 10, 2});

    EXPECT_THAT(net.compute({0, 0}), ElementsAre(41, 41));
    EXPECT_THAT(net.compute({1, 0}), ElementsAre(71, 71));
    EXPECT_THAT(net.compute({0, 1}), ElementsAre(71, 71));
    EXPECT_THAT(net.compute({1, 1}), ElementsAre(101, 101));
    EXPECT_THAT(net.compute({-1, -1}), ElementsAre(11, 11));
    EXPECT_THAT(net.compute({-1, 0}), ElementsAre(11, 11));
    EXPECT_THAT(net.compute({0, -1}), ElementsAre(11, 11));

    net = BuildOnesNetwork({1, 1000, 1000, 1000, 1000, 1});
    net.compute({0.5});
}

TEST(ReLu, basic) {
    EXPECT_EQ(ActivationFunctionReLu(-1.0), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(-1.e-6), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(-0.0), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(0.0), 0.0);
    EXPECT_EQ(ActivationFunctionReLu(1.e6), 1.e6);
    EXPECT_EQ(ActivationFunctionReLu(1.0), 1.0);
}