#include <core/core.h>
// #include <core/platform.h>
#include <datasets/xor.h>
#include <evo/evo.h>

#include <iostream>

// using namespace core;

int main(int argc, char** argv) {
    core::InstallBacktraceHandler();

    evo::Evolver evo;
    evo.installTrainingData(BuildXorDataset());
    evo.runAlgorithm(10, 3, 0.5, {2, 3, 5, 1});

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
}