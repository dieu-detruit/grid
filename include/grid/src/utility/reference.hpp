#pragma once

#include <type_traits>
#include <vector>

namespace Grid::Impl
{

template <class src_type, class dst_type>
struct copy_ref_info {
    using type
        = std::conditional_t<
            /* cond */ std::is_lvalue_reference_v<src_type>,
            /* ? */ std::add_lvalue_reference_t<dst_type>,
            /* : */ std::conditional_t<
                /* cond */ std::is_lvalue_reference_v<src_type>,
                /* ? */ std::add_lvalue_reference_t<dst_type>,
                /* : */ std::remove_reference_t<dst_type>>>;
};
template <class src_type>
struct copy_ref_info<src_type, std::vector<bool>::reference> {
    using type = std::vector<bool>::reference;
};


template <class src_type, class dst_type>
using copy_ref_info_t = typename copy_ref_info<src_type, dst_type>::type;

}  // namespace Grid::Impl
