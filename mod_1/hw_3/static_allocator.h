#pragma once

#include <cstdint>
#include <utility>
#include <memory>

namespace mem_allocators {

template<typename T, std::size_t CAPACITY>
struct static_allocator
{
    using value_type = T;

    static constexpr std::size_t capacity = CAPACITY;
    static std::size_t pos;
    static T pool[capacity];

    static_allocator() = default;
    ~static_allocator() = default;

    static_allocator select_on_container_copy_construction() const
    {
        return static_allocator();
    }

    T *allocate(std::size_t n)
    {
        if (pos + n > capacity)
            throw std::bad_alloc();

        int cur = pos;
        pos += n;
        return reinterpret_cast<T *>(pool) + cur;
    }

    void deallocate(T *, std::size_t) {}

    template<class Up, class... Args>
    void construct(Up *p, Args &&... args)
    {
        ::new ((void *)p) Up(std::forward<Args>(args)...);
    }

    void destroy(T *p)
    {
        p->~T();
    }

    template<class U>
    struct rebind
    {
        typedef static_allocator<U, CAPACITY> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
};

template<typename T, std::size_t CAPACITY>
T static_allocator<T, CAPACITY>::pool[capacity];

template<typename T, std::size_t CAPACITY>
std::size_t static_allocator<T, CAPACITY>::pos = 0;

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator==(const static_allocator<T, CAPACITY> &a1, const static_allocator<U, CAPACITY> &a2) noexcept
{
    return true;
}

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator!=(const static_allocator<T, CAPACITY> &a1, const static_allocator<U, CAPACITY> &a2) noexcept
{
    return false;
}

} // namespace mem_allocators