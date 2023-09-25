#pragma once

#include <sstream>

#include "user_types.h"
#include "utility.h"

namespace ip_filter {
std::ostream &operator<<(std::ostream &os, const user_types::ip_pool_t &ip_pool)
{
    for (auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for (auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                os << ".";
            }
            os << static_cast<int>(*ip_part);
        }
        os << std::endl;
    }
    return os;
}

template<typename ContainerIterator>
void sort__desc_by_lex_compare(ContainerIterator beg, ContainerIterator end)
{
    std::sort(beg, end, utility::numeric_comparator_greater<typename ContainerIterator::value_type>);
}

template<typename ContainerIterator>
void sort__asc_by_lex_compare(ContainerIterator beg, ContainerIterator end)
{
    std::sort(beg, end, utility::numeric_comparator_less<typename ContainerIterator::value_type>);
}

user_types::ip_pool_t filter(const user_types::ip_pool_t &ip_pool, const user_types::ip_t &mask)
{
    return utility::filter_copy_if(ip_pool, [&mask](const user_types::ip_t &ip) {
        for (size_t idx_offset = mask.size() - 1; idx_offset != 0; --idx_offset)
        {
            if (std::all_of(mask.cbegin(), mask.cbegin() + idx_offset, [](const auto val) { return val != 0u; }))
            {
                return std::equal(ip.cbegin(), ip.cbegin() + idx_offset, mask.cbegin(), mask.cbegin() + idx_offset);
            }
        }

        if (std::any_of(mask.cbegin(), mask.cend(), [](const auto val) { return val != 0u; }))
        {
            const auto it = std::find_if(mask.cbegin(), mask.cend(), [](const auto val) { return val != 0u; });
            if (it != mask.cend())
            {
                const auto it_idx = std::distance(mask.cbegin(), it);
                return ip.at(it_idx) == *it;
            }
            else
            {
                return false;
            }
        }

        return false;
    });
}

user_types::ip_pool_t filter_any(const user_types::ip_pool_t &ip_pool, const user_types::ip_t::value_type mask)
{
    return utility::filter_copy_if(ip_pool, [&mask](const user_types::ip_t &ip) {
        const auto it = std::find_if(ip.cbegin(), ip.cend(), [&mask](const auto val) { return val == mask; });
        return it != ip.cend();
    });
}
} // namespace ip_filter
