#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <grid/src/grid/grid_base.hpp>
#include <grid/src/grid_vector/range.hpp>
#include <grid/src/utility/tuple.hpp>
#include <grid/vector.hpp>

namespace Grid
{

template <class dim_vector_proxy_type, class range_tuple, std::size_t rank_rest>
class GridVectorBracketProxy
{
    dim_vector_proxy_type proxy;
    const range_tuple& ranges;

public:
    GridVectorBracketProxy(dim_vector_proxy_type proxy, const range_tuple& ranges)
        : proxy(proxy), ranges(ranges) {}

    decltype(auto) operator[](typename dim_vector_proxy_type::value_type subscript)
    {
        if constexpr (rank_rest == 1) {
            return proxy[std::get<std::tuple_size<range_tuple>::value - 1>(ranges).quantize(subscript)];
        } else {
            return GridVectorBracketProxy<decltype(proxy[0]), range_tuple, rank_rest - 1>{
                proxy[std::get<std::tuple_size<range_tuple>::value - rank_rest>(ranges).quantize(subscript)],
                ranges};
        }
    }
};

template <class value_type, typename measure_type, std::size_t rank>
class GridVector
    : public GridBase<value_type,
          measure_type,
          rank,
          Grid::vector<value_type, rank>>
{
protected:
    using this_type = GridVector<value_type, measure_type, rank>;
    using base_type = typename this_type::base_type;

    Impl::dynamic_tuple<DynamicRange<measure_type>, rank> ranges;

public:
    GridVector(Impl::dynamic_tuple<DynamicRange<measure_type>, rank> ranges)
        : ranges(ranges), base_type(Impl::tuple_cast<std::size_t>{}(ranges)) {}

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
            return this->data[std::get<0>(ranges).quantize(subscript)];
        } else {
            return GridVectorBracketProxy<decltype(this->data[0]), decltype(ranges), rank - 1>{
                this->data[std::get<0>(ranges).quantize(subscript)], ranges};
        }
    }

protected:
    template <std::size_t... I, class... types>
    inline value_type& at_impl(std::index_sequence<I...>, types... subscript)
    {
        return this->data.at(std::get<I>(ranges).quantize(subscript)...);
    }
};

}  // namespace Grid
