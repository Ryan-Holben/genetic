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
        ASSERT(low <= high);
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

// A Gaussian/normal distribution with a mean and a standard deviation, with the option to clamp the
// output to a fixed interval [minVal, maxVal].
class RandGaussianDistribution : RandBase {
  public:
    RandGaussianDistribution(Number mean, Number stdev, Number minVal, Number maxVal) : RandBase() {
        setDist(mean, stdev, minVal, maxVal);
    }
    void setDist(Number mean, Number stdev, Number minVal, Number maxVal) {
        ASSERT(minVal < mean);
        ASSERT(mean < maxVal);
        _dist = std::make_unique<std::normal_distribution<>>(mean, stdev);
        _minVal = minVal;
        _maxVal = maxVal;
    }
    Number get() {
        const Number val = (*_dist)(_rng);
        if (val < _minVal) {
            return _minVal;
        } else if (val > _maxVal) {
            return _maxVal;
        } else {
            return val;
        }
    }

  private:
    std::unique_ptr<std::normal_distribution<>> _dist;
    Number _minVal;
    Number _maxVal;
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