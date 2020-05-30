#pragma once

#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/utility/iterator.hpp>
#include <grid/src/utility/reference.hpp>

namespace Grid
{

template <class... itr_types>
class zip_iterator
{
    using index_sequence = std::make_index_sequence<sizeof...(itr_types)>;
    using this_type = zip_iterator<itr_types...>;

    std::tuple<std::remove_reference_t<itr_types>...> itrs;

public:
    using value_type = std::tuple<Impl::copy_ref_info_t<itr_types, Impl::iterator_value_type_t<std::remove_reference_t<itr_types>>>...>;

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
    auto get_ref_impl(std::index_sequence<I...>)
    {
        return value_type{*std::get<I>(itrs)...};
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
