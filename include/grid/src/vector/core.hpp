#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <tuple>
#include <vector>

#include <grid/src/generic/multidim_base.hpp>
#include <grid/src/generic/multidim_proxy.hpp>
#include <grid/src/utility/parameter_pack.hpp>
#include <grid/src/utility/tuple.hpp>
#include <grid/src/vector/index.hpp>

namespace Grid::Impl
{

template <class T, std::size_t rank>
struct multidim_vector : multidim_container_base<T, std::vector<T>, rank> {
protected:
    using this_type = multidim_vector<T, rank>;
    using base_type = this_type::base_type;

    std::array<std::size_t, rank> sizes;

public:
    // constructor
    multidim_vector(std::vector<T>&& rdata) : base_type{std::move(rdata)} {}
    multidim_vector(this_type&& r) : base_type{std::move(r._data)}, sizes(std::move(r.sizes)) {}
    multidim_vector(const this_type& l) : base_type{l._data}, sizes(l.sizes) {}

    template <std::unsigned_integral... U>
    constexpr multidim_vector(U... size) : sizes{static_cast<std::size_t>(size)...}
    {
        static_assert(rank == sizeof...(size), "argument number must match the rank of vector");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        std::size_t whole_size = (... * size);
        this->_data.resize(whole_size);
    }

    // resize
    template <std::unsigned_integral... U>
    void resize(U... size)
    {
        static_assert(sizeof...(size) == rank, "The number of argument must match the dimension rank");
        sizes = std::array<std::size_t, rank>{{size...}};

        std::size_t whole_size = (... * size);
        this->_data.resize(whole_size);
    }

    // random access
    template <std::unsigned_integral... U>
    inline T& at(U... subscript)
    {
        static_assert(sizeof...(subscript) == rank, "The number of argument must match the dimension rank");
        return this->_data.at(vector_index<rank>::index(sizes, subscript...));
    }

    inline decltype(auto) operator[](std::size_t subscript)
    {
        if constexpr (rank == 1) {
            return this->_data[subscript];
        } else {
            return multidim_proxy<this_type, rank - 1>{*this, subscript * sizes[1]};
        }
    }

    template <std::size_t dim>
    inline std::size_t dim_size() const
    {
        return sizes[dim];
    }

    inline std::size_t dim_size(std::size_t dim) const override
    {
        return sizes[dim];
    }
};

}  // namespace Grid::Impl
