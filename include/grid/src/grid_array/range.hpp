#pragma once

namespace Grid
{

template <class T, std::size_t N>
struct StaticRange {
protected:
    T min;
    T max;

    T cell_size;

public:
    constexpr StaticRange(T min, T max)
        : min(min), max(max), cell_size((max - min) / T{N}) {}

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
        if (x == max) {
            return N - 1;
        } else {
            return static_cast<std::size_t>((x - min) / cell_size);
        }
    }
};

}  // namespace Grid
