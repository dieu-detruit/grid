#include <iostream>
#include <random>

#include <grid/bundle.hpp>
#include <grid/linear.hpp>
#include <grid/src/grid_vector/core.hpp>

int main()
{
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::normal_distribution<> dist_x(0.0, 0.3);
    std::normal_distribution<> dist_y(0.3, 0.4);

    Grid::GridVector<double, double, 2> map{{-1.0, 1.0, 100}, {-1.0, 1.0, 100}};

    map.fill(0.0);

    for (std::size_t n = 0; n < 10000; ++n) {
        double x = dist_x(engine);
        double y = dist_y(engine);

        if (-1.0 < x && x < 1.0 && -1.0 < y && y < 1.0) {
            map.at(x, y) += 1.0;
        }
    }


    /*
    for (auto [x, y, m] : Grid::zip(map.lines(), map)) {
        std::cout << x << ' ' << y << ' ' << m << std::endl;
    }
    */

    return 0;
}
