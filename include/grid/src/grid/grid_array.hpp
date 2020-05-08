#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/array.hpp>
#include <grid/src/grid/grid_base.hpp>
#include <grid/src/grid/range.hpp>
#include <grid/src/utility/dynamic_tuple.hpp>

namespace Grid
{

template <class dim_array_proxy_type, class range_tuple, std::size_t rank_rest>
class GridArrayBracketProxy
{
    dim_array_proxy_type proxy_fragment;
    range_tuple ranges;

public:
    GridArrayBracketProxy(dim_array_proxy_type proxy_fragment, const range_tuple& ranges)
        : proxy_fragment(proxy_fragment), ranges(ranges) {}

    GridArrayBracketProxy(GridArrayBracketProxy&& r)
        : proxy_fragment(r.proxy_fragment), ranges(r.ranges) {}

    auto operator[](typename dim_array_proxy_type::value_type subscript)
    {
        return GridArrayBracketProxy<decltype(proxy_fragment[0]), range_tuple, rank_rest - 1>{
            proxy_fragment[std::get<std::tuple_size<range_tuple>::value - rank_rest>(ranges).quantize(subscript)],
            ranges};
    }
};

template <class dim_array_proxy_type, class range_tuple>
class GridArrayBracketProxy<dim_array_proxy_type, range_tuple, 1>
{
    dim_array_proxy_type proxy_fragment;
    range_tuple ranges;

public:
    GridArrayBracketProxy(dim_array_proxy_type proxy_fragment, const range_tuple& ranges)
        : proxy_fragment(proxy_fragment), ranges(ranges) {}

    GridArrayBracketProxy(GridArrayBracketProxy&& r)
        : proxy_fragment(r.proxy_fragment), ranges(r.ranges) {}

    decltype(auto) operator[](typename dim_array_proxy_type::value_type subscript)
    {
        return proxy_fragment[std::get<std::tuple_size<range_tuple>::value - 1>(ranges).quantize(subscript)];
    }
};

template <class value_type, typename measure_type, std::size_t... N>
class GridArray : GridMapBase<value_type, measure_type, sizeof...(N)>
{
public:
    static constexpr std::size_t rank = sizeof...(N);
    static constexpr std::size_t whole_size = (... * N);

    using value_t = value_type;
    using measure_t = measure_type;

protected:
    using this_type = GridArray<value_type, measure_type, rank>;
    using data_type = Grid::array<value_type, N...>;

    std::tuple<Range<measure_type, N>...> ranges;
    data_type data;

public:
    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    iterator begin() { return this->data.begin(); }
    const_iterator begin() const { return this->data.begin(); }
    iterator end() { return this->data.end(); }
    const_iterator end() const { return this->data.end(); }

    GridArray(Range<measure_type, N>... ranges) : ranges({ranges...}) {}

    template <class... types>
    value_type& at(types... subscript)
    {
        static_assert(rank == sizeof...(subscript), "The number of argument is invalid.");
        static_assert(std::conjunction<std::is_convertible<types, measure_type>...>::value, "The arguments must be convertible to the measure type.");

        return at_impl(std::make_index_sequence<rank>{}, subscript...);
    }

    decltype(auto) operator[](measure_type subscript)
    {
        if constexpr (rank == 1) {
            return data[std::get<0>(ranges).quantize(subscript)];
        } else {
            return GridArrayBracketProxy<decltype(data[0]), decltype(ranges), rank - 1>{
                data[std::get<0>(ranges).quantize(subscript)], ranges};
        }
    }

    inline void fill(value_type value)
    {
        data.fill(value);
    }

protected:
    template <std::size_t... I, class... types>
    inline value_type& at_impl(std::index_sequence<I...>, types... subscript)
    {
        return data.at(std::get<I>(ranges).quantize(subscript)...);
    }
};

}  // namespace Grid
