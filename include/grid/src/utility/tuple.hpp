#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/utility/tag.hpp>

namespace Grid::Impl
{

// dynamic_tuple
template <class T, std::size_t N, class... pack>
struct dynamic_tuple_impl {
    using type = typename dynamic_tuple_impl<T, N - 1, T, pack...>::type;
};
template <class T, class... pack>
struct dynamic_tuple_impl<T, 0, pack...> {
    using type = std::tuple<pack...>;
};

template <class T, std::size_t N>
using dynamic_tuple = typename dynamic_tuple_impl<T, N>::type;

// convert tuple to array
template <class Tuple, std::size_t... I>
inline auto tuple_to_array_impl(Tuple&& tp, std::index_sequence<I...>)
{
    using element_type = std::tuple_element_t<0, std::decay_t<Tuple>>;
    return std::array<element_type, sizeof...(I)>{std::get<I>(tp)...};
}
template <class Tuple>
auto tuple_to_array(Tuple&& tp)
{
    return tuple_to_array_impl(tp, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

// get an array of reference to each value of a tuple
template <class Tuple, std::size_t... I>
inline auto tuple_to_ref_array_impl(Tuple& tp, std::index_sequence<I...>)
{
    using element_type
        = std::add_lvalue_reference_t<
            std::tuple_element_t<0, std::decay_t<Tuple>>>;
    return std::array<element_type, sizeof...(I)>{std::ref(std::get<I>(tp))...};
}
template <class Tuple>
auto tuple_to_ref_array(Tuple& tp)
{
    return tuple_to_array_impl(tp, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

// cast all elements to a type
template <class T, class Tuple, std::size_t... I>
inline auto tuple_cast_impl(type_tag<T>, Tuple&& tp, std::index_sequence<I...>)
{
    return dynamic_tuple<T, sizeof...(I)>{static_cast<T>(std::get<I>(tp))...};
}
template <class T>
struct tuple_cast {
    template <class Tuple>
    auto operator()(Tuple&& tp)
    {
        return tuple_cast_impl(type_tag<T>{}, tp, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
    }
};

// get product of tuple elements
template <class Tuple, std::size_t... I>
inline auto tuple_product_impl(Tuple&& tp, std::index_sequence<I...>)
{
    return (... * std::get<I>(tp));
}
template <class Tuple>
inline auto tuple_product(Tuple&& tp)
{
    return tuple_product_impl(tp, std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

}  // namespace Grid::Impl
