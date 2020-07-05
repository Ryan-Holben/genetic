#include <core/core.h>
#include <datasets/mnist.h>
#include <evo/evo.h>

#include <iostream>

int main(int argc, char** argv) {
    core::InstallBacktraceHandler();

    auto dataset = GetMnistTrainingSet(false); // = GetXorDataset();
    if (dataset.empty()) {
        std::cout << "Failed to load dataset!  Exiting.\n";
        return 1;
    }
    std::cout << "Loaded " << dataset.size() << " entries.\n";
    const size_t inputSize = dataset[0].first.size();
    const size_t outputSize = dataset[0].second.size();

    evo::Evolver evo;
    evo.installTrainingData(std::move(dataset));
    evo.runAlgorithm(/* initialPopulation = */ 10,
                     /* maxGenerations = */ 50,
                     /* targetScore = */ 0.01,
                     /* defaultTopology = */ {inputSize, 10, 5, 5, outputSize});
    evo.saveOutput("output");

    std::cout << "\n";

    return 0;
}