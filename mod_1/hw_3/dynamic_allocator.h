#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <utility>

#include "utility.h"

namespace mem_allocators {

template<typename T, std::size_t CAPACITY>
struct dynamic_allocator
{
    using value_type = T;
    using size_type = std::size_t;

    T *pool{nullptr};
    size_type capacity = CAPACITY;
    size_type size = 0;
    size_type pos = 0;

    dynamic_allocator()
    {
        pool = static_cast<T *>(std::malloc(capacity * sizeof(T)));
    }

    ~dynamic_allocator()
    {
        free(pool);
    }

    T *allocate(size_type n, const void *hint = 0)
    {
        (void)hint;

        if (pos + n > capacity)
        {
            capacity *= 2;
            pool = static_cast<T *>(std::realloc(pool, capacity * sizeof(T)));
        }
        const auto cur = pos;
        pos += n;
        return pool + cur;
    }

    void deallocate(T *p, size_type)
    {
        for (size_type i = 0u; i < capacity; ++i)
        {
            T *it = pool + i;
            if (it == p)
            {
                std::cout << __func__ << ": ptr pos = " << i << std::endl;

                --size;
                break;
            }
        }
    }

    void construct(T *p, const T &value)
    {
        _construct(p, value);
        ++size;
    }

    void destroy(T *p)
    {
        _destroy(p);
    }

    size_type max_size() const
    {
        return capacity;
    }

    template<class U>
    struct rebind
    {
        typedef dynamic_allocator<U, CAPACITY> other;
    };
};

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator==(const dynamic_allocator<T, CAPACITY> &a1, const dynamic_allocator<U, CAPACITY> &a2) noexcept
{
    return a1.pool == a2.pool && a1.capacity == a2.capacity && a1.pos == a2.pos;
}

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator!=(const dynamic_allocator<T, CAPACITY> &a1, const dynamic_allocator<U, CAPACITY> &a2) noexcept
{
    return a1.pool != a2.pool && a1.capacity != a2.capacity && a1.pos != a2.pos;
}

} // namespace mem_allocators