#pragma once

#include <algorithm>
#include <execution>
#include <iterator>


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

    // iterator diverted from STL container
    using iterator = decltype(std::begin(_data));
    using const_iterator = decltype(std::cbegin(_data));


    iterator begin()
    {
        return std::begin(_data);
    }
    const_iterator begin() const
    {
        return std::cbegin(_data);
    }
    iterator end()
    {
        return std::end(_data);
    }
    const_iterator end() const
    {
        return std::cend(_data);
    }

    // constructor
    multidim_container_base() : _data{} {}
    multidim_container_base(container_type&& rdata) : _data{std::move(rdata)} {}
    multidim_container_base(const container_type& ldata) : _data{ldata} {}

    T* data()
    {
        return _data.data();
    }
    const T* data() const
    {
        return _data.data();
    }
    std::size_t size() const
    {
        return _data.size();
    }

    inline void swap(container_type& x)
    {
        _data.swap(x);
    }

    inline void fill(value_type val)
    {
        std::fill(std::execution::par_unseq, std::begin(_data), std::end(_data), val);
    }

    inline virtual std::size_t dim_size(std::size_t dim) const = 0;
};


}  // namespace Grid::Impl
