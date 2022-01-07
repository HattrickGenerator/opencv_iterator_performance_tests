#include "algorithm.hh"
#include "opencv2/core.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>

#include <gtest/gtest.h>
#include <random>

TEST(ContinuousDetection, continuousCvMat) {
  cv::Mat_<char> mat(10, 10, 4);

  EXPECT_TRUE(experimental::__iterators__replaceable(
      mat.begin(), mat.end(), [](auto a, auto b) { return a + b; }));
}

TEST(ContinuousDetection, nonContinuousCvMat) {
  cv::Mat_<char> mat(100, 100, 4);
  cv::Mat_<char> matSub = mat(cv::Rect(4, 4, 90, 90));

  EXPECT_FALSE(experimental::__iterators__replaceable(
      matSub.begin(), matSub.end(), mat.begin(),
      [](auto a, auto b) { return a + b; }));
}

TEST(ContinuousDetection, vectorIterator) {

  std::vector<int> mat{1, 2, 3, 4, 1};

  EXPECT_FALSE(experimental::__iterators__replaceable(
      mat.begin(), mat.end(), mat.begin(),
      [](auto a, auto b) { return a + b; }));
}

TEST(IteratorReplacement, tupleReplaceTest_cvIt) {

  cv::Mat_<int> mat(10, 10, 4);

  auto tpl = experimental::make_tpl_replaced(std::tuple<>{}, mat.begin(),
                                             mat.end(), mat.begin());

  auto tpl_ref = std::make_tuple((int *)mat.begin().ptr, (int *)mat.end().ptr,
                                 (int *)mat.begin().ptr);

  bool same = typeid(tpl) == typeid(tpl_ref);
  EXPECT_TRUE(same) << typeid(tpl).name() << " unequal reference "
                    << typeid(tpl_ref).name();
}

TEST(IteratorReplacement, tupleReplaceTest_vectorIt) {

  std::vector<double> mat{1, 3, 2, 4, 5};

  auto tpl = experimental::make_tpl_replaced(std::tuple<>{}, mat.begin(),
                                             mat.end(), mat.begin());

  auto tpl_ref = std::make_tuple(mat.begin(), mat.end(), mat.begin());

  bool same = typeid(tpl) == typeid(tpl_ref);
  EXPECT_TRUE(same) << typeid(tpl).name() << "unequal reference "
                    << typeid(tpl_ref).name();
}

