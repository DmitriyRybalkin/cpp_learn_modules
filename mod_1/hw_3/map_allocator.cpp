
#include <iostream>
#include <cstdlib>
#include <map>
#include <memory>

#include "utility.h"

static constexpr std::size_t test_map_size = 10u;

template<typename T, std::size_t CAPACITY>
struct map_allocator
{
    using value_type = T;

    const size_t capacity = CAPACITY;
    size_t pos = 0;
    std::shared_ptr<T> pool = nullptr;

    map_allocator() noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        pool.reset(new T[capacity]);
    }

    template<class U>
    map_allocator(const map_allocator<U, CAPACITY> &a) noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        pool = a.pool;
        capacity = a.capacity;
        pos = a.pos;
    }

    map_allocator select_on_container_copy_construction() const
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return map_allocator();
    }

    T *allocate(std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if (pos + n > capacity)
            throw std::bad_alloc();

        int cur = pos;
        pos += n;
        return pool.get() + cur;
    }

    void deallocate(T *, std::size_t)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    template<class Up, class... Args>
    void construct(Up *p, Args &&... args)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        ::new ((void *)p) Up(std::forward<Args>(args)...);
    }

    void destroy(T *p)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        p->~T();
    }

    template<class U>
    struct rebind
    {
        typedef map_allocator<U, CAPACITY> other;
    };

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
};

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator==(const map_allocator<T, CAPACITY> &a1, const map_allocator<U, CAPACITY> &a2) noexcept
{
    return a1.pool == a2.pool && a1.capacity == a2.capacity;
}

template<typename T, typename U, std::size_t CAPACITY>
constexpr bool operator!=(const map_allocator<T, CAPACITY> &a1, const map_allocator<U, CAPACITY> &a2) noexcept
{
    return a1.pool != a2.pool && a1.capacity != a2.capacity;
}

int main()
{
    auto test_map = std::map<int, int, std::less<int>, map_allocator<std::pair<int, int>, test_map_size>>{};

    for (auto i = 0u; i < test_map_size; ++i)
    {
        test_map[i] = utility::fact(i);
    }

    std::cout << "map size: " << test_map.size() << std::endl;
    for (auto &&map_pair : test_map)
    {
        std::cout << map_pair.second << ' ';
    }
    std::cout << std::endl;

    return 0;
}