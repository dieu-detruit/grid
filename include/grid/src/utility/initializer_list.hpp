#pragma once

#include <grid/src/utility/tag.hpp>
#include <grid/src/utility/tuple.hpp>

namespace Grid::Impl
{

template <template <class...> class type_to_initialize, class... types>
auto initializer_wrapper_impl(type_tag<std::tuple<types...>>)
{
    return type_tag<type_to_initialize<types...>>{};
}

template <std::size_t N, class T, template <class...> class type_to_initialize>
struct initializer_wrapper {
    using type = unwrap_type_tag_t<
        decltype(initializer_wrapper_impl<type_to_initialize>(
            type_tag<dynamic_tuple<T, N>>{}))>;
};

}  // namespace Grid::Impl
