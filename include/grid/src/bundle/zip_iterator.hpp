#pragma once

#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/bundle/value_type.hpp>
#include <grid/src/utility/iterator.hpp>
#include <grid/src/utility/reference.hpp>

namespace Grid
{

template <class... itr_types>
class zip_iterator
{

    using value_type_tuple = Bundle::value_type_tuple_t<itr_types...>;

    template <std::size_t n>
    using value_tuple_element = Bundle::value_type_tuple_element<n, itr_types...>;

    static constexpr std::size_t zip_size = sizeof...(itr_types);
    static constexpr std::size_t zip_expanded_size = std::tuple_size_v<value_type_tuple>;

    using itr_tuple = std::tuple<std::remove_reference_t<itr_types>...>;
    using this_type = zip_iterator<itr_types...>;

    using index_sequence = std::make_index_sequence<zip_size>;
    using expanded_index_sequence = std::make_index_sequence<zip_expanded_size>;

    itr_tuple itrs;

public:
    using value_type = value_type_tuple;

    zip_iterator(std::remove_reference_t<itr_types>... itrs) : itrs{itrs...} {}

    bool operator==(this_type right)
    {
        return comp_eq_impl(index_sequence{}, right);
    }
    bool operator!=(this_type right)
    {
        return not comp_eq_impl(index_sequence{}, right);
    }
    decltype(auto) operator*()
    {
        return get_ref_impl(expanded_index_sequence{});
    }
    this_type& operator++()
    {
        pre_increment_impl(index_sequence{});
        return *this;
    }
    this_type& operator++(int)
    {
        post_increment_impl(index_sequence{});
        return *this;
    }
    this_type& operator--()
    {
        pre_decrement_impl(index_sequence{});
        return *this;
    }
    this_type& operator--(int)
    {
        post_decrement_impl(index_sequence{});
        return *this;
    }


private:
    template <std::size_t... I>
    bool comp_eq_impl(std::index_sequence<I...>, this_type right)
    {
        return ((std::get<I>(itrs) == std::get<I>(right.itrs)) or ...);
    }
    template <std::size_t n>
    decltype(auto) get_value(itr_tuple& itrs)
    {
        constexpr bool is_bundle_iterator = value_tuple_element<n>::is_bundle_iterator;
        constexpr std::size_t global_index = value_tuple_element<n>::global_index;
        constexpr std::size_t local_index = value_tuple_element<n>::local_index;

        if constexpr (is_bundle_iterator) {
            return std::get<local_index>(*std::get<global_index>(itrs));
        } else {
            return *std::get<global_index>(itrs);
        }
    };

    template <std::size_t... I>
    inline decltype(auto) get_ref_impl(std::index_sequence<I...>)
    {
        using namespace Impl;
        return value_type_tuple{get_value<I>(itrs)...};
    }
    template <std::size_t... I>
    void pre_increment_impl(std::index_sequence<I...>)
    {
        (void(++std::get<I>(itrs)), ...);
    }
    template <std::size_t... I>
    void post_increment_impl(std::index_sequence<I...>)
    {
        (void(std::get<I>(itrs)++), ...);
    }
    template <std::size_t... I>
    void pre_decrement_impl(std::index_sequence<I...>)
    {
        (void(--std::get<I>(itrs)), ...);
    }
    template <std::size_t... I>
    void post_decrement_impl(std::index_sequence<I...>)
    {
        (void(std::get<I>(itrs)--), ...);
    }
};

}  // namespace Grid

namespace std
{

// Specialization of std::iterator_traits
template <class... itr_types>
struct iterator_traits<Grid::zip_iterator<itr_types...>> {
    using value_type = typename Grid::zip_iterator<itr_types...>::value_type;
    using iterator_category = typename std::bidirectional_iterator_tag;
};

}  // namespace std
