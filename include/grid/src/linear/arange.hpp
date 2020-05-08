#pragma once

#include <vector>

namespace Grid
{

template <class T>
std::vector<T> arange(T start, T stop, T step)
{
    std::vector<T> result;
    result.reserve(static_cast<std::size_t>((stop - start) / step));
    for (; start < stop; start += step) {
        result.emplace_back(start);
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
