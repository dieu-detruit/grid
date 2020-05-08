#include <iostream>
#include <random>

#include <grid/src/grid/grid_array.hpp>

double round(double min, double max, double x)
{
    if (x < min) {
        return min;
    } else if (max < x) {
        return max;
    } else {
        return x;
    }
}

int main()
{
    std::random_device seed_gen;
    std::default_random_engine engine(seed_gen());

    std::normal_distribution<> dist_x(0.0, 0.3);
    std::normal_distribution<> dist_y(0.3, 0.4);

    Grid::GridArray<double, double, 100, 100> map{{-1.0, 1.0}, {-1.0, 1.0}};

    map.fill(0.0);

    for (std::size_t n = 0; n < 50000; ++n) {
        double x = dist_x(engine);
        double y = dist_y(engine);

        if (-1.0 < x && x < 1.0 && -1.0 < y && y < 1.0) {
            map.at(y, x) += 1.0;
        }
    }

    /*
    for (auto& v : map) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;
    */

    for (int i = 0; i < 100; ++i) {
        double y = 0.02 * i - 1.0;
        for (int j = 0; j < 100; ++j) {
            double x = 0.02 * j - 1.0;
            std::cout << x << ' ' << y << ' ' << map[y][x] << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
