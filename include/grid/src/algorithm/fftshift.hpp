#pragma once

#include <algorithm>
#include <execution>
#include <iterator>

#include <grid/core.hpp>
#include <strings.h>
#include <vector>

namespace Grid
{

// 1D fftshift
// swap left half and right half
template <typename value_type, typename measure_type, std::size_t N>
void fftshift(Grid::GridArray<value_type, measure_type, N>& vec)
{
    constexpr std::size_t left_size = (N % 2 == 0) ? N / 2 : N / 2 + 1;
    std::rotate(std::execution::par_unseq, vec.begin(), vec.begin() + left_size, vec.end());
}

template <class grid_vector, std::enable_if_t<Impl::is_grid_vector_v<grid_vector> and Impl::get_rank_v<grid_vector> == 1, std::nullptr_t> = nullptr>
void fftshift(grid_vector& vec)
{
    std::size_t N = vec.range(0).N;
    std::size_t left_size = (N % 2 == 0) ? N / 2 : N / 2 + 1;
    std::rotate(std::execution::par_unseq, vec.begin(), vec.begin() + left_size, vec.end());
}

// 2D fftshift
//
//  +---+---+      +---+---+
//  | 2 | 1 |      | 4 | 3 |
//  +---+---|  ->  +---+---+
//  | 3 | 4 |      | 1 | 2 |
//  +---+---+      +---+---+
//
//  if the number of rows / columns is odd, the center one is included in upper / left part
//

namespace Impl
{
template <class Iterator>
inline void fftshift_impl(Iterator begin, Iterator end,
    typename std::iterator_traits<Iterator>::difference_type N,
    typename std::iterator_traits<Iterator>::difference_type M)
{
    using index_t = typename std::iterator_traits<Iterator>::difference_type;

    if (N % 2 == 0) {
        // N = M = a even number

        const index_t half = N / 2;

        const index_t first_quadrant_offset = (N - 1) * half;
        const index_t second_quadrant_offset = (N + 1) * half;

        std::vector<Iterator> offset_list(half);
        for (index_t i = 0; i < half; ++i) {
            offset_list[i] = begin + N * i;
        }
        auto process = [&N, &first_quadrant_offset, &second_quadrant_offset, &half](auto& head) {
            for (auto it = head; it < head + half; ++it) {
                std::iter_swap(it, it + second_quadrant_offset);
            }
            for (auto it = head + half; it < head + N; ++it) {
                std::iter_swap(it, it + first_quadrant_offset);
            }
        };
        std::for_each(std::execution::par_unseq, offset_list.begin(), offset_list.end(), process);
    } else {
        // else
        // Recursive Juggling Algorithm
        // Rotate lines and elements in each line in this order
        //
        //     A    B
        //   +----+---+      +----+---+      +---+----+
        // C | 2  | 1 |      | 3  | 4 |      | 4 | 3  |
        //   |    |   |      +----+---+      +---+----+
        //   +----+---|  ->  | 2  | 1 |  ->  | 1 | 2  |
        // D | 3  | 4 |      |    |   |      |   |    |
        //   +----+---+      +----+---+      +---+----+

        const index_t B = N / 2, A = N - B;
        const index_t D = M / 2, C = M - D;

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
void fftshift(Grid::GridArray<value_type, measure_type, N, M>& vec)
{
    Impl::fftshift_impl(vec.begin(), vec.end(), N, M);
}

template <class grid_vector, std::enable_if_t<Impl::is_grid_vector_v<grid_vector> and Impl::get_rank_v<grid_vector> == 2, std::nullptr_t> = nullptr>
void fftshift(grid_vector& vec)
{
    using index_t = typename std::iterator_traits<decltype(vec.begin())>::difference_type;

    const index_t N = vec.range(0).N;
    const index_t M = vec.range(1).N;

    Impl::fftshift_impl(vec.begin(), vec.end(), N, M);
}

}  // namespace Grid
