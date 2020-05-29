#pragma once

#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/bundle/zip_iterator.hpp>
#include <grid/src/utility/iterator.hpp>
#include <grid/src/utility/reference.hpp>

namespace Grid
{

template <class... types>
class Zip
{
public:
    using iterator = zip_iterator<Impl::copy_ref_info_t<types, Impl::get_iterator_t<types>>...>;

private:
    std::tuple<types...> refs;  // ref or copy

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
    Zip(types... refs) : refs{refs...} {}

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
auto zip(types&&... refs)
{
    return Zip<types...>{refs...};
}

}  // namespace Grid
