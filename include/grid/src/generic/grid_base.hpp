#pragma once

#include <iterator>
#include <numeric>

namespace Grid
{

template <class value_type, class measure_type, std::size_t rank, class data_type>
class GridBase
{
protected:
    data_type _data;

    using container_type = typename data_type::container_type;

public:
    using base_type = GridBase<value_type, measure_type, rank, data_type>;

    using value_t = value_type;
    using measure_t = measure_type;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    iterator begin() { return std::begin(_data); }
    const_iterator begin() const { return std::begin(_data); }
    iterator end() { return std::end(_data); }
    const_iterator end() const { return std::end(_data); }

    template <class... Args>
    GridBase(Args... args) : _data(args...)
    {
    }

    value_type* data()
    {
        return _data.data();
    }
    std::size_t size()
    {
        return _data.size();
    }

    void swap(container_type& x)
    {
        _data.swap(x);
    }

    inline virtual void fill(value_type value)
    {
        _data.fill(value);
    }
};

}  // namespace Grid
