#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include <grid/src/utility/type_sequence.hpp>

namespace Grid::Impl
{

// variant from tuple
template <class... types>
auto tuple_to_variant_impl(type_tag<std::tuple<types...>>)
{
    return type_tag<std::variant<types...>>{};
}
template <class Tuple>
struct tuple_to_variant {
    using type = typename decltype(tuple_to_variant_impl(type_tag<Tuple>{}))::type;
};
template <class Tuple>
using tuple_to_variant_t = typename tuple_to_variant<Tuple>::type;

// type sequence to variant
template <class sequence_tag>
struct sequence_to_variant : public sequencial_specialize<std::variant, sequence_tag> {
};
template <class sequence_tag>
using sequence_to_variant_t = typename sequence_to_variant<sequence_tag>::type;

// variant which has no coupled type element
template <class Variant>
struct unique_variant : public unique_specialization<std::variant, Variant> {
};
template <class Variant>
using unique_variant_t = typename unique_variant<Variant>::type;

}  // namespace Grid::Impl
