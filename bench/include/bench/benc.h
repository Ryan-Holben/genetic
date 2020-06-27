#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <math.h>
#include <vector>

namespace bench {

constexpr char* COL_NONE = "\x1b[0m";
constexpr char* COL_RED = "\x1b[0;31m";
constexpr char* COL_RED_BOLD = "\x1b[1;31m";
constexpr char* COL_GREEN = "\x1b[0;32m";
constexpr char* COL_GREEN_BOLD = "\x1b[1;32m";
constexpr char* COL_YELLOW = "\x1b[0;33m";
constexpr char* COL_YELLOW_BOLD = "\x1b[1;33m";
constexpr char* COL_BLUE = "\x1b[0;34m";
constexpr char* COL_BLUE_BOLD = "\x1b[1;34m";
constexpr char* COL_PINK = "\x1b[0;35m";
constexpr char* COL_PINK_BOLD = "\x1b[1;35m";

// ******************* Math *******************
double mean(std::vector<double> vec) {
    double sum = 0.0;
    for (size_t i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum / vec.size();
}

double variance(std::vector<double> vec) {
    double sum = 0.0;
    for (size_t i = 0; i < vec.size() - 1; i++) {
        double diff = vec[i + 1] - vec[i];
        sum += diff * diff;
    }
    return sum / (vec.size() - 1);
}

double standardDeviation(std::vector<double> vec) { return sqrt(variance(vec)); }

// ******************* Benchmarking classes *******************
using benchWrapper = std::function<void()>;

struct benchmark {
    benchWrapper wrapper;
    std::string minorName;

    std::vector<double> _times;
    size_t _iterations = 0;
    double _mean = 0.0;
    double _variance = 0.0;
    double _standardDeviation = 0.0;

    void runAndComputeStats() {
        for (size_t i = 0; i < 10; i++) {
            _times.push_back(timeFunctionCall().count());
        }
        _iterations = _times.size();
        _mean = mean(_times);
        _variance = variance(_times);
        _standardDeviation = standardDeviation(_times);
    }

    std::chrono::duration<double> timeFunctionCall() {
        auto start = std::chrono::high_resolution_clock::now();
        wrapper();
        auto end = std::chrono::high_resolution_clock::now();
        return end - start;
    }
};

class benchmarker {
  public:
    benchmarker(std::string name, std::string path) : _name(name), _path(path) {}

    void main() {
        std::cout << "\nBenchmark suite " << COL_RED_BOLD << _name << COL_NONE << " in " << COL_BLUE
                  << _path << COL_NONE << "!\n";
        std::cout << "Running " << _benchmarks.size() << " benchmarks... ⏳\n\n";
        for (auto& bench : _benchmarks) {
            std::cout << "⏱  " << COL_PINK_BOLD <<  _name << "." << bench.minorName
                      << COL_NONE << " ▶︎▶︎ ";
            bench.runAndComputeStats();
            std::cout << COL_GREEN_BOLD << bench._mean << COL_NONE << " ± " << COL_GREEN
                      << bench._standardDeviation << COL_NONE << " (" << bench._iterations
                      << " iterations)\n";
        }
    }

    // void add(benchmark wrapper) { _benchmarks.push_back(wrapper); }
    void add(std::string minorName, benchWrapper func) {
        benchmark _b;
        _b.minorName = minorName;
        _b.wrapper = func;
        _benchmarks.push_back(_b);
    }

  private:
    std::vector<benchmark> _benchmarks;
    std::string _name;
    std::string _path;
};

// ******************* Macro-defined interface *******************
#define DECLARE_BENCHMARK_SET(name) bench::benchmarker name(#name, __FILE__);

#define ADD_BENCH(name, minorName, func)                                       \
    name.add(#minorName, []() func);

#define RUN_BENCHMARK_SET(name) name.main()

} // namespace bench