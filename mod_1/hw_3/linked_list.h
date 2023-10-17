#pragma once

#include <cstdint>
#include <memory>

namespace mem_allocators {

template<typename T>
struct node
{
    node() = default;

    node(const T &v)
        : value{v} {};

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

    list(list& l) : m_alloc(l.m_alloc)
    {
        _init();
        _initialize_dispatch(l.begin(), l.end());
    }

    list& operator=(list& l) 
    {
        if(this != &l)
        {
            _clear();
            _init();
            _initialize_dispatch(l.begin(), l.end());
        }
        return *this;
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

    template<typename... Args>
    void emplace_back(Args &&... args)
    {
        _insert(end(), std::forward<Args>(args)...);
    }

    void push_back(const value_type &val)
    {
        _insert(end(), val);
    }

    void pop_back()
    {
        _erase(iterator(m_tail->prev));
    }

    template<typename... Args>
    void emplace_front(Args &&... args)
    {
        _insert(begin(), std::forward<Args>(args)...);
    }

    void push_front(const value_type &val)
    {
        _insert(begin(), val);
    }

    void pop_front()
    {
        _erase(begin());
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
            alloc_traits::destroy(m_alloc, node);
            alloc_traits::deallocate(m_alloc, node, 1);

            node = next_node;
        }
        m_counter = 0u;

        alloc_traits::deallocate(m_alloc, m_tail, 1);
        alloc_traits::deallocate(m_alloc, m_head, 1);
    }

    template<typename... Args>
    node<value_type> *_create_node(Args &&... args)
    {
        auto *node = alloc_traits::allocate(m_alloc, 1);
        alloc_traits::construct(m_alloc, node, std::forward<Args>(args)...);
        return node;
    }

    template<typename... Args>
    void _insert(iterator position, Args &&... args)
    {
        auto *tmp = _create_node(std::forward<Args>(args)...);
        _hook(position, tmp);
        ++m_counter;
    }

    void _erase(iterator position)
    {
        _unhook(position);
        auto *node = position.m_node;

        alloc_traits::destroy(m_alloc, node);
        alloc_traits::deallocate(m_alloc, node, 1);
        --m_counter;
    }

    void _hook(iterator position, node<value_type> *node) noexcept
    {
        node->next = position.m_node;
        node->prev = position.m_node->prev;
        position.m_node->prev->next = node;
        position.m_node->prev = node;
    }

    void _unhook(iterator position) noexcept
    {
        auto *next_node = position.m_node->next;
        auto *prev_node = position.m_node->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }

    void _initialize_dispatch(iterator begin, iterator end)
    {
        for(; begin != end; ++begin)
        {
            emplace_back(*begin);
        }
    }

    allocator_type m_alloc;
    node<value_type> *m_head{nullptr};
    node<value_type> *m_tail{nullptr};
    size_type m_counter{0u};
};

} // namespace mem_allocators