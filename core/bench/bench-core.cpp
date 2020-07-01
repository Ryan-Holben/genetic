#include <bench/benc.h>
#include <core/math.h>
#include <core/random.h>

using namespace core;

void RandIntDistributionConstructor() {
    RandIntDistribution r(1, 10);
}

void DiceConstructor() {
    Dice r(0.1);
}

void RandIntDistributionGet(RandIntDistribution& rand) {
    rand.get();
}

void RandRealDistributionGet(RandRealDistribution& rand) {
    rand.get();
}

void RandRealNormalDistributionGet(RandRealNormalDistribution& rand) {
    rand.get();
}

void DiceRoll(Dice& rand) {
    rand.roll();
}

int main(int argc, char *argv[]) {
    DECLARE_BENCHMARK_SET(coreBench, argc, argv);

    // ********** random.h ********** //
    RUN_BENCH(coreBench, RandIntDistributionConstructor, "basic", {
        return std::make_tuple();
    });

    RUN_BENCH(coreBench, DiceConstructor, "basic", {
        return std::make_tuple();
    });

    RUN_BENCH(coreBench, RandIntDistributionGet, "basic", {
        return std::make_tuple(RandIntDistribution(1,50));
    });


    RUN_BENCH(coreBench, RandRealDistributionGet, "basic", {
        return std::make_tuple(RandRealDistribution(1,50));
    });

    RUN_BENCH(coreBench, RandRealNormalDistributionGet, "basic", {
        return std::make_tuple(RandRealNormalDistribution(1,50));
    });

    RUN_BENCH(coreBench, DiceRoll, "basic", {
        return std::make_tuple(Dice(0.1));
    });

    // ********** math.h ********** //
    RUN_BENCH(coreBench, ActivationFunctionReLu, "basic", {
        RandRealDistribution rand(-1.0, 1.0);
        return std::make_tuple(rand.get());
    });

    RUN_BENCH(coreBench, L2_SQUARED, "5", {
        RandRealDistribution rand(-10.0, 10.0);
        Tuple x;
        Tuple y;
        for (size_t i = 0; i < 10; i++) {
            x.push_back(rand.get());
            y.push_back(rand.get());
        }
        return std::make_tuple(x, y);
    });

    RUN_BENCH(coreBench, L2_SQUARED, "50", {
        RandRealDistribution rand(-10.0, 10.0);
        Tuple x;
        Tuple y;
        for (size_t i = 0; i < 50; i++) {
            x.push_back(rand.get());
            y.push_back(rand.get());
        }
        return std::make_tuple(x, y);
    });

    END_BENCHMARK_SET(coreBench);

    return 0;
}