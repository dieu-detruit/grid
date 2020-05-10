#pragma once

namespace Grid::Impl
{

// tag for type sequence
template <class... types>
struct type_sequence_tag {
};

// tag that has 0 argument constructor to get the result type of function with no argument invoke
template <class T>
struct type_empty_tag {
    using type = T;
};

// unwrap empty tag
template <class T>
struct unwrap_empty_tag {
    using type = T;
};
template <class T>
struct unwrap_empty_tag<type_empty_tag<T>> {
    using type = T;
};
template <class T>
using unwrap_empty_tag_t = typename unwrap_empty_tag<T>::type;

}  // namespace Grid::Impl
