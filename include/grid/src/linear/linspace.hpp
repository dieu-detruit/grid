#pragma once

#include <vector>

namespace Grid
{

template <class T>
std::vector<T> linspace(T start, T stop, std::size_t num, bool endpoint = true)
{
    T step = (stop - start) / (endpoint ? num - 1 : num);
    std::vector<T> result(num);
    for (auto& v : result) {
        v = start;
        start += step;
    }
    return result;
}

}  // namespace Grid
