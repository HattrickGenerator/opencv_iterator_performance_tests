#include "algorithm.hh"
#include "opencv2/core.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>

#include <random>

template <typename T, int U> static void BM_count_if(benchmark::State &state) {
  double lower_bound = 1;
  double upper_bound = 10000;
  std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
  std::default_random_engine re;

  cv::Mat_<T> mat(U, U);
  std::generate(mat.begin(), mat.end(), [&re, &unif]() { return T(unif(re)); });

  // cv::Mat_<T> matSub = mat(cv::Rect(4, 4, U - 10, U - 10));

  T d = 0;
  for (auto _ : state) {

    // T *beginPtr = reinterpret_cast<T *>(mat.data);
    // T *endPtr = reinterpret_cast<T *>(mat.data) + mat.cols * mat.rows;
    // d = std::count_if(mat.begin(), mat.end(),
    //                [](T val) { return val != T(0); });

    count_if(mat.begin(), mat.end(), [](T val) { return 2 > val; });
    // d = cv::countNonZero(matSub);
    benchmark::DoNotOptimize(mat);
  }
}

template <typename T, int U>
static void BM_count_if_forward(benchmark::State &state) {
  double lower_bound = 1;
  double upper_bound = 10000;
  std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
  std::default_random_engine re;

  cv::Mat_<T> mat(U, U);
  std::generate(mat.begin(), mat.end(), [&re, &unif]() { return T(unif(re)); });

  // cv::Mat_<T> matSub = mat(cv::Rect(4, 4, U - 10, U - 10));

  T d = 0;
  for (auto _ : state) {

    // T *beginPtr = reinterpret_cast<T *>(mat.data);
    // T *endPtr = reinterpret_cast<T *>(mat.data) + mat.cols * mat.rows;
    transform(mat.begin(), mat.end(), mat.begin(),
              [](T val) { return 2 * val; });

    // d = cv::countNonZero(matSub);
    benchmark::DoNotOptimize(mat);
  }
}

// Register the function as a benchmark
// BENCHMARK_TEMPLATE(BM_countNZero, char, 4 * 1000000);

BENCHMARK_TEMPLATE(BM_count_if, char, 4 * 100);
BENCHMARK_TEMPLATE(BM_count_if_forward, char, 4 * 100);

BENCHMARK_MAIN();
