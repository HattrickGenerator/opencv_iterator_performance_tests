#pragma once
#include "opencv2/core.hpp"
#include <tuple>
#include <type_traits>

namespace experimental {

/// We recursively go through the variadic template pack and use overload
/// resolution to either
/// append elements to the tuple that are not inheriting (i.e. are no) openCV
/// iterators and replace the openCV iterators with a pointer to their data.
/// This is part 1) of 2) In another header we check if this is something valid
/// to do

/// Definitions for being globally accessible
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

template <
    typename Tpl, typename Arg, typename... Args,
    std::enable_if_t<!std::is_base_of<cv::MatConstIterator, Arg>::value, bool>>
constexpr auto make_tpl_replaced_pair(Tpl tpl, Arg arg, Args... args) {

  auto replace_tpl = make_tpl_replaced(tpl, arg, args...);
  return std::make_pair(replace_tpl,
                        std::tuple_size<decltype(replace_tpl)>::value);
}

} // namespace experimental
