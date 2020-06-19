#pragma once

#include <grid/parallel.hpp>

#include <grid/src/grid_array/core.hpp>
#include <grid/src/grid_vector/core.hpp>

namespace Grid
{

template <class T, std::size_t... N>
using GridArrayf = GridArray<T, float, N...>;
template <class T, std::size_t... N>
using GridArrayd = GridArray<T, double, N...>;
template <class T, std::size_t... N>
using GridArrayld = GridArray<T, double, N...>;


template <class T>
using GridVector1f = GridVector<T, float, 1>;
template <class T>
using GridVector2f = GridVector<T, float, 2>;
template <class T>
using GridVector3f = GridVector<T, float, 3>;

template <class T>
using GridVector1d = GridVector<T, double, 1>;
template <class T>
using GridVector2d = GridVector<T, double, 2>;
template <class T>
using GridVector3d = GridVector<T, double, 3>;

template <class T>
using GridVector1ld = GridVector<T, long double, 1>;
template <class T>
using GridVector2ld = GridVector<T, long double, 2>;
template <class T>
using GridVector3ld = GridVector<T, long double, 3>;

}  // namespace Grid
