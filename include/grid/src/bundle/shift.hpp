#pragma once

#include <iterator>

#include <grid/src/utility/iterator.hpp>
#include <grid/src/utility/reference.hpp>

namespace Grid
{

template <class T>
class Shift
{
    using iterator = Impl::copy_ref_info_t<T, Impl::get_iterator_t<T>>;

    T ref;
    std::size_t shift_size;

public:
    Shift(T ref, std::size_t shift_size) : ref(ref), shift_size(shift_size) {}

    auto begin()
    {
        return std::begin(ref) + shift_size;
    }
    auto end()
    {
        return std::end(ref);
    }
};

template <class T>
auto shift(T&& ref, std::size_t shift_size)
{
    return Shift<T>{ref, shift_size};
}

}  // namespace Grid
