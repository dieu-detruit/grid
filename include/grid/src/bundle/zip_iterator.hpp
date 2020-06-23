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
    using difference_type = long int;

    zip_iterator(std::remove_reference_t<itr_types>... itrs) : itrs{itrs...} {}

    // comparison
    bool operator==(const this_type& right) const
    {
        return comp_eq_impl(right, index_sequence{});
    }
    bool operator!=(const this_type& right) const
    {
        return not comp_eq_impl(right, index_sequence{});
    }
    bool operator<(const this_type& right) const
    {
        return std::get<0>(itrs) < std::get<0>(right.itrs);
    }
    bool operator>(const this_type& right) const
    {
        return std::get<0>(itrs) > std::get<0>(right.itrs);
    }
    bool operator<=(const this_type& right) const
    {
        return std::get<0>(itrs) <= std::get<0>(right.itrs);
    }
    bool operator>=(const this_type& right) const
    {
        return std::get<0>(itrs) >= std::get<0>(right.itrs);
    }

    // value access
    decltype(auto) operator*()
    {
        return ref_impl(expanded_index_sequence{});
    }

    // increment
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

    // decrement(TODO limit to bidirectional iterator)
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

    // random shift and access(TODO limit to random access iterator)
    this_type& operator+=(difference_type n)
    {
        add_substitution_impl(n, index_sequence{});
        return *this;
    }
    this_type& operator-=(difference_type n)
    {
        sub_substitution_impl(n, index_sequence{});
        return *this;
    }
    this_type operator+(difference_type n)
    {
        return add_impl(n, index_sequence{});
    }
    this_type operator-(difference_type n)
    {
        return sub_impl(n, index_sequence{});
    }
    difference_type operator-(const this_type& right) const
    {
        return std::get<0>(itrs) - std::get<0>(right.itrs);
    }
    auto operator[](difference_type n)
    {
        return random_access_impl(n, index_sequence{});
    }

private:
    template <std::size_t... I>
    bool comp_eq_impl(const this_type& right, std::index_sequence<I...>) const
    {
        return ((std::get<I>(itrs) == std::get<I>(right.itrs)) or ...);
    }

    // value access
    template <std::size_t n>
    inline auto element_ref(itr_tuple& itrs) -> std::tuple_element_t<n, value_type_tuple>
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
    inline auto ref_impl(std::index_sequence<I...>)
    {
        using namespace Impl;
        return value_type_tuple{element_ref<I>(itrs)...};
    }

    // increment
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

    // decrement
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

    // random shift
    template <std::size_t... I>
    void add_substitution_impl(difference_type n, std::index_sequence<I...>)
    {
        (void(std::get<I>(itrs) += static_cast<typename std::iterator_traits<decltype(std::get<I>(itrs))>::difference_type>(n)), ...);
    }
    template <std::size_t... I>
    void sub_substitution_impl(difference_type n, std::index_sequence<I...>)
    {
        (void(std::get<I>(itrs) -= static_cast<typename std::iterator_traits<decltype(std::get<I>(itrs))>::difference_type>(n)), ...);
    }
    template <std::size_t... I>
    this_type add_impl(difference_type n, std::index_sequence<I...>)
    {
        return this_type{(std::get<I>(itrs) + static_cast<typename std::iterator_traits<std::remove_reference_t<decltype(std::get<I>(itrs))>>::difference_type>(n))...};
    }
    template <std::size_t... I>
    this_type sub_impl(difference_type n, std::index_sequence<I...>)
    {
        return this_type{(std::get<I>(itrs) - static_cast<typename std::iterator_traits<decltype(std::get<I>(itrs))>::difference_type>(n))...};
    }

    // random access
    template <std::size_t I>
    inline auto element_random_access(itr_tuple& itrs, difference_type n) -> std::tuple_element_t<I, value_type_tuple>
    {
        constexpr bool is_bundle_iterator = value_tuple_element<I>::is_bundle_iterator;
        constexpr std::size_t global_index = value_tuple_element<I>::global_index;
        constexpr std::size_t local_index = value_tuple_element<I>::local_index;

        using element_difference_type = typename std::iterator_traits<std::remove_reference_t<decltype(std::get<global_index>(itrs))>>::difference_type;

        if constexpr (is_bundle_iterator) {
            return std::get<local_index>(std::get<global_index>(itrs)[static_cast<element_difference_type>(n)]);
        } else {
            return *std::get<global_index>(itrs);
        }
    };
    template <std::size_t... I>
    inline auto random_access_impl(difference_type n, std::index_sequence<I...>)
    {
        using namespace Impl;
        return value_type_tuple{element_random_access<I>(itrs, n)...};
    }
};


}  // namespace Grid

namespace std
{

// Specialization of std::iterator_traits
template <class... itr_types>
struct iterator_traits<Grid::zip_iterator<itr_types...>> {
    using value_type = typename Grid::zip_iterator<itr_types...>::value_type;
    using difference_type = typename Grid::zip_iterator<itr_types...>::difference_type;
    using iterator_category = std::random_access_iterator_tag;
};

}  // namespace std
