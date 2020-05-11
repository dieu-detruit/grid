#include <array>
#include <cstdarg>

#include <grid/src/utility/parameter_pack.hpp>

namespace Grid::Impl
{

template <std::size_t n, std::size_t... N>
struct array_index_coef {
    static_assert(0 <= n && n < sizeof...(N), "index is out of range");
    static constexpr std::size_t value
        = get_nth_param_v<std::size_t, n + 1, N...> * array_index_coef<n + 1, N...>::value;
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
                params...,
                array_index_coef<index, N...>::value>::value;
    };
    template <std::size_t... params>
    struct array_index_coef_list<rank, params...> {
        static constexpr std::array<std::size_t, rank> value = {params...};
    };

public:
    static constexpr std::array<std::size_t, rank> coefs = array_index_coef_list<0>::value;

    template <class... U>
    static inline std::size_t index(U... subscript)
    {
        return index(std::array<std::size_t, rank>{subscript...});
    }

    static inline std::size_t index_ar(const std::array<std::size_t, rank>& subscript)
    {
        std::size_t result = 0;
        for (int i = 0; i < rank; ++i) {
            result += coefs[i] * subscript[i];
        }
        return result;
    }
};

template <std::size_t N0, std::size_t N1>
struct array_index<N0, N1> {
public:
    static constexpr std::size_t rank = 2;

    template <class U0, class U1>
    static inline std::size_t index(U0 sub0, U1 sub1)
    {
        return N1 * sub0 + sub1;
    }
    static inline std::size_t index_ar(const std::array<std::size_t, rank>& subscript)
    {
        return N1 * subscript[0] + subscript[1];
    }
};

}  // namespace Grid::Impl
