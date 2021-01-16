#include <cassert>
#include <cmath>

#include <grid/linear.hpp>

int main()
{

    // -1.0, -0.5, 0.0, 0.5, 1.0
    auto x = Grid::linspace(-1.0, 1.0, 5);
    auto y = Grid::arange(-1.0, 1.0, 0.5);

    double v = -1.0;
    for (int i = 0; i < 5; v += 0.5, ++i) {
        assert(std::abs(x[i] - v) < 1.0e-10);
        assert(std::abs(y[i] - v) < 1.0e-10);
    }

    return 0;
}
