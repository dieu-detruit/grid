#pragma once

#include <array>
#include <tuple>
#include <utility>

namespace Grid::Impl
{

// dynamic_tuple_impl
template <class T, std::size_t N>
struct dynamic_tuple_impl {
    using type = decltype(std::tuple_cat(typename dynamic_tuple_impl<T, N - 1>::type{}, std::make_tuple(T{})));
};
template <class T>
struct dynamic_tuple_impl<T, 1> {
    using type = std::tuple<T>;
};

template <class T, std::size_t N>
using dynamic_tuple = typename dynamic_tuple_impl<T, N>::type;


// convert tuple to an array
template <class Tuple, std::size_t... I>
inline auto tuple_to_array_impl(Tuple&& tp, std::index_sequence<I...>)
{
    return std::array<std::tuple_element_t<0, std::decay_t<Tuple>>, sizeof...(I)>{std::get<I>(tp)...};
}
template <class Tuple>
auto tuple_to_array(Tuple&& tp)
{
    return tuple_to_array_impl(tp, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

}  // namespace Grid::Impl
