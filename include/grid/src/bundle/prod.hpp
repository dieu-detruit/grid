#pragma once

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/bundle/prod_iterator.hpp>
#include <grid/src/utility/iterator.hpp>
#include <grid/src/utility/parameter_pack.hpp>
#include <grid/src/utility/reference.hpp>
#include <grid/src/utility/tuple.hpp>

namespace Grid
{

template <class... types>
class Product
{
public:
    using iterator = product_iterator<Impl::copy_ref_info_t<types, Impl::get_iterator_t<types>>...>;
    //using iterator = product_iterator<Impl::get_iterator_t<types>...>;

public:
    std::tuple<types...> refs;

    template <std::size_t... I>
    inline iterator begin_impl(std::index_sequence<I...>)
    {
        return iterator{
            std::get<I>(refs).begin()...,
            std::get<I>(refs).begin()...,
            std::get<I>(refs).end()...};
    }
    template <std::size_t... I>
    inline iterator end_impl(std::index_sequence<I...>)
    {
        return iterator{
            std::get<I>(refs).end()...,
            std::get<I>(refs).begin()...,
            std::get<I>(refs).end()...};
    }

public:
    Product(types... refs) : refs{refs...} {}

    iterator begin()
    {
        return begin_impl(std::make_index_sequence<sizeof...(types)>{});
    }
    iterator end()
    {
        return end_impl(std::make_index_sequence<sizeof...(types)>{});
    }
};

// Argument as rvalue reference
//      -> types :  lvalue reference (arg: lvalue)
//                  value type       (arg: rvalue)
template <class... types>
auto prod(types&&... refs)
{
    return Product<types...>{refs...};
}


}  // namespace Grid
