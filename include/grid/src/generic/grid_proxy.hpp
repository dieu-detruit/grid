#pragma once

namespace Grid
{

template <class grid_type, class container_proxy_type, class range_tuple, std::size_t rank_rest>
class GridProxy
{
    container_proxy_type proxy;
    const range_tuple& ranges;

public:
    GridProxy(container_proxy_type proxy, const range_tuple& ranges)
        : proxy(proxy), ranges(ranges) {}

    decltype(auto) operator[](typename container_proxy_type::value_type subscript)
    {
        if constexpr (rank_rest == 1) {
            return proxy[std::get<std::tuple_size<range_tuple>::value - 1>(ranges).quantize(subscript)];
        } else {
            return GridProxy<grid_type, decltype(proxy[0]), range_tuple, rank_rest - 1>{
                proxy[std::get<std::tuple_size<range_tuple>::value - rank_rest>(ranges).quantize(subscript)],
                ranges};
        }
    }
};

}  // namespace Grid

