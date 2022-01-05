#pragma once

#include "opencv2/core.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>
#include <random>
#include <tuple>
#include <type_traits>
#include <utility>

namespace experimental { /// This is how we loop through the tuple.
/// We replace all instances of a cv::MatIterator  with its pointer.
/// We recursively shave off one argument of the variadic template pack

//*********************definitions*************************************/

template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatIterator_<T> arg,
                                       Args... args);

template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatConstIterator_<T> arg,
                                       Args... args);

template <typename Tpl, typename T>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatIterator_<T> arg);

template <typename Tpl, typename T>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatConstIterator_<T> arg);

// constexpr part 1
template <typename Tpl, typename Arg,
          std::enable_if_t<std::is_base_of<cv::MatConstIterator, Arg>::value,
                           bool> = true>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg);

template <typename Tpl, typename Arg,
          std::enable_if_t<!std::is_base_of<cv::MatConstIterator, Arg>::value,
                           bool> = true>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg);
template <typename Tpl, typename Arg, typename... Args,
          std::enable_if_t<std::is_base_of<cv::MatConstIterator, Arg>::value,
                           bool> = true>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args);

template <typename Tpl, typename Arg, typename... Args,
          std::enable_if_t<!std::is_base_of<cv::MatConstIterator, Arg>::value,
                           bool> = true>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args);
//*********************************************************************/

template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatIterator_<T> arg,
                                       Args... args) {
  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...);
}

template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatConstIterator_<T> arg,
                                       Args... args) {
  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...);
}

template <typename Tpl, typename T>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatIterator_<T> arg) {
  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  return std::tuple_cat(tpl, tpl_ptr);
}

template <typename Tpl, typename T>
constexpr auto make_tpl_replaced_cvMat(Tpl tpl, cv::MatConstIterator_<T> arg) {
  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  return std::tuple_cat(tpl, tpl_ptr);
}

// constexpr part 1
template <
    typename Tpl, typename Arg,
    std::enable_if_t<std::is_base_of<cv::MatConstIterator, Arg>::value, bool>>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg) {
  return make_tpl_replaced_cvMat(tpl, arg);
} // namespace experimental

// constexpr part 2
template <
    typename Tpl, typename Arg,
    std::enable_if_t<!std::is_base_of<cv::MatConstIterator, Arg>::value, bool>>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg) {
  auto tpl_ptr = std::make_tuple(arg);
  return std::tuple_cat(tpl, tpl_ptr);
} // namespace experimental

template <
    typename Tpl, typename Arg, typename... Args,
    std::enable_if_t<std::is_base_of<cv::MatConstIterator, Arg>::value, bool>>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args) {

  return make_tpl_replaced_cvMat(tpl, arg, args...);
}

template <
    typename Tpl, typename Arg, typename... Args,
    std::enable_if_t<!std::is_base_of<cv::MatConstIterator, Arg>::value, bool>>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args) {

  auto tpl_ptr = std::make_tuple(arg);
  auto tpl_cat = std::tuple_cat(tpl, std::make_tuple(arg));
  return make_tpl_replaced(tpl_cat, args...);
}

} // namespace experimental
