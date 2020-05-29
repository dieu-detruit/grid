#pragma once

#include <grid/src/utility/tag.hpp>

namespace Grid::Impl
{

// specialization with types in a sequence
template <template <class...> class tmpl, class... types>
auto sequencial_specialize_impl(type_sequence_tag<types...>)
{
    return type_tag<tmpl<types...>>{};
}
template <template <class...> class tmpl, class sequence_tag>
struct sequencial_specialize {
    using type = typename decltype(sequencial_specialize_impl<tmpl>(sequence_tag{}))::type;
};
template <template <class...> class tmpl, class sequence_tag>
using sequencial_specialize_t = typename sequencial_specialize<tmpl, sequence_tag>::type;

// concat type sequence
template <class... types1, class... types2>
auto concat_type_sequence_impl(type_sequence_tag<types1...>, type_sequence_tag<types2...>)
{
    return type_sequence_tag<types1..., types2...>{};
}
template <class sequence1, class sequence2>
struct concat_type_sequence {
    using type = decltype(concat_type_sequence_impl(sequence1{}, sequence2{}));
};
template <class sequence1, class sequence2>
using concat_type_sequence_t = typename concat_type_sequence<sequence1, sequence2>::type;

// is specialiazation with type sequence
template <class T, template <class...> class tmpl>
struct is_type_sequence_specialization
    : public std::false_type {
};
template <template <class...> class tmpl, class... types>
struct is_type_sequence_specialization<tmpl<types...>, tmpl>
    : public std::true_type {
};
template <class T, template <class...> class tmpl>
inline static constexpr bool is_type_sequence_specialization_v = is_type_sequence_specialization<T, tmpl>::value;

// concat specialization
template <template <class...> class tmpl, class... left_types, class... right_types>
constexpr auto get_concat_specialization_tag(type_tag<tmpl<left_types...>>, type_tag<tmpl<right_types...>>)
{
    return type_tag<tmpl<left_types..., right_types...>>{};
}

template <template <class...> class tmpl, class Spec1, class Spec2>
struct concat_specialization {
    using type = typename decltype(get_concat_specialization_tag<tmpl>(
        type_tag<Spec1>{},
        type_tag<Spec2>{}))::type;
};
template <template <class...> class tmpl, class Spec1, class Spec2>
using concat_specialization_t = typename concat_specialization<tmpl, Spec1, Spec2>::type;

// specialization with duplicated type
template <template <class...> class tmpl, class T, std::size_t N, class... pack>
struct homo_specialization {
    using type = typename homo_specialization<tmpl, T, N - 1, T, pack...>::type;
};
template <template <class...> class tmpl, class T, class... pack>
struct homo_specialization<tmpl, T, 0, pack...> {
    using type = tmpl<pack...>;
};

template <template <class...> class tmpl, class T, std::size_t N>
using homo_specialization_t = typename homo_specialization<tmpl, T, N>::type;

// flatten tuple
template <template <class...> class tmpl, class T>  // tuple type -> argument type itself
auto make_single_specialization_impl(type_tag<T>)
{
    return type_tag<std::conditional_t<is_type_sequence_specialization_v<T, tmpl>, T, tmpl<T>>>{};
}

template <template <class...> class tmpl, class T>
struct make_single_specialization {
    using type = typename decltype(make_single_specialization_impl<tmpl>(type_tag<T>{}))::type;
};
template <template <class...> class tmpl, class T>
using make_single_specialization_t = typename make_single_specialization<tmpl, T>::type;

template <template <class...> class tmpl, class head, class... types>
struct flatten_specialization {
    static constexpr std::size_t size = sizeof...(types) + 1;
    using type = concat_specialization_t<
        tmpl,
        make_single_specialization_t<tmpl, head>,
        typename flatten_specialization<tmpl, types...>::type>;
};
template <template <class...> class tmpl, class T>
struct flatten_specialization<tmpl, T> {
    static constexpr std::size_t size = 1;
    using type = make_single_specialization_t<tmpl, T>;
};
template <template <class...> class tmpl, class... types>
using flatten_specialization_t = typename flatten_specialization<tmpl, types...>::type;


// sequence with no duplicate element
template <class T, class... types>
auto is_in_types_impl(type_sequence_tag<types...>)
    -> std::conditional_t<(std::is_same_v<T, types> or ...),
        std::true_type, std::false_type>
{
}
template <class T, class... types>
struct is_in_types : decltype(is_in_types_impl<T>(type_sequence_tag<types...>{})) {
};
template <class T, class... types>
inline constexpr bool is_in_types_v = is_in_types<T, types...>::value;

template <class T, class sequence>
struct is_in_sequence : decltype(is_in_types_impl<T>(sequence{})) {
};
template <class T, class sequence>
inline constexpr bool is_in_sequence_v = is_in_sequence<T, sequence>::value;

template <class sequence, class... types>
struct unique_sequence_impl {
};
template <class sequence, class head, class... types>
struct unique_sequence_impl<sequence, head, types...> {
    using type = std::conditional_t<
        is_in_sequence_v<head, sequence>,
        typename unique_sequence_impl<sequence, types...>::type,
        typename unique_sequence_impl<concat_type_sequence_t<sequence, type_sequence_tag<head>>, types...>::type>;
};
template <class sequence>
struct unique_sequence_impl<sequence> {
    using type = sequence;
};

template <class... types>
struct unique_sequence : unique_sequence_impl<empty_tag, types...> {
};
template <class... types>
using unique_sequence_t = typename unique_sequence<types...>::type;

// specialization with no duplicate type sequence
template <template <class...> class tmpl, class head, class... types>
struct unique_specialization_impl {
    using type = sequencial_specialize_t<tmpl, unique_sequence_t<types...>>;
};

template <template <class...> class tmpl, class... types>
auto unique_specialization_redirect(type_tag<tmpl<types...>>)
    -> typename unique_specialization_impl<tmpl, types...>::type
{
}

template <template <class...> class tmpl, class Spec>
struct unique_specialization {
    using type = decltype(unique_specialization_redirect<tmpl>(type_tag<Spec>{}));
};

template <template <class...> class tmpl, class Spec>
using unique_specialization_t = typename unique_specialization<tmpl, Spec>::type;


}  // namespace Grid::Impl
