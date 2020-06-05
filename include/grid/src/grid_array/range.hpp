#pragma once

#include <grid/src/linear/arange.hpp>

namespace Grid
{

template <class T, std::size_t N>
struct StaticRange {
protected:
    T _min;
    T _max;

    T _cell_size;

public:
    constexpr StaticRange(T min, T max)
        : _min(min), _max(max), _cell_size((_max - _min) / static_cast<T>(N)) {}

    /*
     * Policy:
     *
     *      The whole range is closed set.
     *
     *      each divided cell has the values satisfying
     *
     *          cell_min <= x < cell_max
     *
     *      but only if cell_max == max
     *
     *          cell_min <= x <= cell_max
     *
     */
    inline constexpr std::size_t quantize(T x) const
    {
        if (x == _max) {
            return N - 1;
        } else {
            return static_cast<std::size_t>((x - _min) / _cell_size);
        }
    }

    inline T min() const { return _min; }
    inline T max() const { return _max; }
    inline T cell_size() const { return _cell_size; }

    inline void min(T min_new)
    {
        _min = min_new;
        _cell_size = (_max - _min) / static_cast<T>(N);
    }
    inline void max(T max_new)
    {
        _min = max_new;
        _cell_size = (_max - _min) / static_cast<T>(N);
    }

    auto line()
    {
        auto cell_size_half = _cell_size / (T{2.0L} / T{1.0});
        return Grid::arange(_min + cell_size_half,
            _max - cell_size_half,
            _cell_size, true);
    }
};

}  // namespace Grid
