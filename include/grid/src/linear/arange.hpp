#pragma once

#include <vector>

namespace Grid
{

template <class T>
std::vector<T> arange(T start, T stop, T step, bool endpoint = true)
{
    std::size_t size = static_cast<std::size_t>((stop - start) / step) + (endpoint ? 1UL : 0UL);
    std::vector<T> result(size);
    for (std::size_t i = 0; i < size; ++i) {
        result[i] = start;
        start += step;
    }
    return result;
}

}  // namespace Grid
