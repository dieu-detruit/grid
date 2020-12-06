#pragma once

#include <algorithm>
#include <execution>
#include <iterator>
#include <vector>

#include <grid/core.hpp>
#include <grid/src/algorithm/fftshift.hpp>

namespace Grid
{

// 1D ifftshift
// inverse fftshift result
template <typename value_type, typename measure_type, std::size_t N>
void ifftshift(Grid::GridArray<value_type, measure_type, N>& vec)
{
    constexpr std::size_t left_size = N / 2;
    std::rotate(std::execution::par_unseq, vec.begin(), vec.begin() + left_size, vec.end());
}

template <class grid_vector, std::enable_if_t<Impl::is_grid_vector_v<grid_vector> and Impl::get_rank_v<grid_vector> == 1, std::nullptr_t> = nullptr>
void ifftshift(grid_vector& vec)
{
    std::size_t N = vec.range(0).N;
    std::size_t left_size = N / 2;
    std::rotate(std::execution::par_unseq, vec.begin(), vec.begin() + left_size, vec.end());
}

// 2D ifftshift
//
//  +---+---+      +---+---+
//  | 2 | 1 |      | 4 | 3 |
//  +---+---|  ->  +---+---+
//  | 3 | 4 |      | 1 | 2 |
//  +---+---+      +---+---+
//
//  inverse fftshift result
//  if the number of rows / columns is odd, the center one is included in "lower / right" part
//
namespace Impl
{
template <class Iterator>
inline void ifftshift_impl(Iterator begin, Iterator end,
    typename std::iterator_traits<Iterator>::difference_type N,
    typename std::iterator_traits<Iterator>::difference_type M)
{
    using index_t = typename std::iterator_traits<Iterator>::difference_type;

    if (N % 2 == 0) {
        // N = M = a even number -> identical to fftshift
        Impl::fftshift_impl(begin, end, N, M);

    } else {
        // else
        // Recursive Juggling Algorithm
        // Rotate lines and elements in each line in this order

        const index_t A = N / 2, B = N - A;
        const index_t C = M / 2, D = M - C;

        // Rotate lines
        const index_t g = std::gcd(N, A);
        const index_t offset = A * M;

        std::vector<Iterator> lines_to_juggle(g);
        for (index_t i = 0; i < g; ++i) {
            lines_to_juggle[i] = begin + M * i;
        }

        auto process = [&N, &M, &g, &offset, &end](auto line) {
            for (index_t k = 0; k < N / g - 1; ++k) {
                index_t mod_offset = (line + offset < end) ? offset : -M * N + offset;
                for (auto it = line; it < line + M; ++it) {
                    std::iter_swap(it, it + mod_offset);
                }
                line += mod_offset;
            }
        };
        std::for_each(std::execution::par_unseq, lines_to_juggle.begin(), lines_to_juggle.end(), process);
        auto line_head = begin;
        for (index_t n = 0; n < N; ++n) {
            std::rotate(std::execution::par_unseq, line_head, line_head + C, line_head + M);
            line_head += M;
        }
    }
}
}  // namespace Impl

template <typename value_type, typename measure_type, std::size_t N, std::size_t M>
void ifftshift(Grid::GridArray<value_type, measure_type, N, M>& vec)
{
    Impl::ifftshift_impl(vec.begin(), vec.end(), N, M);
}

template <class grid_vector, std::enable_if_t<Impl::is_grid_vector_v<grid_vector> and Impl::get_rank_v<grid_vector> == 2, std::nullptr_t> = nullptr>
void ifftshift(grid_vector& vec)
{
    using index_t = typename std::iterator_traits<decltype(vec.begin())>::difference_type;

    const index_t N = vec.range(0).N;
    const index_t M = vec.range(1).N;

    Impl::ifftshift_impl(vec.begin(), vec.end(), N, M);
}

}  // namespace Grid
