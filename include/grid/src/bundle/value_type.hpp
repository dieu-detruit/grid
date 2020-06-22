#pragma once

#include <iterator>
#include <type_traits>

#include <grid/src/utility/reference.hpp>
#include <grid/src/utility/type_sequence.hpp>

namespace Grid
{

template <class... itr_types>
class zip_iterator;

template <class... itr_types>
class product_iterator;

namespace Bundle
{

// is_bundle_iterator
template <class Iterator>
struct is_bundle_iterator
    : public std::false_type {
};
template <class... itr_types>
struct is_bundle_iterator<Grid::zip_iterator<itr_types...>>
    : public std::true_type {
};
template <class... itr_types>
struct is_bundle_iterator<Grid::product_iterator<itr_types...>>
    : public std::true_type {
};
template <class... itr_types>
struct is_bundle_iterator<Grid::zip_iterator<itr_types...>&>
    : public std::true_type {
};
template <class... itr_types>
struct is_bundle_iterator<Grid::product_iterator<itr_types...>&>
    : public std::true_type {
};
template <class Iterator>
inline constexpr bool is_bundle_iterator_v = is_bundle_iterator<Iterator>::value;

// value_type_tuple
template <class Iterator>
struct unpack_bundle_to_value_sequence {
    using type = Impl::type_sequence_tag<Impl::copy_ref_info_t<Iterator, typename std::iterator_traits<std::remove_reference_t<Iterator>>::value_type>>;
};
template <class... itr_types>
struct unpack_bundle_to_value_sequence<Grid::zip_iterator<itr_types...>> {
    using type = Impl::type_sequence_tag<Impl::copy_ref_info_t<itr_types, typename std::iterator_traits<std::remove_reference_t<itr_types>>::value_type>...>;
};
template <class... itr_types>
struct unpack_bundle_to_value_sequence<Grid::product_iterator<itr_types...>> {
    using type = Impl::type_sequence_tag<Impl::copy_ref_info_t<itr_types, typename std::iterator_traits<std::remove_reference_t<itr_types>>::value_type>...>;
};
template <class... itr_types>
struct unpack_bundle_to_value_sequence<Grid::zip_iterator<itr_types...>&> {
    using type = Impl::type_sequence_tag<Impl::copy_ref_info_t<itr_types, typename std::iterator_traits<std::remove_reference_t<itr_types>>::value_type>...>;
};
template <class... itr_types>
struct unpack_bundle_to_value_sequence<Grid::product_iterator<itr_types...>&> {
    using type = Impl::type_sequence_tag<Impl::copy_ref_info_t<itr_types, typename std::iterator_traits<std::remove_reference_t<itr_types>>::value_type>...>;
};

template <class... itr_types, class itr_type_to_append>
auto concat_unpacked_value_type(Impl::type_sequence_tag<itr_types...>, Impl::type_tag<itr_type_to_append>)
    -> Impl::concat_type_sequence_t<Impl::type_sequence_tag<itr_types...>, typename unpack_bundle_to_value_sequence<itr_type_to_append>::type>
{
    return {};
}

template <class sequence, class... itr_types>
struct value_type_tuple {
};
template <class sequence, class first, class... itr_types>
struct value_type_tuple<sequence, first, itr_types...> {
    using type = value_type_tuple<decltype(concat_unpacked_value_type(sequence{}, Impl::type_sequence_tag<first>{})), itr_types...>::type;
};
template <class sequence>
struct value_type_tuple<sequence> {
    using type = Impl::sequencial_specialize_t<std::tuple, sequence>;
};

template <class... itr_types>
using value_type_tuple_t = value_type_tuple<Impl::type_sequence_tag<>, itr_types...>::type;

// value type size
template <class Iterator>
struct value_type_size {
    static constexpr std::size_t value = 1;
};
template <class... itr_types>
struct value_type_size<zip_iterator<itr_types...>> {
    static constexpr std::size_t value = std::tuple_size_v<typename std::iterator_traits<zip_iterator<itr_types...>>::value_type>;
};
template <class... itr_types>
struct value_type_size<product_iterator<itr_types...>> {
    static constexpr std::size_t value = std::tuple_size_v<typename std::iterator_traits<product_iterator<itr_types...>>::value_type>;
};
template <class... itr_types>
struct value_type_size<zip_iterator<itr_types...>&> {
    static constexpr std::size_t value = std::tuple_size_v<typename std::iterator_traits<zip_iterator<itr_types...>>::value_type>;
};
template <class... itr_types>
struct value_type_size<product_iterator<itr_types...>&> {
    static constexpr std::size_t value = std::tuple_size_v<typename std::iterator_traits<product_iterator<itr_types...>>::value_type>;
};

// traits for each element
template <std::size_t tmp_global_index, std::size_t n, class... itr_types>
struct value_type_tuple_element_impl {
};

template <std::size_t tmp_global_index, std::size_t n, class first, class... itr_types>
struct value_type_tuple_element_impl<tmp_global_index, n, first, itr_types...> {
private:
    static constexpr std::size_t first_type_size = value_type_size<first>::value;
    static constexpr bool is_terminal = n < first_type_size;
    using recurse_type = value_type_tuple_element_impl<tmp_global_index + 1, n - first_type_size, itr_types...>;

    //fuga<n, first, is_terminal, is_bundle_iterator_v<first>> _;

public:
    static constexpr bool is_bundle_iterator
        = is_terminal ? is_bundle_iterator_v<first>
                      : recurse_type::is_bundle_iterator;
    static constexpr std::size_t global_index
        = is_terminal ? tmp_global_index
                      : recurse_type::global_index;
    static constexpr std::size_t local_index
        = is_terminal ? n
                      : recurse_type::local_index;
};

template <std::size_t tmp_global_index, std::size_t n, class itr_type>
struct value_type_tuple_element_impl<tmp_global_index, n, itr_type> {
    static constexpr bool is_bundle_iterator = is_bundle_iterator_v<itr_type>;
    static constexpr std::size_t global_index = tmp_global_index;
    static constexpr std::size_t local_index = n;
};


template <std::size_t n, class... itr_types>
struct value_type_tuple_element {
private:
    using impl_type = value_type_tuple_element_impl<0, n, itr_types...>;

public:
    static constexpr bool is_bundle_iterator = impl_type::is_bundle_iterator;
    static constexpr std::size_t global_index = impl_type::global_index;
    static constexpr std::size_t local_index = impl_type::local_index;
};


}  // namespace Bundle
}  // namespace Grid
