#pragma once

#include <stdexcept>
#include <string>

namespace Grid
{

using out_of_bound = std::out_of_range;

struct invalid_dimension : public std::invalid_argument {
    explicit invalid_dimension(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit invalid_dimension(const char* what_arg) : std::invalid_argument(what_arg) {}
};

}  // namespace Grid
