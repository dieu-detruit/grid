#pragma once

namespace Grid
{

template <class T>
struct DynamicRange {
protected:
    T min;
    T max;
    std::size_t N;

    T cell_size;

public:
    DynamicRange(T min, T max, std::size_t N)
        : min(min), max(max), N(N), cell_size((max - min) / T{static_cast<T>(N)}) {}

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
    inline std::size_t quantize(T x) const
    {
        if (x == max) {
            return N - 1;
        } else {
            return static_cast<std::size_t>((x - min) / cell_size);
        }
    }

    void resize(T _min, T _max)
    {
        min = _min;
        max = _max;
    }

    operator std::size_t() const
    {
        return N;
    }
};

}  // namespace Grid
