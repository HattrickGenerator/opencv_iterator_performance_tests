#include "opencv2/core.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <iostream>
#include <math.h>
#include <random>
#include <type_traits>
#include <utility>

#include <tuple>

/*
template <typename T>
auto accumulate(cv::MatConstIterator_<T> a, cv::MatConstIterator_<T> b, T val) {
  if (a.m->isContinuous() && b.m->isContinuous()) {
    return std::accumulate((T *)(a.ptr), (T *)(b.ptr), val);
  } else {
    return std::accumulate(a, b, val);
  }
}

template <typename... Ts> void g(Ts... args) {
  f(&args...); // “&args...” is a pack expansion
               // “&args” is its pattern
}

template <typename... Args, typename U> void transform_var(Args &&... args) {
  std::transform(&args...);
}

template <typename... Args> void accumulate_var(Args &&... args) {
  std::accumulate(&args...);
}

template <typename T, typename U>
void transform(cv::MatConstIterator_<T> a, cv::MatConstIterator_<T> b,
               cv::MatConstIterator_<T> c, U lambda) {
  if (a.m->isContinuous() && b.m->isContinuous()) {

    std::transform((T *)(a.ptr), (T *)(b.ptr), (T *)(c.ptr), lambda);
  } else {

    std::transform(a, b, c, lambda);
  }
}
*/

template <typename... Args> auto transform(Args... args) {

  return std::transform(args...);
}

template <typename Last> std::string type_name() {
  return std::string(typeid(Last).name());
}

template <typename First, typename Second, typename... Rest>
std::string type_name() {
  return type_name<First>() + " " + type_name<Second, Rest...>();
}

template <class none = void> int f() { return 0; }
template <int First, int... Rest> int f() { return First + f<Rest...>(); }

// within all(), the unary left fold expands as
//  return ((true && true) && true) && false;
// b is false

/// Nifty Tricks
template <size_t... Indices> struct indices_holder {};

template <size_t index_to_add, typename Indices = indices_holder<>>
struct make_indices_impl;

template <size_t index_to_add, size_t... existing_indices>
struct make_indices_impl<index_to_add, indices_holder<existing_indices...>> {
  typedef typename make_indices_impl<
      index_to_add - 1,
      indices_holder<index_to_add - 1, existing_indices...>>::type type;
};

template <size_t... existing_indices>
struct make_indices_impl<0, indices_holder<existing_indices...>> {
  typedef indices_holder<existing_indices...> type;
};

template <size_t max_index>
typename make_indices_impl<max_index>::type make_indices() {
  return typename make_indices_impl<max_index>::type();
}

template <typename... T> struct sequences {
  std::tuple<cv::Mat_<T>...> m_sequences;

  template <size_t... Indices>
  std::tuple<T...> get_impl(size_t pos, indices_holder<Indices...>) {
    return std::make_tuple(std::get<Indices>(m_sequences)[pos]...);
  }

  std::tuple<T...> get(size_t pos) {
    return get_impl(pos, make_indices<sizeof...(T)>());
  }
};

/* compile time recursion
#include <tuple>
#include <utility>

template <std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> &, FuncT) // Unused arguments are given no names.
{

}

template <std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if <
    I<sizeof...(Tp), void>::type for_each(std::tuple<Tp...> &t, FuncT f) {
  f(std::get<I>(t));
  for_each<I + 1, FuncT, Tp...>(t, f);
}
*/

///
///
/*
template <size_t I, typename T, typename Tuple_t>
constexpr size_t index_in_tuple_fn() {
  static_assert(I < std::tuple_size<Tuple_t>::value,
                "The element is not in the tuple");

  typedef typename std::tuple_element<I, Tuple_t>::type el;
  if constexpr (std::is_same<T, el>::value) {
    return I;
  } else {
    return index_in_tuple_fn<I + 1, T, Tuple_t>();
  }
}

template <typename T, typename Tuple_t> struct index_in_tuple {
  static constexpr size_t value = index_in_tuple_fn<0, T, Tuple_t>();
};

template <typename... Args> auto count_if(Args... args) {

  auto a = index_in_tuple_fn<1, cv::MatIterator_<int>, std::tuple<Args...>>();
  std::cout << a << std::endl;
  return std::count_if(std::forward<Args>(args)...);
}
*/

/*
 *
Compile time for loop!
 */
template <std::size_t N> struct num { static const constexpr auto value = N; };

template <class F, std::size_t... Is>
void for_(F func, std::index_sequence<Is...>) {
  using expander = int[];
  (void)expander{0, ((void)func(num<Is>{}), 0)...};
}

template <std::size_t N, typename F> void for_(F func) {
  for_(func, std::make_index_sequence<N>());
}

// approach to see if it exists
template <class X, class Tuple> class Idx;

template <class X, class... T> class Idx<X, std::tuple<T...>> {
  template <std::size_t... idx>
  static constexpr ssize_t find_idx(std::index_sequence<idx...>) {
    return std::max({static_cast<ssize_t>(std::is_same_v<X, T> ? idx : -1)...});
  }

public:
  static constexpr ssize_t value = find_idx(std::index_sequence_for<T...>{});
};
template <typename X, class Tuple>
inline constexpr ssize_t Idx_v = Idx<X, Tuple>::value;

template <typename Tpl, typename T_arr, size_t lenArr>
constexpr auto cat_tuple_index(Tpl tpl, std::array<T_arr, lenArr> tplChooser) {}

template <typename Tpl> constexpr auto getArray(Tpl tpl) {

  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;
  constexpr std::array<int, tupleSize> arrayIndex{};

  // Here we for loop

  for_<tupleSize>([&](auto i) {
    using SelectedType = std::tuple_element_t<i.value, Tpl>;
    if constexpr (std::is_base_of<cv::MatConstIterator, SelectedType>::value) {
      arrayIndex.at(i.value) = i.value;
    }
  });

  return arrayIndex;
}

template <typename... Args> auto count_if(Args... args) {

  std::tuple<Args...> tpl = std::make_tuple(args...);
  bool isContinuous = true; // runtime

  constexpr size_t tupleSize = std::tuple_size<decltype(tpl)>::value;
  auto arrayIndex = getArray(tpl);

  // Here we for loop
  for_<tupleSize>([&](auto i) {
    using SelectedType = std::tuple_element_t<i.value, decltype(tpl)>;
    if constexpr (std::is_base_of<cv::MatConstIterator, SelectedType>::value) {
      isContinuous &= std::get<i.value>(tpl).m->isContinuous();
    }
  });

  // build a new tuple of types
  constexpr std::tuple<> empty;

  if (!isContinuous) {
    return std::count_if(std::forward<Args>(args)...);
  } else {
    return std::count_if(std::forward<Args>(args)...);
  }
}
