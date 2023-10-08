#pragma once

#include <cstdint>

namespace mem_allocators {

template<typename T>
struct node
{
    T value;
    node *next{nullptr};
    node *prev{nullptr};
};

template<typename T>
struct list_iterator
{
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;
    using pointer = T *;
    using reference = T &;
    using self = list_iterator<T>;

    list_iterator() noexcept
        : m_node()
    {}
    list_iterator(node<T> *n) noexcept
        : m_node(n)
    {}
    self m_const_cast() const noexcept
    {
        return *this;
    }

    reference operator*() const noexcept
    {
        return m_node->value;
    }

    pointer operator->() const noexcept
    {
        return &m_node->value;
    }

    self &operator++() noexcept
    {
        m_node = m_node->next;
        return *this;
    }

    self &operator--() noexcept
    {
        m_node = m_node->prev;
        return *this;
    }

    friend constexpr bool operator==(const self &lhs, const self &rhs) noexcept
    {
        return lhs.m_node == rhs.m_node;
    }

    friend constexpr bool operator!=(const self &lhs, const self &rhs) noexcept
    {
        return lhs.m_node != rhs.m_node;
    }

    node<T> *m_node{nullptr};
};

template<typename T, typename Alloc = std::allocator<T>>
class list
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using allocator_type = typename Alloc::template rebind<node<T>>::other;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using difference_type = ptrdiff_t;
    using iterator = list_iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using pointer = typename list_iterator<T>::pointer;
    using reference = typename list_iterator<T>::reference;

    list()
    {
        _init();
    }

    ~list()
    {
        _clear();
    }

    static size_t distance(iterator first, iterator last)
    {
        return std::distance(first, last);
    }

    iterator begin() noexcept
    {
        return iterator(m_head->next);
    }

    iterator end() noexcept
    {
        return iterator(m_tail);
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    void push_back(const value_type &val)
    {
        auto *node = alloc_traits::allocate(m_alloc, 1);
        node->value = val;

        auto *prev_node = m_tail->prev;
        prev_node->next = node;
        node->next = m_tail;
        node->prev = prev_node;
        m_tail->prev = node;

        ++m_counter;
    }

    void push_front(const value_type &val)
    {
        auto *node = alloc_traits::allocate(m_alloc, 1);
        node->value = val;

        auto *next_node = m_head->next;
        next_node->prev = node;
        node->prev = m_head;
        node->next = next_node;
        m_head->next = node;

        ++m_counter;
    }

    void pop_back()
    {
        auto *node = m_tail->prev;
        if (node != m_head)
        {
            node->prev->next = m_tail;
            m_tail->prev = node->prev;

            alloc_traits::deallocate(m_alloc, node, 1);
            --m_counter;
        }
    }

    void pop_front()
    {
        auto *node = m_head->next;
        if (node != m_tail)
        {
            m_head->next = node->next;
            node->next->prev = m_head;

            alloc_traits::deallocate(m_alloc, node, 1);
            --m_counter;
        }
    }

    reference front() const noexcept
    {
        return *begin();
    }

    reference back() const noexcept
    {
        iterator tmp = end();
        --tmp;
        return *tmp;
    }

    bool empty() const noexcept
    {
        return m_counter == 0;
    }

    size_type size() const noexcept
    {
        return m_counter;
    }

    size_type max_size() const noexcept
    {
        return alloc_traits::max_size(m_alloc);
    }

private:
    void _init()
    {
        m_head = alloc_traits::allocate(m_alloc, 1);
        m_tail = alloc_traits::allocate(m_alloc, 1);
        m_head->next = m_tail;
        m_tail->prev = m_head;
    }

    void _clear()
    {
        auto node = m_head->next;
        for (; node != m_tail;)
        {
            if (!node)
            {
                break;
            }

            auto next_node = node->next;
            m_head->next = next_node;
            alloc_traits::deallocate(m_alloc, node, 1);

            node = next_node;
        }
        m_counter = 0u;

        alloc_traits::deallocate(m_alloc, m_tail, 1);
        alloc_traits::deallocate(m_alloc, m_head, 1);
    }

    allocator_type m_alloc;
    node<value_type> *m_head{nullptr};
    node<value_type> *m_tail{nullptr};
    size_type m_counter{0u};
};

} // namespace mem_allocators