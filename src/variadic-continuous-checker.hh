#pragma once
#include "opencv2/core.hpp"
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

/// This is how we loop through the tuple.
/// We replace all instances of a cv::MatIterator  with its pointer.
/// Last round in the recursive call

/// These two functions replace a c++17 if constexpr.
/// We return true in the generic case because that's the neutral element of the
/// && operation
template <typename Arg,
          std::enable_if_t<!std::is_base_of<cv::MatConstIterator, Arg>::value,
                           bool> = true>
std::pair<bool, bool> __iterator__replaceable(Arg arg) {
  (void)arg; // unused parameter. Only used for overload resolution
  return std::make_pair(true, false);
}

/// These two functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous
std::pair<bool, bool> __iterator__replaceable(cv::MatConstIterator it) {
  return std::make_pair(it.m->isContinuous(), true);
}

/// We loop through the touple and try to find if we can replace iterators by
/// its pointers This is only valid, if there cv::Mat matrices described by the
/// iterators in there, that are all contiguous in memory!
template <typename Tpl> bool __iterators__replaceable(Tpl tpl) {
  bool isContinuous = true; // runtime
  bool oneElement = false;

  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;

  // Here we for loop
  for_<tupleSize>([&](auto i) {
    std::pair<bool, bool> continuousPair =
        __iterator__replaceable(std::get<i.value>(tpl));
    isContinuous &= continuousPair.first;
    oneElement |= continuousPair.second;
  });

  return isContinuous && oneElement;
}
} // namespace experimental
