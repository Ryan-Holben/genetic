#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <math.h>
#include <tuple>
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

// HSV<-->RGB code taken from the top answer in:
// https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
typedef struct {
    double r; // a fraction between 0 and 1
    double g; // a fraction between 0 and 1
    double b; // a fraction between 0 and 1
} rgb;

typedef struct {
    double h; // angle in degrees
    double s; // a fraction between 0 and 1
    double v; // a fraction between 0 and 1
} hsv;

hsv rgb2hsv(const rgb& in) {
    hsv out;
    double min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max > in.b ? max : in.b;

    out.v = max; // v
    delta = max - min;
    if (delta < 0.00001) {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) {           // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max); // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN; // its now undefined
        return out;
    }
    if (in.r >= max)                   // > is bogus, just keeps compilor happy
        out.h = (in.g - in.b) / delta; // between yellow & magenta
    else if (in.g >= max)
        out.h = 2.0 + (in.b - in.r) / delta; // between cyan & yellow
    else
        out.h = 4.0 + (in.r - in.g) / delta; // between magenta & cyan

    out.h *= 60.0; // degrees

    if (out.h < 0.0)
        out.h += 360.0;

    return out;
}

rgb hsv2rgb(const hsv& in) {
    double hh, p, q, t, ff;
    long i;
    rgb out;

    if (in.s <= 0.0) { // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if (hh >= 360.0)
        hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

#define COL_RGB(r, g, b) "\x1b[38;2;" << r << ";" << g << ";" << b << "m"

// ******************* Math *******************
double mean(const std::vector<double>& vec) {
    double sum = 0.0;
    for (size_t i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum / vec.size();
}

double variance(const std::vector<double>& vec, double mean) {
    double sum = 0.0;
    for (size_t i = 0; i < vec.size(); i++) {
        double diff = vec[i] - mean;
        sum += diff * diff;
    }
    return sum / (vec.size());
}

double standardDeviation(const double variance) { return sqrt(variance); }

// ******************* Benchmarking classes *******************
struct BenchmarkResult {
    std::string funcName;
    std::string testName;

    std::vector<double> times;
    size_t numIterations = 0;
    double mean = 0.0;
    double variance = 0.0;
    double standardDeviation = 0.0;
    double coefficientOfVariation = 0.0;
    double totalDuration = 0.0;
};

class Benchmarker {
  public:
    Benchmarker(std::string name, std::string path) : _name(name), _path(path) {
        std::cout << "\nRunning benchmarking set " << COL_RED_BOLD << name << COL_NONE << " in "
                  << COL_BLUE_BOLD << path << COL_NONE;
    }

    void printBenchmarkResult(const BenchmarkResult& bench, double minMean, double maxMean) {
        hsv baseGreen{93.66, 0.5861, 0.34};
        // Logarithmic scale
        double B = (1.0 - baseGreen.v) / log10(maxMean / minMean);
        double A = 1.0 - B * log10(maxMean);
        double newV = A + B * log10(bench.mean);
        baseGreen.v = newV;

        rgb newGreen = hsv2rgb(baseGreen);
        int r = (int)(newGreen.r * 255);
        int g = (int)(newGreen.g * 255);
        int b = (int)(newGreen.b * 255);

        std::cout << "⏱  " << COL_PINK_BOLD << _name << "." << bench.funcName << "."
                  << bench.testName << COL_NONE << " ▶︎▶︎ " << COL_RGB(r, g, b)
                  << bench.mean << COL_NONE << " ± " << COL_GREEN << bench.coefficientOfVariation
                  << "%" << COL_NONE << " (" << bench.numIterations << " iterations, "
                  << bench.totalDuration << " s)\n";
    }

    template <typename A, typename B>
    void runBenchmark(std::string funcName, std::string testName, A func, B generator) {
        std::cout << "." << std::flush;
        BenchmarkResult bench;
        bench.funcName = funcName;
        bench.testName = testName;

        bench.totalDuration = 0.0;
        bench.numIterations = 0;
        const auto start = std::chrono::high_resolution_clock::now();
        while (true) {
            bench.times.push_back(timeFunctionCall(func, generator).count());
            bench.numIterations++;
            bench.totalDuration += bench.times.back();

            // Stop when 1000 runs happen, OR if it's taking too long, then at least get 50 runs.
            if (bench.numIterations >= 1000 ||
                (bench.totalDuration >= 1.0 && bench.numIterations >= 50)) {
                break;
            }
        }

        bench.mean = mean(bench.times);
        bench.variance = variance(bench.times, bench.mean);
        bench.standardDeviation = standardDeviation(bench.variance);
        bench.coefficientOfVariation = 100.0 * bench.standardDeviation / bench.mean;

        _results.push_back(bench);
    }

    template <typename A, typename B>
    std::chrono::duration<double> timeFunctionCall(A func, B generator) {
        auto args = generator();
        const auto start = std::chrono::high_resolution_clock::now();
        std::apply(func, args);
        const auto end = std::chrono::high_resolution_clock::now();
        return end - start;
    }

    void finish() {
        std::cout << "\n";
        std::sort(_results.begin(), _results.end(),
                  [](BenchmarkResult& a, BenchmarkResult& b) { return a.mean > b.mean; });
        const double maxMean = _results.front().mean;
        const double minMean = _results.back().mean;

        for (const auto& result : _results) {
            // std::cout << result.mean << "\t" << result.funcName << "." << result.testName <<
            // "\n";
            printBenchmarkResult(result, minMean, maxMean);
        }
    }

  private:
    std::vector<BenchmarkResult> _results;
    std::string _name;
    std::string _path;
};

// ******************* Macro-defined interface *******************
#define DECLARE_BENCHMARK_SET(name) bench::Benchmarker name(#name, __FILE__);

#define RUN_BENCH(name, func, testname, generator)                                                 \
    name.runBenchmark(#func, testname, func, []() generator);

#define END_BENCHMARK_SET(name) name.finish();

} // namespace bench