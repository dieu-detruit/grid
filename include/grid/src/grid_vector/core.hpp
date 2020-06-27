#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <grid/linear.hpp>
#include <grid/prod.hpp>
#include <grid/src/generic/grid_base.hpp>
#include <grid/src/generic/grid_proxy.hpp>
#include <grid/src/grid_vector/range.hpp>
#include <grid/src/utility/tuple.hpp>
#include <grid/vector.hpp>

namespace Grid
{

template <class value_type, typename measure_type, std::size_t rank, class... range_types>
class GridVectorImpl
    : public GridBase<value_type,
          measure_type,
          rank,
          Grid::vector<value_type, rank>>
{
protected:
    using this_type = GridVectorImpl<value_type, measure_type, rank, range_types...>;
    using base_type = typename this_type::base_type;

    std::array<DynamicRange<measure_type>, rank> ranges;

public:
    GridVectorImpl(range_types... ranges)
        : ranges{{ranges...}}, base_type(static_cast<std::size_t>(ranges)...) {}


    template <class... T>
    void resize(DynamicRange<T>... new_ranges)
    {
        static_assert(std::conjunction_v<std::is_same<T, measure_type>...>, "");
        this->_data.resize(static_cast<std::size_t>(new_ranges)...);
        ranges = std::array<DynamicRange<measure_type>, rank>{{new_ranges...}};
    }

    auto range(std::size_t n) const
    {
        return ranges.at(n);
    }
    auto line(std::size_t n)
    {
        return ranges.at(n).line();
    }
    auto lines()
    {
        return lines_impl(std::make_index_sequence<rank>{});
    }

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
            return this->_data[std::get<0>(ranges).quantize(subscript)];
        } else {
            return GridProxy<this_type, decltype(this->_data[0]), decltype(ranges), rank - 1>{
                this->_data[std::get<0>(ranges).quantize(subscript)], ranges};
        }
    }

protected:
    template <std::size_t... I, class... types>
    inline value_type& at_impl(std::index_sequence<I...>, types... subscript)
    {
        return this->_data.at(std::get<I>(ranges).quantize(subscript)...);
    }

    template <std::size_t... I>
    inline auto lines_impl(std::index_sequence<I...>)
    {
        return Grid::prod(ranges[I].line()...);
    }
};

namespace Impl
{
template <class value_type, typename measure_type, std::size_t rank, class... types>
auto grid_vector_wrapper_impl(type_tag<std::tuple<types...>>)
{
    return type_tag<GridVectorImpl<value_type, measure_type, rank, types...>>{};
}

template <class value_type, typename measure_type, std::size_t rank>
struct grid_vector_wrapper {
    using type = typename decltype(
        grid_vector_wrapper_impl<value_type, measure_type, rank>(
            type_tag<dynamic_tuple_t<DynamicRange<measure_type>, rank>>{}))::type;
};
}  // namespace Impl

template <class value_type, typename measure_type, std::size_t rank>
using GridVector = typename Impl::grid_vector_wrapper<value_type, measure_type, rank>::type;

namespace Impl
{
template <class T>
struct is_grid_vector : public std::false_type {
};
template <class value_type, class measure_type, std::size_t rank, class... range_types>
struct is_grid_vector<GridVectorImpl<value_type, measure_type, rank, range_types...>> : public std::true_type {
};
template <class T>
inline constexpr bool is_grid_vector_v = is_grid_vector<T>::value;

template <class T>
struct get_rank {
    static constexpr std::size_t value = 0;
};
template <class value_type, class measure_type, std::size_t rank, class... range_types>
struct get_rank<GridVectorImpl<value_type, measure_type, rank, range_types...>> {
    static constexpr std::size_t value = rank;
};
template <class T>
inline constexpr std::size_t get_rank_v = get_rank<T>::value;

}  // namespace Impl

}  // namespace Grid
