#include <core/core.h>
#include <datasets/mnist.h>
#include <evo/evo.h>

#include <iostream>

// using namespace core;

int main(int argc, char** argv) {
    core::InstallBacktraceHandler();

    auto dataset = GetMnistTrainingSet();         // or = GetXorDataset()
    if (dataset.empty()) {
        std::cout << "MNIST failed somehow!\n";
        return 1;
    }
    std::cout << "Loaded " << dataset.size() << " entries.\n";

    evo::Evolver evo;
    evo.installTrainingData(std::move(dataset));
    evo.runAlgorithm(/* initialPopulation = */ 10,
                     /* maxGenerations = */ 20,
                     /* targetScore = */ 0.01,
                     /* defaultTopology = */ {28*28, 10, 5, 1});

    // core::NeuralNetwork net;
    // net.buildOnesNetwork({2, 3, 19, 1});
    // core::Tuple output = net.compute({-0.5, -0.2});
    // std::cout << "The output was " << output[0] << "\n";

    // for (const auto& datum : xorDataset) {
    //     core::Tuple output = net.compute(datum.first);
    //     std::cout << "Input was [" << datum.first[0] << ", " << datum.first[1] << "], we wanted "
    //               << datum.second[0] << " and we got " << output[0] << "\n";
    // }
    std::cout << "\n";

    return 0;
}