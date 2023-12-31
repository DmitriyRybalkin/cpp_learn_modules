#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <utility>

namespace mem_allocators {

template<typename T, std::size_t CAPACITY>
struct dynamic_allocator
{
    using value_type = T;
    using size_type = std::size_t;

    dynamic_allocator()
    {
        pool = new value_type *[capacity];
    }

    ~dynamic_allocator()
    {
        for (auto i = 0u; i < pos; ++i)
        {
            delete pool[i];
            pool[i] = nullptr;
        }
        delete[] pool;
    }

    dynamic_allocator(const dynamic_allocator &other)
    {
        capacity = other.capacity;
        pos = other.pos;

        delete[] pool;
        pool = new value_type *[capacity];
        for (auto i = 0u; i < pos; ++i)
        {
            pool[i] = new value_type{*other.pool[i]};
        }
    }

    dynamic_allocator &operator=(const dynamic_allocator &other)
    {
        if (this != &other)
        {
            delete[] pool;

            capacity = other.capacity;
            pos = other.pos;

            pool = new value_type *[capacity];
            for (auto i = 0u; i < pos; ++i)
            {
                pool[i] = new value_type{*other.pool[i]};
            }
        }
        return *this;
    }

    T *allocate(size_type n, const void *hint = 0)
    {
        (void)hint;

        if (pos + n > capacity)
        {
            capacity *= 2;
            value_type **new_pool = new value_type *[capacity];
            std::copy(pool, pool + pos, new_pool);
            delete[] pool;
            pool = new_pool;
        }

        pool[pos] = new value_type;
        const auto cur = pos;
        pos += n;
        return pool[cur];
    }

    void deallocate(value_type *p, size_type)
    {
        for (auto i = 0u; i < pos; ++i)
        {
            if (pool[i] == p)
            {
                delete pool[i];
                for (auto j = i; j < pos; ++j)
                {
                    pool[j] = pool[j + 1];
                }
                pool[pos] = nullptr;
                --pos;
                break;
            }
        }
    }

    template<typename U, typename... Args>
    void construct(U *p, Args &&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U *p)
    {
        p->~U();
    }

    size_type max_size() const
    {
        return capacity;
    }

    size_type size() const
    {
        return pos;
    }

    template<class U>
    struct rebind
    {
        typedef dynamic_allocator<U, CAPACITY> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;

private:
    T **pool{nullptr};
    size_type capacity = CAPACITY;
    size_type pos = 0;
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