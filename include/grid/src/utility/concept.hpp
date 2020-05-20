#pragma once

#include <utility>

namespace Grid::Impl
{

template <typename T, template <class> class Concept>
struct match_concept {
private:
    static auto check(T) -> decltype(std::declval<Concept<T>>().constraint(), std::true_type{});
    static std::false_type check(...);

public:
    static constexpr bool value = decltype(check(std::declval<T>()))::value;
};

template <typename T, template <class> class Concept>
inline constexpr bool match_concept_v = match_concept<T, Concept>::value;

}  // namespace Grid::Impl
