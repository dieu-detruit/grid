#include <array>
#include <deque>
#include <iostream>
#include <random>
#include <vector>

#include <grid/bundle.hpp>
#include <grid/core.hpp>

template <class T>
struct what_is_T;

int main()
{

    std::vector<double> A(20);
    std::array<double, 100> B;
    std::deque<double> C(30);
    Grid::GridArray<bool, double, 300> D{{1.0, 2.0}};
    Grid::GridVector<int, double, 2> E{{-1.0, 1.0, 4}, {-1.0, 1.0, 4}};

    B.fill(0.0);

    for (int i = 0; i < 20; ++i) {
        A[i] = i;
    }

    for (auto [a, b, c, d] : Grid::zip(A, B, C, D)) {
        b = a + 1;
        c = a * 2;
        d = int(a) % 2 == 0;
    }

    for (auto [a, b, c, d] : Grid::zip(A, B, C, D)) {
        std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
    }

    // Multi Dimensional Zip test
    std::cout << "multi dimension ----" << std::endl;
    for (auto [a, e, f] : Grid::prod(A, std::array<int, 2>{{1, 2}}, std::array<int, 2>{{334, 335}})) {
        std::cout << a << ' ' << e << ' ' << f << std::endl;
    }

    return 0;
}
