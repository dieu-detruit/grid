#pragma once

#include <vector>

namespace Grid
{

template <class T>
std::vector<T> linspace(T start, T stop, std::size_t num, bool endpoint = true)
{
    T step = (stop - start) / static_cast<T>(endpoint ? num - 1 : num);
    std::vector<T> result(num);
    for (auto& v : result) {
        v = start;
        start += step;
    }
    return result;
}

template <class T>
std::vector<T> linspace(T start, T stop, bool endpoint = true)
{
    constexpr T step{1};
    std::vector<T> result(static_cast<std::size_t>(stop - start) + (endpoint ? 1UL : 0));
    for (auto& v : result) {
        v = start;
        start += step;
    }
    return result;
}

template <class T>
std::vector<T> linspace(T stop, bool endpoint = true)
{
    constexpr T step{1}, tmp{0};
    std::vector<T> result(static_cast<std::size_t>(stop));
    for (auto& v : result) {
        v = tmp;
        tmp += step;
    }
    return result;
}

}  // namespace Grid
