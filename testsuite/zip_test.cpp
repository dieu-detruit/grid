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
    // One Dimensional Zip test

    std::vector<double> A(20);
    std::array<double, 100> B;
    std::deque<double> C(30);
    Grid::GridArray<double, double, 300> D{{1.0, 2.0}};

    B.fill(0.0);

    for (int i = 0; i < 20; ++i) {
        A[i] = i;
    }

    for (auto [a, b, c, d] : Grid::zip(A, B, C, D)) {
        //what_is_T<decltype(b)> _;
        b = a + 1;
        c = a * 2;
        d = a / 2.0;
    }

    for (auto [a, b, c, d] : Grid::zip(A, B, C, D)) {
        std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
    }

    // Multi Dimensional Zip test
    std::cout << "multi dimension ----" << std::endl;
    for (auto [a, b, c] : Grid::prod(A, Grid::zip(B, C))) {
        std::cout << a << ' ' << b << ' ' << c << std::endl;
    }

    /*std::vector<int> E(5);
    std::vector<int> F(10);

    for (int i = 0; i < 5; ++i) {
        E[i] = i;
        F[i] = -i;
        F[i + 5] = -i - 5;
    }

    std::cout << "hoge" << std::endl;

    for (auto [e, f] : Grid::prod(E, F)) {
        std::cout << e << ' ' << f << std::endl;
    }
*/

    return 0;
}
