#include <bench/benc.h>
#include <iostream>

#include <evo/evo.h>

int main() {
    DECLARE_BENCHMARK_SET(evoBench);
    DECLARE_BENCHMARK_SET(getNumChildren);

    ADD_BENCH(evoBench, basicTest, {
        double x = 1.0;
        for (int i = 0; i < 1000; i++) {
            x *= 1.1;
        }
    });

    

    ADD_BENCH(getNumChildren, basic, {
        evo::GetNumChildren(1.0, 0.1, 53.1);
    });

    RUN_BENCHMARK_SET(evoBench);
    RUN_BENCHMARK_SET(getNumChildren);
    return 0;
}