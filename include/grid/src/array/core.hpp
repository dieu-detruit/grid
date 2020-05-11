#pragma once

#include <array>
#include <tuple>
#include <type_traits>

#include <grid/src/array/index.hpp>
#include <grid/src/generic/multidim_base.hpp>
#include <grid/src/generic/multidim_proxy.hpp>
#include <grid/src/utility/parameter_pack.hpp>

namespace Grid::Impl
{

template <class T, std::size_t... N>
struct multidim_array
    : multidim_container_base<T, std::array<T, (... * N)>, sizeof...(N)> {

protected:
    using this_type = multidim_array<T, N...>;
    using base_type = typename this_type::base_type;

public:
    using container_type = typename this_type::container_type;
    static constexpr std::size_t rank = base_type::rank;

    multidim_array() {}
    multidim_array(container_type&& rdata) : base_type{std::move(rdata)} {}
    multidim_array(this_type&& r) : base_type{std::move(r._data)} {}
    multidim_array(const this_type& l) : base_type{l._data} {}

    multidim_array(T val)
    {
        this->_data.fill(val);
    }

    template <class... U>
    inline T& at(U... subscripts)
    {
        static_assert(sizeof...(U) == rank, "Number of arguments must match the dimension of array");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        return this->_data.at(array_index<N...>::index(subscripts...));
    }

    inline decltype(auto) operator[](std::size_t subscript)
    {
        if constexpr (rank == 1) {
            return this->_data[subscript];
        } else {
            return multidim_proxy<this_type, rank - 1>{*this, subscript * get_nth_param_v<std::size_t, 0, N...>};
        }
    }

    template <std::size_t dim>
    inline std::size_t dim_size() const
    {
        return get_nth_param_v<std::size_t, dim, N...>;
    }

    inline std::size_t dim_size(std::size_t dim) const override
    {
        return std::array<std::size_t, rank>{N...}[dim];
    }
};

}  // namespace Grid::Impl
