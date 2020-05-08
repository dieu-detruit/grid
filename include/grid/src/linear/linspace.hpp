#pragma once

#include <vector>

namespace Grid
{

template <class T>
std::vector<T> linspace(T start, T stop, std::size_t num)
{
    T step = (stop - start) / static_cast<T>(num);
    std::vector<T> result(num);
    for (auto& v : result) {
        v = start;
        start += step;
    }
    return result;
}

template <class T>
std::vector<T> linspace(T start, T stop)
{
    constexpr T step{1};
    std::vector<T> result(static_cast<std::size_t>(stop - start));
    for (auto& v : result) {
        v = start;
        start += step;
    }
    return result;
}

template <class T>
std::vector<T> linspace(T stop)
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
