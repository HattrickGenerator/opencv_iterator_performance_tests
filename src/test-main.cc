#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>
#include <random>

#include "opencv2/core/mat.hpp"

[[nodiscard]]  double  add(float a, double b)
{
    return std::pow(a,b);
}
static void BM_StringCreation(benchmark::State& state) {
double lower_bound = 0;
    double upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    double a = unif(re);
    double b = unif(re );

cv::Mat_<double> mat(1,1);
double d;
  for (auto _ : state)
  {
        benchmark::DoNotOptimize(d= add(a,b) + mat.at<double>(0));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
