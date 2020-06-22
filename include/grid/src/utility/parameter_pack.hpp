#pragma once

#include <strings.h>
#include <type_traits>
#include <utility>

namespace Grid::Impl
{

// Check if all the template parameter classes are integral
template <class... types>
struct is_all_integral {
    static constexpr bool value = std::conjunction<std::is_integral<types>...>::value;
};
template <class... types>
inline constexpr bool is_all_integral_v = is_all_integral<types...>::value;

// Get the type which has the maximum size in parameters
template <class T, class U, class... types>
struct get_max_size_type {
    using type = get_max_size_type<typename get_max_size_type<T, U>::type, types...>::type;
};
template <class T, class U>
struct get_max_size_type<T, U> {
    using type = std::conditional<(sizeof(T) > sizeof(U)), T, U>::type;
};

template <class... types>
using get_max_size_type_t = get_max_size_type<types...>::type;

// Get the Nth parameter
template <class T, std::size_t n, T first, T... params>
struct get_nth_param {
    static_assert(n < 1 + sizeof...(params), "out of range");
    static constexpr T value = get_nth_param<T, n - 1, params...>::value;
};

template <class T, T first, T... params>
struct get_nth_param<T, 0, first, params...> {
    static constexpr T value = first;
};

template <class T, std::size_t n, T... params>
static constexpr T get_nth_param_v = get_nth_param<T, n, params...>::value;

// Get the last parameter
template <class T, T... params>
struct get_last_param {
    static constexpr T value = get_nth_param<T, sizeof...(params), params...>::value;
};
template <class T, T... params>
constexpr T get_last_param_v = get_nth_param<T, params...>::value;

// Get the Nth index
template <std::size_t n, std::size_t... I>
auto get_nth_index_impl(std::index_sequence<I...>)
{
    return get_nth_param<std::size_t, n, I...>{};
}

template <std::size_t n, class sequence>
static constexpr std::size_t get_nth_index_v = decltype(get_nth_index_impl(sequence{}))::value;

// Get the Nth type
template <std::size_t n, class... types>
struct get_nth_type {
    static_assert(n < 1 + sizeof...(types), "out of range");
    using type = std::tuple_element_t<n, std::tuple<types...>>;
};
template <std::size_t n, class... types>
using get_nth_type_t = get_nth_type<n, types...>::type;

// Index sequence stack
template <std::size_t... I>
struct index_sequence_pile {
    template <std::size_t n>
    using push = index_sequence_pile<n, I...>;

    using type = std::index_sequence<I...>;
};

// Get reverse index sequence
template <class sequence_pile, std::size_t... I>
struct reverse_index_sequence_impl {
};
template <class sequence_pile, std::size_t first, std::size_t... I>
struct reverse_index_sequence_impl<sequence_pile, first, I...> {
    using type = reverse_index_sequence_impl<typename sequence_pile::template push<first>, I...>::type;
};
template <class sequence_pile>
struct reverse_index_sequence_impl<sequence_pile> {
    using type = sequence_pile::type;
};

template <std::size_t first, std::size_t... I>
auto reverse_index_sequence_redirect(std::index_sequence<first, I...>)
{
    if constexpr (sizeof...(I) == 0) {
        return std::index_sequence<first>{};
    } else {
        return typename reverse_index_sequence_impl<index_sequence_pile<first>, I...>::type{};
    }
}

template <std::size_t... I>
using reversed_index_sequence = decltype(reverse_index_sequence_redirect(std::index_sequence<I...>{}));

template <std::size_t... I>
auto make_reversed_index_sequence_impl(std::index_sequence<I...>)
    -> reversed_index_sequence<I...>
{
}

template <std::size_t N>
using make_reversed_index_sequence = decltype(
    make_reversed_index_sequence_impl(std::make_index_sequence<N>{}));

}  // namespace Grid::Impl
