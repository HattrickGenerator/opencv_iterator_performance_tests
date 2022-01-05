#include "algorithm.hh"
#include "opencv2/core.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>

#include <gtest/gtest.h>
#include <random>

TEST(ContinuousDetection, continuousCvMat) {
  cv::Mat_<char> mat(10, 10, 4);

  EXPECT_TRUE(experimental::__iteratorsReplaceable(std::make_tuple(
      mat.begin(), mat.end(), [](auto a, auto b) { return a + b; })));

  // cv::Mat_<T> matSub = mat(cv::Rect(4, 4, U - 10, U - 10));
}

TEST(ContinuousDetection, nonContinuousCvMat) {
  cv::Mat_<char> mat(100, 100, 4);
  cv::Mat_<char> matSub = mat(cv::Rect(4, 4, 90, 90));

  EXPECT_FALSE(experimental::__iteratorsReplaceable(
      std::make_tuple(matSub.begin(), matSub.end(), mat.begin(),
                      [](auto a, auto b) { return a + b; })));
}

TEST(ContinuousDetection, vectorIterator) {

  std::vector<char> mat{1, 2, 3, 4, 1};

  EXPECT_FALSE(experimental::__iteratorsReplaceable(
      std::make_tuple(mat.begin(), mat.end(), mat.begin(),
                      [](auto a, auto b) { return a + b; })));
}

TEST(IteratorReplacement, tupleReplaceTest) {

  cv::Mat_<char> mat(10, 10, 4);

  auto tpl = experimental::make_tpl_replaced(std::tuple<>{}, mat.begin(),
                                             mat.end(), mat.begin());

  auto tpl_ref =
      std::make_tuple(mat.begin().ptr, mat.end().ptr, mat.begin().ptr);

  bool same = std::is_same<decltype(tpl), decltype(tpl_ref)>::value;
  EXPECT_TRUE(same);
}

/*
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
    std::transform(mat.begin(), mat.end(), mat.begin(),
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
*/

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
