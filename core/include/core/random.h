#pragma once

#include <core/platform.h>
#include <core/types.h>
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
    RandIntDistribution(int low, int high) : RandBase() { setDist(low, high); }
    void setDist(int low, int high) {
        ASSERT(low < high);
        _dist = std::make_unique<std::uniform_int_distribution<>>(low, high);
    }
    int get() { return (*_dist)(_rng); }

  private:
    std::unique_ptr<std::uniform_int_distribution<>> _dist;
};

// A uniform random distribution of real Numbers in the closed interval [low, high]
class RandRealDistribution : RandBase {
  public:
    RandRealDistribution(Number low, Number high) : RandBase() { setDist(low, high); }
    void setDist(Number low, Number high) {
        ASSERT(low < high);
        _dist = std::make_unique<std::uniform_real_distribution<>>(low, high);
    }
    Number get() { return (*_dist)(_rng); }

  private:
    std::unique_ptr<std::uniform_real_distribution<>> _dist;
};

// A uniform random distribution of real Numbers in the closed interval [low, high]
class RandRealNormalDistribution : RandBase {
  public:
    RandRealNormalDistribution(Number low, Number high) : RandBase() { setDist(low, high); }
    void setDist(Number low, Number high) {
        ASSERT(low < high);
        _dist = std::make_unique<std::normal_distribution<>>(low, high);
    }
    Number get() { return (*_dist)(_rng); }

  private:
    std::unique_ptr<std::normal_distribution<>> _dist;
};

// A die roll / coinflip with a fixed probability of returning true
class Dice : protected RandRealDistribution {
  public:
    Dice(Number probabilityOfOccurance)
        : RandRealDistribution(0.0, 1.0), _probabilityOfOccurance(probabilityOfOccurance) {}
    Number roll() { return get() <= _probabilityOfOccurance; }

  private:
    Number _probabilityOfOccurance;
};

} // namespace core