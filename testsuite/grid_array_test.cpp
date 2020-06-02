#include <iostream>
#include <random>

#include <grid/linear.hpp>
#include <grid/src/grid_array/core.hpp>

int main()
{
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::normal_distribution<> dist_x(0.0, 0.3);
    std::normal_distribution<> dist_y(0.3, 0.4);

    Grid::GridArray<double, double, 10, 10> map{{-1.0, 1.0}, {-1.0, 1.0}};

    map.fill(0.0);

    for (auto& x : map.line<0>()) {
        for (auto& y : map.line<1>()) {
            map[x][y] = (double)int(10 * (x + y));
        }
    }

    Grid::GridArray<double, double, 10, 10> cp = map;

    for (auto& x : cp.line<0>()) {
        for (auto& y : cp.line<1>()) {
            cp[x][y] = (double)int(-10 * (x + y));
        }
    }

    for (auto& y : map.line<0>()) {
        for (auto& x : map.line<1>()) {
            std::cout << map[y][x] << ' ';
        }
        std::cout << std::endl;
    }


    std::cout << std::endl;
    for (auto& y : map.line<0>()) {
        for (auto& x : map.line<1>()) {
            std::cout << map[y][x] << ' ';
        }
        std::cout << std::endl;
    }

    /*
    for (std::size_t n = 0; n < 50000; ++n) {
        double x = dist_x(engine);
        double y = dist_y(engine);

        if (-1.0 < x && x < 1.0 && -1.0 < y && y < 1.0) {
            map.at(y, x) += 1.0;
        }
    }

    for (auto& y : map.line<0>()) {
        for (auto& x : map.line<1>()) {
            std::cout << x << ' ' << y << ' ' << map[y][x] << std::endl;
        }
        std::cout << std::endl;
    }
    */

    return 0;
}
