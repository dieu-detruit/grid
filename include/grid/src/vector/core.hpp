#pragma once

#include <algorithm>
#include <array>
#include <tuple>
#include <vector>

#include <grid/src/generic/multidim_base.hpp>
#include <grid/src/generic/multidim_proxy.hpp>
#include <grid/src/utility/parameter_pack.hpp>
#include <grid/src/utility/tuple.hpp>
#include <grid/src/vector/index.hpp>

namespace Grid::Impl
{

template <class T, std::size_t _rank>
struct multi_dim_vector : public MultidimContainerBase<T, _rank, std::vector<T>> {
public:
    using value_type = T;
    static constexpr std::size_t rank = _rank;

protected:
    using this_type = multi_dim_vector<T, rank>;

    std::array<std::size_t, rank> sizes;
    std::vector<T> data;

public:
    template <class U, std::size_t n>
    friend struct multidim_proxy;

    // iterator diverted from std::vector
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin() { return this->data.begin(); }
    const_iterator begin() const { return this->data.begin(); }
    iterator end() { return this->data.end(); }
    const_iterator end() const { return this->data.end(); }

    multi_dim_vector(this_type&& r) : data{std::move(r.data)}, sizes(std::move(r.sizes)) {}
    multi_dim_vector(const this_type& l) : data{l.data}, sizes(l.sizes) {}

    template <class... U>
    multi_dim_vector(U... size) : sizes{static_cast<std::size_t>(size)...}
    {
        static_assert(rank == sizeof...(size), "argument number must match the rank of vector");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        std::size_t whole_size = (... * size);
        data.resize(whole_size);
    }

    template <class U>
    multi_dim_vector(std::array<U, rank> sizes) : sizes{sizes}
    {
        static_assert(std::is_integral_v<U>, "The argument must be integral");
        std::size_t whole_size = 1;
        for (auto& size : sizes) {
            whole_size *= size;
        }
        data.resize(whole_size);
    }

    multi_dim_vector(dynamic_tuple<std::size_t, rank> sizes) : sizes{tuple_to_array(sizes)}
    {
        std::size_t whole_size = tuple_product(sizes);
        data.resize(whole_size);
    }

    multi_dim_vector(std::array<std::size_t, rank> sizes) : sizes{std::move(sizes)}
    {
        std::size_t whole_size = 1;
        for (int i = 0; i < rank; ++i) {
            whole_size *= sizes[i];
        }
        data.resize(whole_size);
    }

    template <class... U>
    inline T& at(U... subscript)
    {
        static_assert(sizeof...(subscript) == rank, "The number of argument must match the dimension rank");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        return data.at(vector_index<rank>::index(sizes, subscript...));
    }

    inline decltype(auto) operator[](std::size_t subscript)
    {
        if constexpr (rank == 1) {
            return data[subscript];
        } else {
            return multidim_proxy<this_type, rank - 1>{*this, subscript * sizes[1]};
        }
    }

    inline void fill(value_type val)
    {
        std::fill(data.begin(), data.end(), val);
    }

    template <std::size_t dim>
    inline std::size_t dim_size() const
    {
        return sizes[dim];
    }

    inline std::size_t dim_size(std::size_t dim) const
    {
        return sizes[dim];
    }
};

}  // namespace Grid::Impl
