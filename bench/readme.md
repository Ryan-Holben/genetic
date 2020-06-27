# genetic//bench

My own lightweight benchmarking library.

I'll probably find a third-party library to do this, e.g. `Google Benchmark`, but the installation there seemed weirdly nontrivial, relying on very manual installation steps and `cmake` which I don't have.  The installation process seemed like it might be Linux only.  So here we are!

If this is successful, I might spin this out into its own project.

Example usage:


```c++
#include <bench/benc.h>
#include <iostream>


int main() {
    benchmarker evoBench;

    BENCH(evoBench, evo, basicTest, {
       std::cout << "cool\n"; 
    })

    evoBench.main();
    return 0;
}
```

with build file:

```
cc_library(
    name = "evo",
    # ...
)

cc_binary(
    name = "benchmark",
    srcs = ["bench/bench-evo.cpp"],
    deps = [":evo", "//bench:benchlib"],
)
```