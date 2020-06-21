#include "gtest/gtest.h"
#include <core/random.h>

using namespace core;

TEST(Random, randIntDistribution) {
    constexpr int MIN_VAL = -5;
    constexpr int MAX_VAL = 10;
    RandIntDistribution rand(MIN_VAL, MAX_VAL);

    for (size_t i = 0; i < 1000; i++) {
        int val = rand.get();
        ASSERT_LE(MIN_VAL, val);
        ASSERT_LE(val, MAX_VAL);
    }
}

TEST(Random, randRealDistribution) {
    constexpr Number MIN_VAL = -5.5;
    constexpr Number MAX_VAL = 10.1111;
    RandRealDistribution rand(MIN_VAL, MAX_VAL);

    for (size_t i = 0; i < 1000; i++) {
        Number val = rand.get();
        ASSERT_LE(MIN_VAL, val);
        ASSERT_LE(val, MAX_VAL);
    }
}