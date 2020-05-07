#pragma once

#include <type_traits>

namespace Grid::Impl
{

// Check if all the template parameter classes are integral
template <class T, class... types>
struct is_all_integral {
    static constexpr bool value = std::is_integral_v<T> and is_all_integral<types...>::value;
};
template <class T>
struct is_all_integral<T> {
    static constexpr bool value = std::is_integral_v<T>;
};
template <class... types>
inline constexpr bool is_all_integral_v = is_all_integral<types...>::value;

// Get the type which has the maximum size in parameters
template <class T, class U, class... types>
struct get_max_size_type {
    using type = typename get_max_size_type<typename get_max_size_type<T, U>::type, types...>::type;
};
template <class T, class U>
struct get_max_size_type<T, U> {
    using type = typename std::conditional<(sizeof(T) > sizeof(U)), T, U>::type;
};

template <class... types>
using get_max_size_type_t = typename get_max_size_type<types...>::type;

// Get the Nth parameter
template <class T, std::size_t n, T... params>
struct get_nth_param {
};
template <class T, std::size_t n, T first, T... params>
struct get_nth_param<T, n, first, params...> {
    static constexpr T value = get_nth_param<T, n - 1, params...>::value;
};
template <class T, std::size_t n, T last>
struct get_nth_param<T, n, last> {
    static_assert(n == 0, "out of range");
    static constexpr T value = last;
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


}  // namespace Grid::Impl
