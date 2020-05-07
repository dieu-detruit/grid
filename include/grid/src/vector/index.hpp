#include <array>
#include <cstdarg>

#include <grid/src/utility/meta.hpp>

namespace Grid::Impl
{

// index for vector
template <std::size_t rank>
struct vector_index {
    template <class... U>
    static inline std::size_t index(std::array<std::size_t, rank> size, U... subscript)
    {
        std::array<std::size_t, rank> subscripts{subscript...};
        std::size_t result = subscripts[0];
        for (int i = 1; i < rank; ++i) {
            result = result * size[i] + subscripts[i];
        }
        return result;
    }

    template <class... U>
    static inline std::size_t index(std::array<std::size_t, rank> size, std::array<std::size_t, rank> subscripts)
    {
        std::size_t result = subscripts[0];
        for (int i = 1; i < rank; ++i) {
            result = result * size[i] + subscripts[i];
        }
        return result;
    }
};

template <>
struct vector_index<1> {
    template <class U0>
    static inline std::size_t index(std::array<std::size_t, 1> size, U0 sub0)
    {
        return sub0;
    }
    static inline std::size_t index(std::array<std::size_t, 1> size, std::array<std::size_t, 1> subscripts)
    {
        return subscripts[0];
    }
};
template <>
struct vector_index<2> {
    template <class U0, class U1>
    static inline std::size_t index(std::array<std::size_t, 2> size, U0 sub0, U1 sub1)
    {
        return size[1] * sub0 + sub1;
    }
    static inline std::size_t index(std::array<std::size_t, 2> size, std::array<std::size_t, 2> subscripts)
    {
        return size[1] * subscripts[0] + subscripts[1];
    }
};
template <>
struct vector_index<3> {
    template <class U0, class U1, class U2>
    static inline std::size_t index(std::array<std::size_t, 3> size, U0 sub0, U1 sub1, U2 sub2)
    {
        return size[2] * (size[1] * sub0 + sub1) + sub2;
    }

    static inline std::size_t index(std::array<std::size_t, 3> size, std::array<std::size_t, 3> subscripts)
    {
        return size[2] * (size[1] * subscripts[0] + subscripts[1]) + subscripts[2];
    }
};

}  // namespace Grid::Impl
