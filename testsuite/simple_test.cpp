#include <iostream>
#include <random>
#include <complex>

#include <grid/core.hpp>
#include <grid/linear.hpp>

int main()
{
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::normal_distribution<> dist_x(0.0, 0.3);
    std::normal_distribution<> dist_y(0.3, 0.4);

    Grid::GridArrayd<std::complex<double>, 100, 100> map{{-1.0, 1.0}, {-1.0, 1.0}};

    map.fill(0.0);

    for (std::size_t n = 0; n < 50000; ++n) {
        double x = dist_x(engine);
        double y = dist_y(engine);

        if (-1.0 < x && x < 1.0 && -1.0 < y && y < 1.0) {
            map.at(y, x) += 1.0;
        }
    }

    for (auto& y : Grid::linspace(-1.0, 1.0, 100)) {
        for (auto& x : Grid::linspace(-1.0, 1.0, 100)) {
            std::cout << x << ' ' << y << ' ' << map[y][x] << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
