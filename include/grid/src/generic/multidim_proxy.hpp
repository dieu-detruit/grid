#pragma once

namespace Grid::Impl
{

template <class multidim_type, std::size_t rank_rest>
class multidim_proxy
{
    multidim_type& ref;
    std::size_t index_proxy;

public:
    using value_type = typename multidim_type::value_type;

    multidim_proxy(multidim_type& ref, std::size_t index_proxy)
        : ref(ref), index_proxy(index_proxy) {}

    multidim_proxy(const multidim_proxy& l)
        : ref(l.ref), index_proxy(l.index_proxy) {}

    decltype(auto) operator[](std::size_t subscript)
    {
        if constexpr (rank_rest == 1) {
            return ref.data[index_proxy + subscript];
        } else {
            return multidim_proxy<multidim_type, rank_rest - 1>{
                ref,
                ref.template dim_size<multidim_type::rank - rank_rest + 1>() * (index_proxy + subscript)};
        }
    }
};

}  // namespace Grid::Impl
