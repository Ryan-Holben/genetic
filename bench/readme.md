# genetic//bench

My own lightweight benchmarking library.

I'll probably find a third-party library to do this, e.g. `Google Benchmark`, but the installation there seemed weirdly nontrivial, relying on very manual installation steps and `cmake` which I don't have.  The installation process seemed like it might be Linux only.  So here we are!

If this is successful, I might spin this out into its own project.


### Example: Benchmark set for a file

```c++
#include <bench/benc.h>

#include <core/random.h>
#include <evo/evo.h>

int myFunc(bool option, int a, int b) { return option ? a + b : a - b; }


int main() {
    DECLARE_BENCHMARK_SET(evoBench);

    RUN_BENCH(evoBench, myFunc, "hello", { return std::make_tuple(false, 10, 4); });
    
    // Example of passing in a lambda that will be generating random inputs for our function
    RUN_BENCH(evoBench, evo::GetNumChildren, "basic", {
        thread_local RandRealDistribution rand(0.0, 100.0);
        const double minVal = rand.get();
        const double ourVal = minVal + rand.get();
        const double maxVal = ourVal + rand.get();
        return std::make_tuple(ourVal, minVal, maxVal);
    });

    return 0;
}
```

with build file:

```
cc_binary(
    name = "benchmark",
    srcs = ["bench/my-benchmarks.cpp"],
    deps = ["//bench:benchlib"],
)
```

### Running all benchmarks

You can run all benchmarks across the project by simply running `bazel run bench`, which will find all specially-named build targets of the form `//[my_package]:benchmark`.

```
ryan@Ryans-MacBook-Pro ~/Documents/dev/genetic $% bazel run bench
INFO: Analyzed target //bench:bench (0 packages loaded, 31 targets configured).
INFO: Found 1 target...
Target //bench:bench up-to-date:
  bazel-bin/bench/bench
INFO: Elapsed time: 0.158s, Critical Path: 0.00s
INFO: 0 processes.
INFO: Build completed successfully, 1 total action
INFO: Build completed successfully, 1 total action
INFO: Analyzed target //net:benchmark (0 packages loaded, 10 targets configured).
INFO: Found 1 target...
Target //net:benchmark up-to-date:
  bazel-bin/net/benchmark
INFO: Elapsed time: 0.117s, Critical Path: 0.00s
INFO: 0 processes.
INFO: Build completed successfully, 1 total action
INFO: Build completed successfully, 1 total action

Running benchmarking set netBench in net/bench/bench-net.cpp..................
⏱  netBench.AddRandomLayer.10x1000x5x3 ▶︎▶︎ 0.0050335 ± 135.134% (199 iterations, 1.00167 s)
⏱  netBench.net::BuildRandomNetwork.2x100x100x100x3 ▶︎▶︎ 0.00252437 ± 7.21038% (397 iterations, 1.00218 s)
⏱  netBench.net::BuildRandomNetwork.10x1000x5x3 ▶︎▶︎ 0.00225104 ± 5.11332% (445 iterations, 1.00171 s)
⏱  netBench.MutateBiasesAndWeights.2x100x100x100x3 ▶︎▶︎ 0.00217474 ± 10.706% (460 iterations, 1.00038 s)
⏱  netBench.MutateBiasesAndWeights.10x1000x5x3 ▶︎▶︎ 0.00168361 ± 10.3933% (595 iterations, 1.00175 s)
⏱  netBench.net::BuildOnesNetwork.10x1000x5x3 ▶︎▶︎ 0.000534039 ± 60.2701% (1000 iterations, 0.534039 s)
⏱  netBench.net::BuildOnesNetwork.2x100x100x100x3 ▶︎▶︎ 0.000372308 ± 64.9136% (1000 iterations, 0.372308 s)
⏱  netBench.AddRandomNeuron.10x1000x5x3 ▶︎▶︎ 0.000329655 ± 43.0827% (1000 iterations, 0.329655 s)
⏱  netBench.AddRandomLayer.2x100x100x100x3 ▶︎▶︎ 0.000275129 ± 67.4988% (1000 iterations, 0.275129 s)
⏱  netBench.ComputeNeuralNetwork.2x100x100x100x3 ▶︎▶︎ 0.000179474 ± 13.5416% (1000 iterations, 0.179474 s)
⏱  netBench.ComputeNeuralNetwork.10x1000x5x3 ▶︎▶︎ 0.00017566 ± 15.9678% (1000 iterations, 0.17566 s)
⏱  netBench.AddRandomNeuron.2x100x100x100x3 ▶︎▶︎ 0.000109338 ± 57.0075% (1000 iterations, 0.109338 s)
⏱  netBench.net::BuildRandomNetwork.2x5x5x5x3 ▶︎▶︎ 2.30187e-05 ± 32.5847% (1000 iterations, 0.0230187 s)
⏱  netBench.net::BuildOnesNetwork.2x5x5x5x3 ▶︎▶︎ 1.45526e-05 ± 38.2129% (1000 iterations, 0.0145526 s)
⏱  netBench.MutateBiasesAndWeights.2x5x5x5x3 ▶︎▶︎ 1.11117e-05 ± 35.9657% (1000 iterations, 0.0111117 s)
⏱  netBench.AddRandomLayer.2x5x5x5x3 ▶︎▶︎ 6.91837e-06 ± 50.5131% (1000 iterations, 0.00691837 s)
⏱  netBench.AddRandomNeuron.2x5x5x5x3 ▶︎▶︎ 4.85406e-06 ± 79.3632% (1000 iterations, 0.00485406 s)
⏱  netBench.ComputeNeuralNetwork.2x5x5x5x3 ▶︎▶︎ 3.38757e-06 ± 81.075% (1000 iterations, 0.00338757 s)
INFO: Analyzed target //evo:benchmark (0 packages loaded, 2 targets configured).
INFO: Found 1 target...
Target //evo:benchmark up-to-date:
  bazel-bin/evo/benchmark
INFO: Elapsed time: 0.113s, Critical Path: 0.00s
INFO: 0 processes.
INFO: Build completed successfully, 1 total action
INFO: Build completed successfully, 1 total action

Running benchmarking set evoBench in evo/bench/bench-evo.cpp......
⏱  evoBench.SortGenerationByScores.100 ▶︎▶︎ 3.67552e-05 ± 16.7868% (1000 iterations, 0.0367552 s)
⏱  evoBench.SortGenerationByScores.50 ▶︎▶︎ 1.67351e-05 ± 17.8838% (1000 iterations, 0.0167351 s)
⏱  evoBench.SortGenerationByScores.10 ▶︎▶︎ 2.35348e-06 ± 34.9457% (1000 iterations, 0.00235348 s)
⏱  evoBench.KeepOnlyFirstAgents.nontrivial ▶︎▶︎ 7.99894e-07 ± 53.2664% (1000 iterations, 0.000799894 s)
⏱  evoBench.evo::GetNumChildren.basic ▶︎▶︎ 1.17034e-07 ± 381.859% (1000 iterations, 0.000117034 s)
⏱  evoBench.KeepOnlyFirstAgents.trivial ▶︎▶︎ 8.9187e-08 ± 33.3248% (1000 iterations, 8.9187e-05 s)
```