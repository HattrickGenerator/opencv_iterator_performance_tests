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
/// This is how we loop through the tuple.
/// We replace all instances of a cv::MatIterator  with its pointer.
/// We recursively shave off one argument of the variadic template pack
template <typename Tpl, typename Arg, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg, Args... args) {

  auto tpl_ptr = std::make_tuple(arg);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...); // TODO: has to be right type
}

/// MatIterator_<T>  is a better fit than the generic argument
template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatIterator_<T> arg,
                                 Args... args) {

  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...);
}

/// MatConstIterator_<T> is a better fit than the generic argument
template <typename Tpl, typename T, typename... Args>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatConstIterator_<T> arg,
                                 Args... args) {

  auto tpl_ptr = std::make_tuple((T *)arg.ptr);
  auto tpl_cat = std::tuple_cat(tpl, tpl_ptr);
  return make_tpl_replaced(tpl_cat, args...);
}

/// This is how we loop through the tuple.
/// We replace all instances of a cv::MatIterator  with its pointer.
/// Last round in the recursive call
template <typename Tpl, typename Arg>
constexpr auto make_tpl_replaced(Tpl tpl, Arg arg) {
  auto tpl_base = std::make_tuple(arg);
  return std::tuple_cat(tpl, tpl_base);
}

/// MatIterator_<T> is a better fit than the generic argument
template <typename Tpl, typename T>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatIterator_<T> arg) {
  auto tpl_ptr = std::make_tuple(arg.ptr);
  return std::tuple_cat(tpl, tpl_ptr);
}

/// MatConstIterator_<T> is a better fit than the generic argument
template <typename Tpl, typename T>
constexpr auto make_tpl_replaced(Tpl tpl, cv::MatConstIterator_<T> arg) {
  auto tpl_ptr = std::make_tuple(arg.ptr);
  return std::tuple_cat(tpl, tpl_ptr);
}

/// These functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous and if we've had at least one opencv
/// iterator
template <typename Arg, typename... Args>
bool __isContinuous_variadic(bool isContinuous, Arg arg, Args... args) {
  (void)arg; // unused parameter. Only used for overload resolution
  return __isContinuous_variadic(isContinuous, args...);
}

/// These functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous and if we've had at least one opencv
/// iterator
template <typename T, typename... Args>
std::pair<bool, bool>
__isContinuous_variadic(std::pair<bool, bool> continuousPair,
                        cv::MatConstIterator_<T> it, Args... args) {
  return std::make_pair(continuousPair.first & it.m->isContinuous(), true);
}

/// These functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous and if we've had at least one opencv
/// iterator
template <typename T, typename... Args>
std::pair<bool, bool>
__isContinuous_variadic(std::pair<bool, bool> continuousPair,
                        cv::MatIterator_<T> it, Args... args) {
  return std::make_pair(continuousPair.first & it.m->isContinuous(), true);
}

/// These functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous and if we've had at least one opencv
/// iterator
template <typename Arg>
std::pair<bool, bool>
__isContinuous_variadic(std::pair<bool, bool> continuousPair, Arg arg) {
  (void)arg; // unused parameter. Only used for overload resolution
  return continuousPair;
}

/// These functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous and if we've had at least one opencv
/// iterator
template <typename T>
std::pair<bool, bool>
__isContinuous_variadic(std::pair<bool, bool> continuousPair,
                        cv::MatConstIterator_<T> it) {
  return std::make_pair(continuousPair.first & it.m->isContinuous(), true);
}

/// These functions replace a c++17 if constexpr.
/// We return if the iterator is contiguous and if we've had at least one opencv
/// iterator
template <typename T>
std::pair<bool, bool>
__isContinuous_variadic(std::pair<bool, bool> continuousPair,
                        cv::MatIterator_<T> it) {
  return std::make_pair(continuousPair.first & it.m->isContinuous(), true);
}

/// We loop through the touple
template <typename... Args> bool __isContinuous(Args... args) {
  bool isContinuous = true;
  bool oneElement = false;

  std::pair<bool, bool> continousPair = __isContinuous_variadic(
      std::make_pair(isContinuous, oneElement), args...);

  return continousPair.first && continousPair.second;
}

template <typename... Args, std::size_t... Is>
auto __count_if(std::tuple<Args...> tpl, std::index_sequence<Is...>) {
  return std::count_if(std::get<Is>(tpl)...);
}

template <typename... Args> auto count_if(Args... args) {

  std::tuple<Args...> tpl = std::make_tuple(args...);
  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;
  bool isContinuous = __isContinuous(args...);

  // std::cout << isContinuous << std::endl;

  std::cout << isContinuous << std::endl;

  auto replacedTpl = make_tpl_replaced(std::tuple<>{}, args...);
  if (isContinuous) {
    return __count_if(replacedTpl, std::make_index_sequence<tupleSize>());
  } else {
    return std::count_if(std::forward<Args>(args)...);
  }
}
} // namespace experimental
