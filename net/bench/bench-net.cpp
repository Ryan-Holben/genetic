#include <bench/benc.h>
#include <iostream>

#include <net/neural_network.h>

void ComputeNeuralNetwork(const net::NeuralNetwork& net, const Tuple& input) { net.compute(input); }

void AddRandomNeuron(net::NeuralNetwork& net) { net.AddRandomNeuron(); }

void AddRandomLayer(net::NeuralNetwork& net) { net.AddRandomLayer(); }

void MutateBiasesAndWeights(net::NeuralNetwork& net) {
    size_t numBiasMutations, numWeightMutations;
    net.MutateBiasesAndWeights(&numBiasMutations, &numWeightMutations);
}

int main(int argc, char *argv[]) {
    DECLARE_BENCHMARK_SET(netBench, argc, argv);

    // ********** NetworkBuilding ********** //
    RUN_BENCH(netBench, net::BuildOnesNetwork, "2x5x5x5x3", {
        std::vector<size_t> layerSizes({2, 5, 5, 5, 3});
        return std::make_tuple(layerSizes);
    });

    RUN_BENCH(netBench, net::BuildOnesNetwork, "2x100x100x100x3", {
        std::vector<size_t> layerSizes({2, 100, 100, 100, 3});
        return std::make_tuple(layerSizes);
    });

    RUN_BENCH(netBench, net::BuildOnesNetwork, "10x1000x5x3", {
        std::vector<size_t> layerSizes({10, 1000, 5, 3});
        return std::make_tuple(layerSizes);
    });

    RUN_BENCH(netBench, net::BuildRandomNetwork, "2x5x5x5x3", {
        std::vector<size_t> layerSizes({2, 5, 5, 5, 3});
        return std::make_tuple(layerSizes);
    });

    RUN_BENCH(netBench, net::BuildRandomNetwork, "2x100x100x100x3", {
        std::vector<size_t> layerSizes({2, 100, 100, 100, 3});
        return std::make_tuple(layerSizes);
    });

    RUN_BENCH(netBench, net::BuildRandomNetwork, "10x1000x5x3", {
        std::vector<size_t> layerSizes({10, 1000, 5, 3});
        return std::make_tuple(layerSizes);
    });

    // ********** Net compute() times ********** //
    RUN_BENCH(netBench, ComputeNeuralNetwork, "2x5x5x5x3", {
        thread_local RandRealDistribution rand(-5.0, 5.0);
        const auto network = net::BuildRandomNetwork({2, 5, 5, 5, 3});
        Tuple input({rand.get(), rand.get()});
        return std::make_tuple(network, input);
    });

    RUN_BENCH(netBench, ComputeNeuralNetwork, "2x100x100x100x3", {
        thread_local RandRealDistribution rand(-5.0, 5.0);
        const auto network = net::BuildRandomNetwork({2, 100, 100, 100, 3});
        Tuple input({rand.get(), rand.get()});
        return std::make_tuple(network, input);
    });

    RUN_BENCH(netBench, ComputeNeuralNetwork, "10x1000x5x3", {
        thread_local RandRealDistribution rand(-5.0, 5.0);
        const auto network = net::BuildRandomNetwork({10, 1000, 5, 3});
        Tuple input({rand.get(), rand.get()});
        return std::make_tuple(network, input);
    });

    // ********** Mutations ********** //
    // ********** AddRandomNeuron ********** //
    RUN_BENCH(netBench, AddRandomNeuron, "2x5x5x5x3", {
        const auto network = net::BuildRandomNetwork({2, 5, 5, 5, 3});
        return std::make_tuple(network);
    });

    RUN_BENCH(netBench, AddRandomNeuron, "2x100x100x100x3", {
        const auto network = net::BuildRandomNetwork({2, 100, 100, 100, 3});
        return std::make_tuple(network);
    });

    RUN_BENCH(netBench, AddRandomNeuron, "10x1000x5x3", {
        const auto network = net::BuildRandomNetwork({10, 1000, 5, 3});
        return std::make_tuple(network);
    });

    // ********** AddRandomLayer ********** //
    RUN_BENCH(netBench, AddRandomLayer, "2x5x5x5x3", {
        const auto network = net::BuildRandomNetwork({2, 5, 5, 5, 3});
        return std::make_tuple(network);
    });

    RUN_BENCH(netBench, AddRandomLayer, "2x100x100x100x3", {
        const auto network = net::BuildRandomNetwork({2, 100, 100, 100, 3});
        return std::make_tuple(network);
    });

    RUN_BENCH(netBench, AddRandomLayer, "10x1000x5x3", {
        const auto network = net::BuildRandomNetwork({10, 1000, 5, 3});
        return std::make_tuple(network);
    });

    // ********** MutateBiasesAndWeights ********** //
    RUN_BENCH(netBench, MutateBiasesAndWeights, "2x5x5x5x3", {
        const auto network = net::BuildRandomNetwork({2, 5, 5, 5, 3});
        return std::make_tuple(network);
    });

    RUN_BENCH(netBench, MutateBiasesAndWeights, "2x100x100x100x3", {
        const auto network = net::BuildRandomNetwork({2, 100, 100, 100, 3});
        return std::make_tuple(network);
    });

    RUN_BENCH(netBench, MutateBiasesAndWeights, "10x1000x5x3", {
        const auto network = net::BuildRandomNetwork({10, 1000, 5, 3});
        return std::make_tuple(network);
    });

    END_BENCHMARK_SET(netBench);

    return 0;
}