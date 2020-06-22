#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace Grid::Impl
{

template <class T>
struct get_iterator {
    using type = decltype(std::begin(std::declval<std::add_lvalue_reference_t<T>>()));
};

template <class T>
using get_iterator_t = get_iterator<T>::type;

}  // namespace Grid::Impl
