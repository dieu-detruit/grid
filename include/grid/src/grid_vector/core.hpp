#pragma once

#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <grid/linear.hpp>
#include <grid/src/generic/grid_base.hpp>
#include <grid/src/generic/grid_proxy.hpp>
#include <grid/src/grid_vector/range.hpp>
#include <grid/src/utility/tuple.hpp>
#include <grid/vector.hpp>

namespace Grid
{

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

    auto range(std::size_t n) const
    {
        auto range = Impl::tuple_to_ref_array(ranges).at(n);
        return std::make_pair<measure_type, measure_type>(range.min(), range.max());
    }
    auto line(std::size_t n)
    {
        const auto& range = Impl::tuple_to_ref_array(ranges).at(n);
        return arange(range.min(), range.max(), range.cell_size());
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
};

}  // namespace Grid
