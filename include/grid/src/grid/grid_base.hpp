#pragma once

namespace Grid
{

template <class value_type, class measure_type, std::size_t rank, class data_type>
class GridBase
{
protected:
    data_type data;

public:
    using base_type = GridBase<value_type, measure_type, rank, data_type>;

    using value_t = value_type;
    using measure_t = measure_type;

    using iterator = typename data_type::iterator;
    using const_iterator = typename data_type::const_iterator;

    iterator begin() { return this->data.begin(); }
    const_iterator begin() const { return this->data.begin(); }
    iterator end() { return this->data.end(); }
    const_iterator end() const { return this->data.end(); }

    template <class... Args>
    GridBase(Args... args) : data(args...)
    {
    }

    inline virtual void fill(value_type value)
    {
        data.fill(value);
    }
};

}  // namespace Grid
