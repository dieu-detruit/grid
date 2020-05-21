#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/utility/iterator.hpp>

namespace Grid
{

template <class... itr_types>
class zip_iterator
{
    using index_sequence = std::make_index_sequence<sizeof...(itr_types)>;
    using value_type_lref_tuple = std::tuple<std::add_lvalue_reference_t<typename std::iterator_traits<itr_types>::value_type>...>;
    using this_type = zip_iterator<itr_types...>;

    std::tuple<itr_types...> itrs;

public:
    zip_iterator(itr_types... itrs) : itrs{itrs...} {}

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
        return get_ref_impl(index_sequence{});
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
    template <std::size_t... I>
    decltype(auto) get_ref_impl(std::index_sequence<I...>)
    {
        return value_type_lref_tuple{*std::get<I>(itrs)...};
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

template <class... types>
struct Zip {

    using iterator = zip_iterator<Impl::get_iterator_t<types>...>;

    std::tuple<types&...> refs;

    template <std::size_t... I>
    inline iterator begin_impl(std::index_sequence<I...>)
    {
        return iterator{std::get<I>(refs).begin()...};
    }
    template <std::size_t... I>
    inline iterator end_impl(std::index_sequence<I...>)
    {
        return iterator{std::get<I>(refs).end()...};
    }

public:
    Zip(types&... refs) : refs{refs...} {}

    iterator begin()
    {
        return begin_impl(std::make_index_sequence<sizeof...(types)>{});
    }
    iterator end()
    {
        return end_impl(std::make_index_sequence<sizeof...(types)>{});
    }
};

template <class... types>
auto zip(types&... refs) -> Zip<types...>
{
    return Zip{refs...};
}

}  // namespace Grid
