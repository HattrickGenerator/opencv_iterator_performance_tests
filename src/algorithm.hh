#include "opencv2/core.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>
#include <random>
#include <tuple>
#include <type_traits>
#include <utility>

template <std::size_t N> struct num { static const constexpr auto value = N; };

template <class F, std::size_t... Is>
void for_(F func, std::index_sequence<Is...>) {
  using expander = int[];
  (void)expander{0, ((void)func(num<Is>{}), 0)...};
}

template <std::size_t N, typename F> void for_(F func) {
  for_(func, std::make_index_sequence<N>());
}

template <typename Tpl, typename Arg, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args) {

  auto tpl_ptr = std::make_tuple(arg);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...); // TODO: has to be right type
}

template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatIterator_<T> arg,
                                 Args... args) {

  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...);
}

template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatConstIterator_<T> arg,
                                 Args... args) {

  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...);
}

template <typename Tpl, typename Arg>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg) {
  auto tpl_base = std::make_tuple(arg);
  return std::tuple_cat(tpl, tpl_base);
}

template <typename Tpl, typename T>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatIterator_<T> arg) {
  auto tpl_ptr = std::make_tuple(arg.ptr);
  return std::tuple_cat(tpl, tpl_ptr);
}

template <typename Tpl, typename T>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatConstIterator_<T> arg) {
  auto tpl_ptr = std::make_tuple(arg.ptr);
  return std::tuple_cat(tpl, tpl_ptr);
}

template <typename Arg> bool __isContinuous_single(Arg arg) {
  (void)arg; // unused parameter. Only used for overload resolution
  return true;
}

bool __isContinuous_single(cv::MatConstIterator it) {
  return it.m->isContinuous();
}

/// Returns false if we don't need to change any iterators.
/// This can also mean, that there are no opencv iterators in here
template <typename Tpl> bool __isContinuous(Tpl tpl) {
  bool isContinuous = true; // runtime
  bool oneElement = false;

  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;

  // Here we for loop
  for_<tupleSize>([&](auto i) {
    isContinuous &= __isContinuous_single(std::get<i.value>(tpl));
  });

  return isContinuous && oneElement;
}

template <typename... Args, std::size_t... Is>
auto __count_if(std::tuple<Args...> tpl, std::index_sequence<Is...>) {
  return std::count_if(std::get<Is>(tpl)...);
}

template <typename... Args> auto count_if(Args... args) {

  std::tuple<Args...> tpl = std::make_tuple(args...);
  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;
  bool isContinuous = __isContinuous(tpl);

  auto replacedTpl = make_tpl_replaced(std::tuple<>{}, args...);
  if (!isContinuous) {
    return __count_if(replacedTpl, std::make_index_sequence<tupleSize>());
  } else {
    return std::count_if(std::forward<Args>(args)...);
  }
}
