#pragma once

#include <array>

#include <grid/src/multi_dimension_stl.hpp>
#include <grid/src/range.hpp>

namespace Grid
{

template <class T, class measure_type, int N>
class GridMapBase
{
protected:
    using M_t = measure_type;

    Impl::multi_dim_vector<T, N> map;
    std::array<std::tuple<Range<T>, measure_type>, N> grid;

public:
    GridBase(const std::array<std::tuple<Range<T>, measure_type>, N>& grid);
    GridBase(std::array<std::tuple<Range<T>, measure_type>, N> grid);

    T& operator[](measure_type... x) = 0;
    T& at(measure_type... x) = 0;
};

}  // namespace Grid
