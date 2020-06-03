#pragma once

#include <array>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/array.hpp>
#include <grid/linear.hpp>
#include <grid/prod.hpp>
#include <grid/src/generic/grid_base.hpp>
#include <grid/src/generic/grid_proxy.hpp>
#include <grid/src/grid_array/range.hpp>
#include <grid/src/utility/tuple.hpp>

namespace Grid
{

template <class value_type, typename measure_type, std::size_t... N>
class GridArray
    : public GridBase<value_type,
          measure_type,
          sizeof...(N),
          Grid::array<value_type, N...>>
{
public:
    static constexpr std::size_t rank = sizeof...(N);
    static constexpr std::size_t whole_size = (... * N);

protected:
    using this_type = GridArray<value_type, measure_type, rank>;

    std::tuple<StaticRange<measure_type, N>...> ranges;

public:
    GridArray(StaticRange<measure_type, N>... ranges) : ranges({ranges...}) {}

    template <std::size_t n>
    decltype(auto) range()
    {
        return std::ref(std::get<n>(ranges));
    }

    template <std::size_t n>
    inline auto line()
    {
        return std::get<n>(ranges).line();
    }

    inline auto lines()
    {
        return lines_impl(std::make_index_sequence<rank>{});
    }

    template <class... types>
    constexpr value_type& at(types... subscript)
    {
        static_assert(rank == sizeof...(subscript), "The number of argument is invalid.");
        static_assert(std::conjunction<std::is_convertible<types, measure_type>...>::value, "The arguments must be convertible to the measure type.");

        return at_impl(std::make_index_sequence<rank>{}, subscript...);
    }

    constexpr decltype(auto) operator[](measure_type subscript)
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
    inline constexpr value_type& at_impl(std::index_sequence<I...>, types... subscript)
    {
        return this->_data.at(std::get<I>(ranges).quantize(subscript)...);
    }

    template <std::size_t... I>
    inline auto lines_impl(std::index_sequence<I...>)
    {
        return Grid::prod(std::get<I>(ranges).line()...);
    }
};

}  // namespace Grid
