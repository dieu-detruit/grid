#pragma once

#include <grid/src/vector/core.hpp>

namespace Grid
{

// Multi-dimension Version std::vector
template <class T, std::size_t rank>
using vector = Impl::multidim_vector<T, rank>;

}  // namespace Grid
