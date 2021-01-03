#pragma once

namespace Grid
{

template <class T>
struct DynamicRange {

    using this_type = DynamicRange<T>;

    T min;
    T max;
    std::size_t N;

    T cell_size;

    constexpr DynamicRange(T min, T max, std::size_t N)
        : min(min), max(max), N(N), cell_size((max - min) / N) {}

    // Cast operator to std::size_t
    explicit operator std::size_t() { return N; }

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
        cell_size = (max - min) / N;
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

    auto line() const
    {
        auto cell_size_half = cell_size / 2;
        return Grid::arange(min + cell_size_half,
            max - cell_size_half,
            cell_size, true);
    }

    T length() const
    {
        return max - min;
    }

    // 範囲拡大・縮小
    this_type& operator*=(std::size_t n)
    {
        N *= n;
        auto center = (max + min) / 2;
        min = center + (min - center) * n;
        max = center + (max - center) * n;
        recalc();
        return *this;
    }
    this_type& operator/=(std::size_t n)
    {
        N /= n;
        auto center = (max + min) / 2;
        min = center + (min - center) / n;
        max = center + (max - center) / n;
        recalc();
        return *this;
    }
    auto operator*(std::size_t n)
    {
        auto center = (max + min) / 2;
        return this_type{center + (min - center) * n, center + (max - center) * n, N * n};
    }
    auto operator/(std::size_t n)
    {
        auto center = (max + min) / 2;
        return this_type{center + (min - center) / n, center + (max - center) / n, N / n};
    }

    // 平行移動
    this_type& operator+=(T translate)
    {
        min += translate;
        max += translate;
        recalc();
        return *this;
    }
    this_type& operator-=(T translate)
    {
        min -= translate;
        max -= translate;
        recalc();
        return *this;
    }
    auto operator+(T translate)
    {
        return this_type{min + translate, max + translate, N};
    }
    auto operator-(T translate)
    {
        return this_type{min - translate, max + translate, N};
    }
};


// 範囲拡大・縮小
template <typename T>
auto operator*(std::size_t n, DynamicRange<T>& range)
{
    auto center = (range.max + range.min) / 2;
    return DynamicRange<T>{center + (range.min - center) * n, center + (range.max - center) * n, range.N * n};
}
template <typename T>
auto operator/(std::size_t n, DynamicRange<T>& range)
{
    auto center = (range.max + range.min) / 2;
    return DynamicRange<T>{center + (range.min - center) / n, center + (range.max - center) / n, range.N / n};
}

// 平行移動
template <typename T>
auto operator+(T translate, DynamicRange<T>& range)
{
    return DynamicRange<T>{range.min + translate, range.max + translate, range.N};
}
template <typename T>
auto operator-(T translate, DynamicRange<T>& range)
{
    return DynamicRange<T>{range.min - translate, range.max - translate, range.N};
}


}  // namespace Grid
