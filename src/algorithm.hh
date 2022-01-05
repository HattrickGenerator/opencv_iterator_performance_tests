#include "opencv2/core.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>
#include <random>
#include <tuple>
#include <type_traits>
#include <utility>
namespace experimental {

/// Compile time loop over a tuple.Thanks to
// https://stackoverflow.com/questions/37602057/why-isnt-a-for-loop-a-compile-time-expression
template <std::size_t N> struct num { static const constexpr auto value = N; };

template <class F, std::size_t... Is>
void for_(F func, std::index_sequence<Is...>) {
  using expander = int[];
  (void)expander{0, ((void)func(num<Is>{}), 0)...};
}

template <std::size_t N, typename F> void for_(F func) {
  for_(func, std::make_index_sequence<N>());
}

template <typename Tpl, typename Arg>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg) {
  if constexpr (!std::is_base_of<cv::MatConstIterator, Arg>::value) {
    auto tpl_ptr = std::make_tuple(arg);
    return std::tuple_cat(tpl, tpl_ptr);
  }

  else {
    auto tpl_ptr = std::make_tuple(arg.ptr);
    return std::tuple_cat(tpl, tpl_ptr);
  }
}

/// This is how we loop through the tuple.
/// We replace all instances of a cv::MatIterator  with its pointer.
/// We recursively shave off one argument of the variadic template pack
template <typename Tpl, typename Arg, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args) {

  if constexpr (!std::is_base_of<cv::MatConstIterator, Arg>::value) {
    auto tpl_ptr = std::make_tuple(arg);
    auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
    return make_tpl_replaced(tpl_cat, args...); // TODO: has to be right type
  }

  else {
    auto tpl_ptr = std::make_tuple(arg.ptr);
    auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
    return make_tpl_replaced(tpl_cat, args...); // TODO: has to be right type
  }
}

/// This is how we loop through the tuple.
/// We replace all instances of a cv::MatIterator  with its pointer.
/// Last round in the recursive call

/// These two functions replace a c++17 if constexpr.
/// We return true in the generic case because that's the neutral element of the
/// && operation
template <typename Arg,
          typename std::enable_if<!std::is_base_of<
              cv::MatConstIterator, Arg>::value>::type * = nullptr>
std::pair<bool, bool> __isContinuous_single(Arg arg) {
  (void)arg; // unused parameter. Only used for overload resolution
  return std::make_pair(true, false);
}

/// These two functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous
std::pair<bool, bool> __isContinuous_single(cv::MatConstIterator it) {
  return std::make_pair(it.m->isContinuous(), true);
}

/// We loop through the touple and try to find if we can replace iterators by
/// its pointers This is only valid, if there cv::Mat matrices described by the
/// iterators in there, that are all contiguous in memory!
template <typename Tpl> bool __iteratorsReplaceable(Tpl tpl) {
  bool isContinuous = true; // runtime
  bool oneElement = false;

  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;

  // Here we for loop
  for_<tupleSize>([&](auto i) {
    std::pair<bool, bool> continuousPair =
        __isContinuous_single(std::get<i.value>(tpl));
    isContinuous &= continuousPair.first;
    oneElement |= continuousPair.second;
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
  bool isContinuous = __iteratorsReplaceable(tpl);

  auto replacedTpl = make_tpl_replaced(std::tuple<>{}, args...);
  if (!isContinuous) {
    return __count_if(replacedTpl, std::make_index_sequence<tupleSize>());
  } else {
    return std::count_if(std::forward<Args>(args)...);
  }
}
} // namespace experimental
