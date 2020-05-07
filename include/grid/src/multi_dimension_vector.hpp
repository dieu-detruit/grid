#pragma once

#include <array>
#include <cstdarg>
#include <tuple>
#include <vector>

#include <grid/src/meta.hpp>

namespace Grid::Impl
{

template <unsigned int N, class... U>
struct index_calculator {
    static inline unsigned long long index(std::array<unsigned int, N> size, U... subscript)
    {
        using largest_type = get_max_size_type_t<U...>;
        std::array<largest_type, N> subs{{subscript...}};

        unsigned long long result = subs[0];
        for (int i = 1; i < N; ++i) {
            result = result * size[i] + subs[i];
        }
        return result;
    }
};

template <class U>
struct index_calculator<1, U> {
    static inline unsigned long long index(std::array<unsigned int, 1> size, U subscript)
    {
        return subscript;
    }
};
template <class U1, class U2>
struct index_calculator<2, U1, U2> {
    static inline unsigned long long index(std::array<unsigned int, 2> size, U1 sub0, U2 sub1)
    {
        return size[1] * sub0 + sub1;
    }
};
template <class U1, class U2, class U3>
struct index_calculator<3, U1, U2, U3> {
    static inline unsigned long long index(std::array<unsigned int, 3> size, U1 sub0, U2 sub1, U3 sub2)
    {
        return size[2] * (size[1] * sub0 + sub1) + sub2;
    }
};

template <class T, unsigned int N>
struct multi_dim_vector {
    std::array<unsigned int, N> size;
    std::vector<T> mem;

    multi_dim_vector(std::array<unsigned long long, N> size) : size(std::move(size))
    {
        unsigned long long whole_size = 1;
        for (int i = 0; i < N; ++i) {
            whole_size *= size[i];
        }
        mem.resize(whole_size);
    }

    template <class... U>
    inline T& at(U... subscript)
    {
        static_assert(sizeof(subscript) == N, "The number of argument must match the dimension N");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        return mem.at(index_calculator::index(subscript...));
    }

    template <class... U>
    inline T& operator[](U... subscript)
    {
        static_assert(sizeof(subscript) == N, "The number of argument must match the dimension N");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        return mem[index_calculator::index(subscript...)];
    }
};


}  // namespace Grid::Impl
