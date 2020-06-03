#pragma once

#include <__tuple>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/bundle/value_type.hpp>
#include <grid/src/utility/iterator.hpp>
#include <grid/src/utility/parameter_pack.hpp>
#include <grid/src/utility/reference.hpp>

namespace Grid
{

template <class... itr_types>
class product_iterator
{
    using value_type_tuple = Bundle::value_type_tuple_t<itr_types...>;

    template <std::size_t n>
    using value_tuple_element = Bundle::value_type_tuple_element<n, itr_types...>;

    static constexpr std::size_t prod_size = sizeof...(itr_types);
    static constexpr std::size_t prod_expanded_size = std::tuple_size_v<value_type_tuple>;

    using itr_tuple = std::tuple<std::remove_reference_t<itr_types>...>;
    using this_type = product_iterator<itr_types...>;

    using index_sequence = std::make_index_sequence<prod_size>;
    using expanded_index_sequence = std::make_index_sequence<prod_expanded_size>;
    using reversed_index_sequence = Impl::make_reversed_index_sequence<prod_size>;

    itr_tuple itrs;
    itr_tuple begins;
    itr_tuple ends;

public:
    using value_type = value_type_tuple;

    product_iterator(
        std::remove_reference_t<itr_types>... itrs,
        std::remove_reference_t<itr_types>... begins,
        std::remove_reference_t<itr_types>... ends)
        : itrs{itrs...}, begins{begins...}, ends{ends...} {}

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
        increment_impl(reversed_index_sequence{});
        return *this;
    }
    this_type& operator++(int)
    {
        increment_impl(reversed_index_sequence{});
        return *this;
    }
    this_type& operator--()
    {
        decrement_impl(reversed_index_sequence{});
        return *this;
    }
    this_type& operator--(int)
    {
        decrement_impl(reversed_index_sequence{});
        return *this;
    }


private:
    template <std::size_t... I>
    inline bool comp_eq_impl(std::index_sequence<I...>, this_type right)
    {
        return ((std::get<I>(itrs) == std::get<I>(right.itrs)) and ...);
    }

    template <std::size_t n>
    auto get_value(itr_tuple& itrs) -> std::tuple_element_t<n, value_type_tuple>
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
    inline this_type& increment_impl(std::index_sequence<I...>)
    {
        (void)((++std::get<I>(itrs), ...));
        if (((std::get<I>(itrs) == std::get<I>(ends)) and ...)) {
            return *this;
        }
        (void)((--std::get<I>(itrs), ...));

        static const std::array<std::function<bool(itr_tuple&, itr_tuple&, itr_tuple&)>, prod_size>
            incrementer{
                {[](itr_tuple& itrs, itr_tuple& begins, itr_tuple& ends) {
                    ++std::get<I>(itrs);
                    if constexpr (I == 0) {
                        return true;
                    }
                    if (std::get<I>(itrs) == std::get<I>(ends)) {
                        std::get<I>(itrs) = std::get<I>(begins);  // 繰り上がり
                        return true;
                    } else {
                        return false;
                    }
                }...}};

        for (auto& f : incrementer) {
            if (not f(itrs, begins, ends)) {
                break;
            }
        }
        return *this;
    }

    template <std::size_t... I>
    inline this_type& decrement_impl(std::index_sequence<I...>)
    {
        if (((std::get<I>(itrs) == std::get<I>(begins)) and ...)) {
            return *this;
        }
        static const std::array<std::function<bool(itr_tuple&, const itr_tuple&, const itr_tuple)>, prod_size> incrementer{
            {[](itr_tuple& itrs, const itr_tuple& begins, const itr_tuple& ends) {
                if (std::get<I>(itrs) == std::get<I>(begins)) {
                    std::get<I>(itrs) = std::get<I>(ends) - 1;  // 繰り下がり
                    return true;
                } else {
                    --std::get<I>(itrs);
                    return false;
                }
            }...}};

        for (auto& f : incrementer) {
            if (not f(itrs, begins, ends)) {
                break;
            }
        }
        return *this;
    }
};
}  // namespace Grid

namespace std
{

// Specialization of std::iterator_traits
template <class... itr_types>
struct iterator_traits<Grid::product_iterator<itr_types...>> {
    using value_type = typename Grid::product_iterator<itr_types...>::value_type;
    using iterator_category = typename std::bidirectional_iterator_tag;
};

}  // namespace std
