#pragma once

#include <algorithm>
#include <execution>
#include <numeric>
#include <vector>

namespace Grid
{

template <class T>
std::vector<T> arange(T start, T stop, T step, bool endpoint = true)
{
    std::size_t size = static_cast<std::size_t>((stop - start) / step) + (endpoint ? 1UL : 0UL);

    std::vector<T> result(size);
    std::iota(result.begin(), result.end(), 0.0);
    std::transform(std::execution::par_unseq, result.begin(), result.end(), result.begin(),
        [&start, &step](T& x) { return start + step * x; });

    return result;
}

}  // namespace Grid
