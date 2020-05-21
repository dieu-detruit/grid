#include <array>
#include <deque>
#include <iostream>
#include <random>
#include <vector>

#include <grid/core.hpp>
#include <grid/zip.hpp>

int main()
{

    std::vector<double> A(20);
    std::array<double, 100> B;
    std::deque<double> C(30);
    Grid::GridArray<double, double, 300> D{{1.0, 2.0}};

    for (int i = 0; i < 20; ++i) {
        A[i] = i;
    }

    for (auto [a, b, c, d] : Grid::zip(A, B, C, D)) {
        b = a + 1;
        c = a * 2;
        d = a / 2.0;
    }

    for (auto [a, b, c, d] : Grid::zip(A, B, C, D)) {
        std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
    }


    return 0;
}
