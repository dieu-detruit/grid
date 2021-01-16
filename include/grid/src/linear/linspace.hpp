#pragma once

#include <algorithm>
#include <execution>
#include <numeric>
#include <vector>

namespace Grid
{

template <class T>
std::vector<T> linspace(T start, T stop, std::size_t num, bool endpoint = true)
{
    T step = (stop - start) / (endpoint ? num - 1 : num);

    std::vector<T> result(num);
    std::iota(result.begin(), result.end(), 0.0);
    std::transform(std::execution::par_unseq, result.begin(), result.end(), result.begin(),
        [&start, &step](T& x) { return start + step * x; });

    return result;
}

}  // namespace Grid
