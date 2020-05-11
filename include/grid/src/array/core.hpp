#pragma once

#include <array>
#include <tuple>
#include <type_traits>

#include <grid/src/array/index.hpp>
#include <grid/src/generic/multidim_proxy.hpp>
#include <grid/src/utility/parameter_pack.hpp>

namespace Grid::Impl
{

template <class T, std::size_t... N>
struct multi_dim_array {
public:
    static constexpr std::size_t rank = sizeof...(N);
    static constexpr std::size_t whole_size = (... * N);

    using value_type = T;

protected:
    using this_type = multi_dim_array<T, N...>;

    std::array<T, whole_size> _data;

public:
    template <class U, std::size_t n>
    friend struct multidim_proxy;

    // iterator diverted from std::array
    using iterator = typename std::array<T, whole_size>::iterator;
    using const_iterator = typename std::array<T, whole_size>::const_iterator;

    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }

    multi_dim_array() {}
    multi_dim_array(this_type&& r) : _data{std::move(r._data)} {}
    multi_dim_array(const this_type& l) : _data{l._data} {}

    multi_dim_array(T val)
    {
        _data.fill(val);
    }

    template <class... U>
    inline T& at(U... subscripts)
    {
        static_assert(sizeof...(U) == rank, "Number of arguments must match the dimension of array");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        return _data.at(array_index<N...>::index(subscripts...));
    }

    inline decltype(auto) operator[](std::size_t subscript)
    {
        if constexpr (rank == 1) {
            return _data[subscript];
        } else {
            return multidim_proxy<this_type, rank - 1>{*this, subscript * get_nth_param_v<std::size_t, 0, N...>};
        }
    }

    constexpr T* data()
    {
        return _data.data();
    }

    inline void fill(T value)
    {
        _data.fill(value);
    }

    template <std::size_t dim>
    inline std::size_t dim_size()
    {
        return get_nth_param_v<std::size_t, dim, N...>;
    }

    inline std::size_t dim_size(std::size_t dim)
    {
        return std::array<std::size_t, rank>{N...}[dim];
    }
};

}  // namespace Grid::Impl
