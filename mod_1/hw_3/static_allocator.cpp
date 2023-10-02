
#include "static_allocator.h"

namespace mem_allocators {

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator==(const static_allocator<T, CAPACITY> &a1, const static_allocator<U, CAPACITY> &a2) noexcept
{
    return a1.capacity == a2.capacity;
}

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator!=(const static_allocator<T, CAPACITY> &a1, const static_allocator<U, CAPACITY> &a2) noexcept
{
    return a1.capacity != a2.capacity;
}

} // namespace mem_allocators