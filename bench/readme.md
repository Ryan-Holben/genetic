# genetic//bench

My own lightweight benchmarking library.

I'll probably find a third-party library to do this, e.g. `Google Benchmark`, but the installation there seemed weirdly nontrivial, relying on very manual installation steps and `cmake` which I don't have.  The installation process seemed like it might be Linux only.  So here we are!

If this is successful, I might spin this out into its own project.

Example usage:


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