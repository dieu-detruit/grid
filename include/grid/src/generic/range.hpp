#pragma once

namespace Grid
{

template <class T>
struct Range {
protected:
    T _min;
    T _max;

    T _cell_size;

public:
    constexpr DynamicRange(T min, T max, std::size_t N)
        : _min(min), _max(max), _N(N), _cell_size((_max - _min) / static_cast<T>(_N)) {}

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
            return _N - 1;
        } else {
            return static_cast<std::size_t>((x - _min) / _cell_size);
        }
    }
    // Cast operator to std::size_t
    operator std::size_t()
    {
        return _N;
    }

    inline T min() const { return _min; }
    inline T max() const { return _max; }
    inline T cell_size() const { return _cell_size; }
    inline std::size_t N() const { return _N; }


    inline void min(T min_new)
    {
        _min = min_new;
        recalc();
    }
    inline void max(T max_new)
    {
        _min = max_new;
        recalc();
    }
    inline void N(std::size_t N_new)
    {
        _N = N_new;
        recalc();
    }
    void resize(T min, T max)
    {
        _min = min;
        _max = max;
        recalc();
    }
    void resize(T min, T max, std::size_t N)
    {
        _min = min;
        _max = max;
        _N = N;
        recalc();
    }
};

}  // namespace Grid
