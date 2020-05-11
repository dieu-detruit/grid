#pragma once

#include <grid/src/array/core.hpp>

namespace Grid
{

// Multi-dimension Version std::array
template <class T, std::size_t... N>
using array = Impl::multidim_array<T, N...>;

}  // namespace Grid
