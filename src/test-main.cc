#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>
#include <random>

#include "opencv2/core/mat.hpp"

template <typename T, int U>
static void BM_AccumulateIterator(benchmark::State &state) {
  double lower_bound = 0;
  double upper_bound = 10000;
  std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
  std::default_random_engine re;

  cv::Mat_<T> mat(U, 1);
  std::generate(mat.begin(), mat.end(), [&re, &unif]() { return T(unif(re)); });

  T d = 0;
  for (auto _ : state) {

    d = std::accumulate(mat.begin(), mat.end(), T(0.0));
    benchmark::DoNotOptimize(d);
  }
}

template <typename T, int U>
static void BM_AccumulatePointer(benchmark::State &state) {
  double lower_bound = 0;
  double upper_bound = 10000;
  std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
  std::default_random_engine re;

  cv::Mat_<T> mat(U, 1);
  std::generate(mat.begin(), mat.end(), [&re, &unif]() { return T(unif(re)); });

  T d = 0;
  for (auto _ : state) {

    T *beginPtr = reinterpret_cast<T *>(mat.data);
    T *endPtr = reinterpret_cast<T *>(mat.data) + mat.cols * mat.rows;

    d = std::accumulate(beginPtr, endPtr, T(0.0));
    benchmark::DoNotOptimize(d);
  }
}
// Register the function as a benchmark
BENCHMARK_TEMPLATE(BM_AccumulateIterator, int, 1000);
BENCHMARK_TEMPLATE(BM_AccumulatePointer, int, 1000);

BENCHMARK_MAIN();
