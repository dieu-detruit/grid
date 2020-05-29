#pragma once

namespace Grid::Impl
{

// tag for type sequence
template <class... types>
struct type_sequence_tag {
};
template <class T>
struct type_sequence_tag<T> {
    using type = T;
};
template <>
struct type_sequence_tag<> {
};


// tag that has 0 argument constructor to get the result type of function with no argument invoke
template <class T>
using type_tag = type_sequence_tag<T>;

using empty_tag = type_sequence_tag<>;

}  // namespace Grid::Impl
