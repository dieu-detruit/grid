#include <iostream>

#include <grid/src/multi_dimension_array.hpp>

int main()
{

    Grid::Impl::multi_dim_array<double, 4, 3> vec{0.0};

    vec[2][1] = 3.141592;

    for (auto& v : vec) {
        std::cout << v << ' ';
    }
    std::cout << std::endl;

    return 0;
}

