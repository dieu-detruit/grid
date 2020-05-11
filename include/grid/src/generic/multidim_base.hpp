#pragma once

#include <algorithm>

namespace Grid::Impl
{

template <class T, class _container_type, std::size_t _rank>
struct multidim_container_base {
public:
    using value_type = T;
    static constexpr std::size_t rank = _rank;
    using base_type = multidim_container_base<T, _container_type, _rank>;
    using container_type = _container_type;

protected:
    container_type _data;

public:
    template <class U, std::size_t n>
    friend struct multidim_proxy;

    // iterator diverted from std::vector
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }

    // constructor
    multidim_container_base() : _data{} {}
    multidim_container_base(container_type&& rdata) : _data{std::move(rdata)} {}
    multidim_container_base(const container_type& ldata) : _data{ldata} {}

    T* data()
    {
        return _data.data();
    }

    inline void fill(value_type val)
    {
        std::fill(_data.begin(), _data.end(), val);
    }

    inline virtual std::size_t dim_size(std::size_t dim) const = 0;
};


}  // namespace Grid::Impl
