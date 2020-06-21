#pragma once

#include "types.h"
#include <memory>
#include <random>

namespace core {

// Base to the random distributions, not meant for public access.  Wraps access to /dev/urandom.
class RandBase {
  public:
    RandBase() {
        std::random_device randomDevice;
        _rng.seed(randomDevice());
    }

  protected:
    std::mt19937 _rng;
};

// A uniform random distribution of integers in the closed interval [low, high]
class RandIntDistribution : RandBase {
  public:
    RandIntDistribution(int low, int high) : RandBase(), _dist(low, high) {}
    int get() { return _dist(_rng); }

  private:
    std::uniform_int_distribution<> _dist;
};

class RandRealDistribution : RandBase {
  public:
    RandRealDistribution(Number low, Number high) : RandBase(), _dist(low, high) {}
    Number get() { return _dist(_rng); }

  private:
    std::uniform_real_distribution<> _dist;
};

} // namespace core