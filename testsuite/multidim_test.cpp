#include <iostream>

#include <grid/array.hpp>
#include <grid/vector.hpp>

int main()
{
    {  // array
        Grid::array<double, 3, 3, 3> vec{0.0};

        vec[0][1][1] = 5.0;
        vec[1][1][0] = 7.0;
        vec[2][1][2] = 3.141592;

        for (auto& v : vec) {
            std::cout << v << ' ';
        }
        std::cout << std::endl;
    }

    {  // array
        Grid::vector<double, 3> vec{3, 3, 3};

        vec[0][1][1] = 5.0;
        vec[1][1][0] = 7.0;
        vec[2][1][2] = 3.141592;

        for (auto& v : vec) {
            std::cout << v << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}

