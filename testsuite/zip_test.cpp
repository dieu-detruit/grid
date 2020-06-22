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

    std::vector<double> A(4);
    std::array<double, 100> B;
    std::deque<double> C(30);
    Grid::GridArray<bool, double, 300> D{{1.0, 2.0}};
    Grid::GridVector<double, double, 2> E{{-1.0, 1.0, 4}, {-1.0, 1.0, 4}};

    B.fill(0.0);

    for (int i = 0; i < 4; ++i) {
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

    std::cout << "--------" << std::endl;

    for (auto [x, y] : E.lines()) {
        std::cout << x << ", " << y << std::endl;
        E.at(x, y) = x - y + 1.0;
    }

    std::cout << "--------" << std::endl;

    for (auto& e : E) {
        std::cout << e << std::endl;
    }


    std::cout << "--------" << std::endl;

    for (auto [e, x, y] : Grid::zip(E, E.lines())) {
        std::cout << "(" << x << ", " << y << "): " << e << " (x + y = " << x - y + 1.0 << ")" << std::endl;
    }

    return 0;
}
