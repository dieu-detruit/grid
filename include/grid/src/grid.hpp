#pragma once

#include <array>

#include <grid/src/multi_dimension_stl.hpp>
#include <grid/src/range.hpp>

namespace Grid
{

template <class T, class measure_type, unsigned int N>
class GridMap : GridMapBase<T, measure_type>
{
protected:
public:
    T& operator[](measure_type... x)
    {
    }
    T& at(measure_type... x) = 0;
};

}  // namespace Grid
