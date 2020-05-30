#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include <grid/src/utility/concept.hpp>
#include <vector>

namespace Grid::Impl
{
template <class T>
struct Iterative {
    void constraint()
    {
        T a;
        a.begin();
        a.end();
    }
};

template <typename T>
struct is_iterative {
public:
    static bool const value = match_concept_v<T, Iterative>;
};

template <class T>
inline auto get_itr_begin(T& ref)
{
    static_assert(is_iterative<T>::value, "types to zip must have begin() and end() as member functions.");
    return ref.begin();
}
template <class T>
inline auto get_itr_end(T& ref)
{
    static_assert(is_iterative<T>::value, "types to zip must have begin() and end() as member functions.");
    return ref.begin();
}

template <class T>
struct get_iterator {
    using type = decltype(std::declval<T>().begin());
};

template <class T>
using get_iterator_t = typename get_iterator<T>::type;

template <class Iterator>
struct iterator_value_type {
private:
    using type_from_std_traits = typename std::iterator_traits<Iterator>::value_type;

public:
    using type = std::conditional_t<
        std::is_same_v<type_from_std_traits, bool>,
        std::vector<bool>::reference,
        type_from_std_traits>;
};

template <class Iterator>
using iterator_value_type_t = typename iterator_value_type<Iterator>::type;

}  // namespace Grid::Impl
