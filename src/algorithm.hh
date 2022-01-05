#pragma once

#include "opencv2/core.hpp"
#include "pointer-tuple-replacer.hh"
#include "variadic-continuous-checker.hh"
#include <algorithm>
#include <tuple>

namespace experimental {

template <typename... Args, std::size_t... Is>
auto count_if__contiguous(std::tuple<Args...> tpl, std::index_sequence<Is...>) {
  return std::count_if(std::get<Is>(tpl)...);
}

template <typename... Args> auto count_if(Args... args) {

  if (__iterators__replaceable(args...)) {
    return count_if__contiguous(
        make_tpl_replaced(std::tuple<>{}, args...),
        std::make_index_sequence<
            std::tuple_size<std::tuple<Args...>>::value>());
  } else {
    return std::count_if(std::forward<Args>(args)...);
  }
}

} // namespace experimental
