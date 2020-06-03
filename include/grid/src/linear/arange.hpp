#pragma once

#include <vector>

namespace Grid
{

template <class T>
std::vector<T> arange(T start, T stop, T step, bool endpoint = true)
{
    std::vector<T> result;
    std::size_t size = static_cast<std::size_t>((stop - start) / step) + (endpoint ? 1UL : 0UL);
    result.reserve(size);
    if (endpoint) {
        for (; start <= stop; start += step) {
            result.emplace_back(start);
        }
    } else {
        for (; start < stop; start += step) {
            result.emplace_back(start);
        }
    }
    return result;
}

template <class T>
std::vector<T> arange(T start, T stop)
{
    std::vector<T> result;
    result.reserve(static_cast<std::size_t>(stop - start));
    for (; start < stop; start += T{1}) {
        result.emplace_back(start);
    }
    return result;
}

template <class T>
std::vector<T> arange(T stop)
{

    std::vector<T> result;
    result.reserve(static_cast<std::size_t>(stop));
    for (T tmp{0}; tmp < stop; tmp += T{1}) {
        result.emplace_back(tmp);
    }
    return result;
}

}  // namespace Grid
