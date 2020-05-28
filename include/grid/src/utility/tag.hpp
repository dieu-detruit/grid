#pragma once

namespace Grid::Impl
{

// tag for type sequence
template <class... types>
struct type_sequence_tag {
};

// tag that has 0 argument constructor to get the result type of function with no argument invoke
template <class T>
struct type_tag {
    using type = T;
};

// unwrap empty tag
template <class T>
struct unwrap_type_tag {
    using type = T;
};
template <class T>
struct unwrap_type_tag<type_tag<T>> {
    using type = T;
};
template <class T>
using unwrap_type_tag_t = typename unwrap_type_tag<T>::type;

}  // namespace Grid::Impl
