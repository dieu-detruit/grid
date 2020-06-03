#pragma once

#include <grid/src/valarray/core.hpp>

namespace Grid
{

// Multi-dimension Version std::vector
template <class T, std::size_t rank>
using valarray = Impl::multidim_valarray<T, rank>;

}  // namespace Grid
