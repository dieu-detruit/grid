#pragma once

#include <algorithm>
#include <array>
#include <tuple>
#include <valarray>

#include <grid/src/generic/multidim_base.hpp>
#include <grid/src/generic/multidim_proxy.hpp>
#include <grid/src/utility/parameter_pack.hpp>
#include <grid/src/utility/tuple.hpp>
#include <grid/src/vector/index.hpp>

namespace Grid::Impl
{

template <class T, std::size_t rank>
struct multidim_valarray : multidim_container_base<T, std::valarray<T>, rank> {
protected:
    using this_type = multidim_valarray<T, rank>;
    using base_type = typename this_type::base_type;

    std::array<std::size_t, rank> sizes;

public:
    // constructor
    multidim_valarray(std::valarray<T>&& rdata) : base_type{std::move(rdata)} {}
    multidim_valarray(this_type&& r) : base_type{std::move(r._data)}, sizes(std::move(r.sizes)) {}
    multidim_valarray(const this_type& l) : base_type{l._data}, sizes(l.sizes) {}

    template <class... U>
    multidim_valarray(U... size) : sizes{static_cast<std::size_t>(size)...}
    {
        static_assert(rank == sizeof...(size), "argument number must match the rank of valarray");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        std::size_t whole_size = (... * size);
        this->_data.resize(whole_size);
    }

    template <class U>
    multidim_valarray(std::array<U, rank> sizes) : sizes{sizes}
    {
        static_assert(std::is_integral_v<U>, "The argument must be integral");
        std::size_t whole_size = 1;
        for (auto& size : sizes) {
            whole_size *= size;
        }
        this->_data.resize(whole_size);
    }

    multidim_valarray(dynamic_tuple<std::size_t, rank> sizes) : sizes{tuple_to_array(sizes)}
    {
        std::size_t whole_size = tuple_product(sizes);
        this->_data.resize(whole_size);
    }

    // random access
    template <class... U>
    inline T& at(U... subscript)
    {
        static_assert(sizeof...(subscript) == rank, "The number of argument must match the dimension rank");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
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

// operators
#define DECLARE_SUBSTITUTION_OPERATOR(op) \
    this_type&                            \
    operator op(const T& x)               \
    {                                     \
        this->_data op x;                 \
        return *this;                     \
    }

    DECLARE_SUBSTITUTION_OPERATOR(+=);
    DECLARE_SUBSTITUTION_OPERATOR(-=);
    DECLARE_SUBSTITUTION_OPERATOR(*=);
    DECLARE_SUBSTITUTION_OPERATOR(/=);
};

}  // namespace Grid::Impl
