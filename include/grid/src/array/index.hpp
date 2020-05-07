#include <array>
#include <cstdarg>

#include <grid/src/meta.hpp>

namespace Grid::Impl
{

// index for array
template <std::size_t n, std::size_t... N>
struct array_index_coef {
    static_assert(n < sizeof...(N), "index is out of range");
    static constexpr std::size_t value
        = get_nth_param_v<std::size_t, sizeof...(N) - n, N...> * array_index_coef<n - 1, N...>::value;
};

template <std::size_t... N>
struct array_index_coef<sizeof...(N) - 1, N...> {
    static constexpr std::size_t value = 1;
};

template <std::size_t... N>
struct array_index {
public:
    static constexpr std::size_t rank = sizeof...(N);

    template <std::size_t index, std::size_t... params>
    struct array_index_coef_list {
        static constexpr auto value
            = array_index_coef_list<
                index + 1,
                array_index_coef<index>::value,
                params...>::value;
    };
    template <std::size_t... params>
    struct array_index_coef_list<rank, params...> {
        static constexpr std::array<std::size_t, rank> value = {params...};
    };

public:
    static constexpr std::array<std::size_t, rank> coefs = array_index_coef_list<0>::value;

    template <class T>
    static inline std::size_t index(T subscript...)
    {
        va_list args;
        va_start(args, subscript);

        std::size_t result = 0;
        for (auto& coef : coefs) {
            result += va_arg(args, T);
        }

        va_end(args);
        return result;
    }
};

template <std::size_t N>
struct array_index<N> {
    template <class U>
    static inline std::size_t index(U subscript)
    {
        return subscript;
    }
};
template <std::size_t N0, std::size_t N1>
struct array_index<N0, N1> {
    template <class U0, class U1>
    static inline std::size_t index(U0 sub0, U1 sub1)
    {
        return N1 * sub0 + sub1;
    }
};
template <std::size_t N0, std::size_t N1, std::size_t N2>
struct array_index<N0, N1, N2> {
    template <class U0, class U1, class U2>
    static inline std::size_t index(U0 sub0, U1 sub1, U2 sub2)
    {
        return N2 * (N1 * sub0 + sub1) + sub2;
    }
};


// index for vector
template <unsigned int N, class... U>
struct vector_index {
    static inline std::size_t index(std::array<unsigned int, N> size, U... subscript)
    {
        using largest_type = get_max_size_type_t<U...>;
        std::array<largest_type, N> subs{{subscript...}};

        std::size_t result = subs[0];
        for (int i = 1; i < N; ++i) {
            result = result * size[i] + subs[i];
        }
        return result;
    }
};

template <class U>
struct vector_index<1, U> {
    static inline std::size_t index(std::array<unsigned int, 1> size, U subscript)
    {
        return subscript;
    }
};
template <class U1, class U2>
struct vector_index<2, U1, U2> {
    static inline std::size_t index(std::array<unsigned int, 2> size, U1 sub0, U2 sub1)
    {
        return size[1] * sub0 + sub1;
    }
};
template <class U1, class U2, class U3>
struct vector_index<3, U1, U2, U3> {
    static inline std::size_t index(std::array<unsigned int, 3> size, U1 sub0, U2 sub1, U3 sub2)
    {
        return size[2] * (size[1] * sub0 + sub1) + sub2;
    }
};

}  // namespace Grid::Impl
