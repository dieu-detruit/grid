#pragma once

#include <array>
#include <tuple>
#include <vector>

#include <grid/src/utility/meta.hpp>
#include <grid/src/vector/index.hpp>

namespace Grid::Impl
{

template <class dim_vector_type, std::size_t rank_rest>
class vector_square_bracket_result
{
    using sub_vector_type = std::array<std::size_t, dim_vector_type::rank>;

    sub_vector_type subscripts;
    dim_vector_type& ref;

public:
    vector_square_bracket_result(dim_vector_type& ref)
        : ref(ref), subscripts{} {}

    vector_square_bracket_result(dim_vector_type& ref, sub_vector_type& subscripts)
        : ref(ref), subscripts(std::move(subscripts)) {}

    vector_square_bracket_result(vector_square_bracket_result&& r)
        : ref(r.ref), subscripts(std::move(r.subscripts)) {}

    template <class U>
    auto operator[](U subscript)
    {
        subscripts.at(dim_vector_type::rank - rank_rest) = subscript;
        return vector_square_bracket_result<dim_vector_type, rank_rest - 1>{ref, subscripts};
    }
};

template <class dim_vector_type>
class vector_square_bracket_result<dim_vector_type, 1>
{
    using sub_vector_type = std::array<std::size_t, dim_vector_type::rank>;

    sub_vector_type subscripts;
    dim_vector_type& ref;

public:
    vector_square_bracket_result(dim_vector_type& ref)
        : ref(ref), subscripts{} {}

    vector_square_bracket_result(dim_vector_type& ref, sub_vector_type& subscripts)
        : ref(ref), subscripts(subscripts) {}

    vector_square_bracket_result(vector_square_bracket_result&& r)
        : ref(r.ref), subscripts(std::move(r.subscripts)) {}

    template <class U>
    typename dim_vector_type::value_type& operator[](U subscript)
    {
        subscripts.at(dim_vector_type::rank - 1) = subscript;
        return ref.bracket(subscripts);
    }
};


template <class T, std::size_t _rank>
struct multi_dim_vector {
public:
    using value_type = T;
    static constexpr std::size_t rank = _rank;

protected:
    using this_type = multi_dim_vector<T, rank>;

    std::array<std::size_t, rank> sizes;
    std::vector<T> data;

    using bracket_return_type = std::conditional_t<
        rank == 1,
        T&,
        vector_square_bracket_result<this_type, rank - 1>>;

public:
    friend struct vector_square_bracket_result<this_type, 1>;

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

    template <class U>
    inline bracket_return_type operator[](U subscript)
    {
        static_assert(std::is_integral_v<U>, "The argument must be integral");
        if constexpr (rank == 1) {
            return data[vector_index<rank>::index(sizes, subscript)];
        } else {
            return vector_square_bracket_result<this_type, rank>{*this}[subscript];
        }
    }

protected:
    inline T& bracket(std::array<std::size_t, rank> subscripts)
    {
        return data[vector_index<rank>::index(sizes, subscripts)];
    }
};

}  // namespace Grid::Impl
