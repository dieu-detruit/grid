#pragma once

namespace Grid
{

template <class T>
struct DynamicRange {
    T min;
    T max;
    std::size_t N;

    T cell_size;

    constexpr DynamicRange(T min, T max, std::size_t N)
        : min(min), max(max), N(N), cell_size((max - min) / static_cast<T>(N)) {}

    // Cast operator to std::size_t
    operator std::size_t() { return N; }

    /*
     * Policy:
     *
     *      The whole range is closed set.
     *
     *      each divided cell has the values satisfying
     *
     *          cellmin <= x < cellmax
     *
     *      but only if cellmax == max
     *
     *          cellmin <= x <= cellmax
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

    inline void recalc()
    {
        cell_size = (max - min) / static_cast<T>(N);
    }

    void resize(T _min, T _max)
    {
        min = _min;
        max = _max;
        recalc();
    }
    void resize(T _min, T _max, std::size_t _N)
    {
        min = _min;
        max = _max;
        N = _N;
        recalc();
    }

    auto line()
    {
        auto cell_size_half = cell_size / (T{2.0L} / T{1.0});
        return Grid::arange(min + cell_size_half,
            max - cell_size_half,
            cell_size, true);
    }
};


}  // namespace Grid
