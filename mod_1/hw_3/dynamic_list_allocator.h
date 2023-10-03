#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <utility>

#include "linked_list.h"

namespace mem_allocators {

template<typename T, std::size_t CAPACITY>
struct dynamic_list_allocator
{
    using value_type = T;
    using size_type = std::size_t;

    list<value_type> *pool{nullptr};
    size_type capacity = CAPACITY;
    size_type pos = 0;

    dynamic_list_allocator() 
    {
        pool = new list<value_type>();
        for(size_type i = 0u; i < capacity; ++i)
        {
            pool->add_node(value_type{});
        }
    }

    ~dynamic_list_allocator()
    {
        pool->clean();
        delete pool;
    }

    value_type *allocate(size_type n)
    {
        if (pos + n > capacity)
        {
            for (size_type i = 0u; i < capacity; ++i)
            {
                pool->add_node(value_type{});
            }
            capacity *= 2;
        }
        const auto cur = pos;
        pos += n;
        return pool->get(cur)->value;
    }

    void deallocate(value_type *p, size_type) {
        auto* remove_node = pool->find(p);
        if(remove_node) {
            const auto node_pos = remove_node->pos;
            pool->remove_node(node_pos);
        }
    }

    template<typename... Args>
    void construct(value_type *p, Args &&... args)
    {
        ::new ((void *)p) value_type(std::forward<Args>(args)...);
    }

    void destroy(value_type *p)
    {
        p->~value_type();
    }

    size_type max_size() const
    {
        return capacity;
    }

    template<class U>
    struct rebind
    {
        typedef dynamic_list_allocator<U, CAPACITY> other;
    };

    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
};

} // namespace mem_allocators