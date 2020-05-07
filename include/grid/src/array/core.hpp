#pragma once

#include <array>
#include <tuple>
#include <type_traits>

#include <grid/src/array/index.hpp>
#include <grid/src/utility/meta.hpp>

namespace Grid::Impl
{

template <class dim_array_type, std::size_t rank_rest>
class array_square_bracket_result
{
    using sub_array_type = std::array<std::size_t, dim_array_type::rank>;

    sub_array_type subscripts;
    dim_array_type& ref;

public:
    array_square_bracket_result(dim_array_type& ref)
        : ref(ref), subscripts{} {}

    array_square_bracket_result(dim_array_type& ref, sub_array_type& subscripts)
        : ref(ref), subscripts(std::move(subscripts)) {}

    array_square_bracket_result(array_square_bracket_result&& r)
        : ref(r.ref), subscripts(std::move(r.subscripts)) {}

    template <class U>
    auto operator[](U subscript)
    {
        subscripts.at(dim_array_type::rank - rank_rest) = subscript;
        return array_square_bracket_result<dim_array_type, rank_rest - 1>{ref, subscripts};
    }
};

template <class dim_array_type>
class array_square_bracket_result<dim_array_type, 1>
{
    using sub_array_type = std::array<std::size_t, dim_array_type::rank>;

    sub_array_type subscripts;
    dim_array_type& ref;

public:
    array_square_bracket_result(dim_array_type& ref)
        : ref(ref), subscripts{} {}

    array_square_bracket_result(dim_array_type& ref, sub_array_type& subscripts)
        : ref(ref), subscripts(subscripts) {}

    array_square_bracket_result(array_square_bracket_result&& r)
        : ref(r.ref), subscripts(std::move(r.subscripts)) {}

    template <class U>
    typename dim_array_type::value_type& operator[](U subscript)
    {
        subscripts.at(dim_array_type::rank - 1) = subscript;
        return ref.bracket(subscripts);
    }
};

template <class T, std::size_t... N>
struct multi_dim_array {
public:
    static constexpr std::size_t rank = sizeof...(N);
    static constexpr std::size_t whole_size = (... * N);

    using value_type = T;

protected:
    using this_type = multi_dim_array<T, N...>;

    std::array<T, whole_size> data;

    using bracket_return_type = std::conditional_t<
        rank == 1,
        T&,
        array_square_bracket_result<this_type, rank - 1>>;

public:
    friend struct array_square_bracket_result<this_type, 1>;

    // iterator diverted from std::vector
    using iterator = typename std::array<T, whole_size>::iterator;
    using const_iterator = typename std::array<T, whole_size>::const_iterator;

    iterator begin() { return this->data.begin(); }
    const_iterator begin() const { return this->data.begin(); }
    iterator end() { return this->data.end(); }
    const_iterator end() const { return this->data.end(); }

    multi_dim_array() {}
    multi_dim_array(this_type&& r) : data{std::move(r.data)} {}
    multi_dim_array(const this_type& l) : data{l.data} {}

    multi_dim_array(T val)
    {
        data.fill(val);
    }

    template <class... U>
    inline T& at(U... subscripts)
    {
        static_assert(sizeof...(U) == rank, "Number of arguments must match the dimension of array");
        static_assert(is_all_integral_v<U...>, "The arguments must be integral");
        return data.at(array_index<N...>::index(subscripts...));
    }

    template <class U>
    inline bracket_return_type operator[](U subscript)
    {
        static_assert(std::is_integral_v<U>, "The argument must be integral");
        if constexpr (rank == 1) {
            return data[array_index<N...>::index(subscript)];
        } else {
            return array_square_bracket_result<this_type, rank>{*this}[subscript];
        }
    }

protected:
    template <std::size_t n>
    using func_arg_types = std::size_t;

    inline T& bracket(std::array<std::size_t, rank> subscripts)
    {
        return data[array_index<N...>::template index(subscripts)];
    }
};

}  // namespace Grid::Impl
