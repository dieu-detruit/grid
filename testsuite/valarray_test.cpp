#include <iostream>
#include <iterator>
#include <vector>

#include <grid/valarray.hpp>

int main()
{
    {  // array
        Grid::valarray<double, 3> vec{3, 3, 3};

        vec[0][1][1] = 5.0;
        vec[1][1][0] = 7.0;
        vec[2][1][2] = 3.141592;

        for (auto& v : vec) {
            std::cout << v << ' ';
        }
        std::cout << std::endl;

        vec *= 10.0;
        for (auto& v : vec) {
            std::cout << v << ' ';
        }

        std::cout << std::endl;
    }

    return 0;
}
