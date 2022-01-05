#pragma once

#include "opencv2/core.hpp"
#include "pointer-tuple-replacer.hh"
#include "variadic-continuous-checker.hh"
#include <algorithm>
#include <tuple>

namespace experimental {

template <typename... Args, std::size_t... Is>
auto __count_if(std::tuple<Args...> tpl, std::index_sequence<Is...>) {
  return std::count_if(std::get<Is>(tpl)...);
}

template <typename... Args> auto count_if(Args... args) {

  std::tuple<Args...> tpl = std::make_tuple(args...);
  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;
  bool isContinuous = __iterators__replaceable(tpl);

  auto replacedTpl = make_tpl_replaced(std::tuple<>{}, args...);
  if (!isContinuous) {
    return __count_if(replacedTpl, std::make_index_sequence<tupleSize>());
  } else {
    return std::count_if(std::forward<Args>(args)...);
  }
}
} // namespace experimental
