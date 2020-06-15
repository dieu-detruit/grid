#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/utility/type_sequence.hpp>

namespace Grid::Impl
{

// is tuple
template <class T>
struct is_tuple : public is_type_sequence_specialization<T, std::tuple> {
};
template <class T>
inline static constexpr bool is_tuple_v = is_tuple<T>::value;

// concat tuple
template <class Tuple1, class Tuple2>
struct concat_tuple : public concat_specialization<std::tuple, Tuple1, Tuple2> {
};
template <class Tuple1, class Tuple2>
using concat_tuple_t = typename concat_tuple<Tuple1, Tuple2>::type;

// dynamic_tuple
template <class T, std::size_t N>
struct dynamic_tuple : public homo_specialization<std::tuple, T, N> {
};
template <class T, std::size_t N>
using dynamic_tuple_t = typename dynamic_tuple<T, N>::type;

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

// flatten tuple
template <class T>
struct make_flatten_tuple : public make_single_specialization<std::tuple, T> {
};
template <class T>
using make_flatten_tuple_t = typename make_flatten_tuple<T>::type;

template <class... types>
struct flatten_tuple : public flatten_specialization<std::tuple, types...> {
};
template <class... types>
using flatten_tuple_t = typename flatten_tuple<types...>::type;

// extended tuple size
template <class T>
struct extended_tuple_size {
    static constexpr std::size_t value = 1;
};
template <class... types>
struct extended_tuple_size<std::tuple<types...>> {
    static constexpr std::size_t value = sizeof...(types);
};
template <class T>
inline constexpr std::size_t extended_tuple_size_v = extended_tuple_size<T>::value;

// traits for flatten tuple
template <std::size_t tmp_global_index, std::size_t n, class... types>
struct flatten_tuple_element_impl {
};

template <std::size_t tmp_global_index, std::size_t n, class first, class... types>
struct flatten_tuple_element_impl<tmp_global_index, n, first, types...> {
private:
    static constexpr std::size_t first_type_size = extended_tuple_size_v<first>;
    static constexpr bool is_terminal = n < first_type_size;
    using recurse_type = flatten_tuple_element_impl<tmp_global_index + 1, n - first_type_size, types...>;

public:
    static constexpr bool is_tuple
        = is_terminal ? is_tuple_v<first>
                      : recurse_type::is_tuple;
    static constexpr std::size_t global_index
        = is_terminal ? tmp_global_index
                      : recurse_type::global_index;
    static constexpr std::size_t local_index
        = is_terminal ? n
                      : recurse_type::local_index;
};
template <std::size_t tmp_global_index, std::size_t n, class type>
struct flatten_tuple_element_impl<tmp_global_index, n, type> {

    static constexpr bool is_tuple = is_tuple_v<type>;
    static constexpr std::size_t global_index = tmp_global_index;
    static constexpr std::size_t local_index = n;
};

template <std::size_t n, class... types>
struct flatten_tuple_element {
private:
    using impl_type = flatten_tuple_element_impl<0, n, types...>;

public:
    static constexpr bool is_tuple = impl_type::is_tuple;
    static constexpr std::size_t global_index = impl_type::global_index;
    static constexpr std::size_t local_index = impl_type::local_index;
};

// type sequence to tuple
template <class sequence_tag>
struct sequence_to_tuple : public sequencial_specialize<std::tuple, sequence_tag> {
};
template <class sequence_tag>
using sequence_to_tuple_t = typename sequence_to_tuple<sequence_tag>::type;

// tuple which has no coupled type element
template <class Tuple>
struct unique_tuple : public unique_specialization<std::tuple, Tuple> {
};
template <class Tuple>
using unique_tuple_t = typename unique_tuple<Tuple>::type;

}  // namespace Grid::Impl
