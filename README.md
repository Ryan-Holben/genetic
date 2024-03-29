# genetic

Build plastic neural networks by optimizing their structure and connections simultaneously using genetic algorithms.


## Useful links for development
* [Useful preprocessor macros](https://gcc.gnu.org/onlinedocs/cpp/Macros.html)
* [Unix Signals](http://people.cs.pitt.edu/~alanjawi/cs449/code/shell/UnixSignals.htm)
* [Common C++ Bazel BUILD patterns](https://docs.bazel.build/versions/master/cpp-use-cases.html)
* [Googletest](https://github.com/google/googletest)
* [Google benchmark](https://github.com/google/benchmark)
* [colorizer.org RGB<->HSV tool](http://colorizer.org/)


## Organization
* Neural network stuff
    * Neuron, Synapse
    * Layer
    * NeuralNetwork
        * BuildOnesNetwork()
        * BuildRandomNetwork()
        * Compute(input) -> output
        * MutateBiasesAndWeights()
        * AddRandomNeuron()
        * AddRandomNewLayer()
* Data stuff
    * Dataset class
    * Some standard datasets for testing with
* Ecosystem
    * Agent: a brain plus a fitness score
        * ScoreAgainstData()
        * GetNumberOfChildren()
        * DoMutations()
    * Generation: encapsulates a generation's set of agents, plus stats about them
    * Evolver: contains/runs the main evolution algorithm

# Architecture


![](architecture.gz.png)