#pragma once

#include <cstdint>

namespace mem_allocators {

template<typename T>
class list
{
private:
    struct node;

public:
    list()
    {
        head = new node();
    }
    ~list()
    {
        clean();

        delete head;
    }

    void clean()
    {
        auto node = head->next;
        for (;;)
        {
            if (!node)
            {
                break;
            }

            auto next_node = node->next;
            head->next = next_node;
            delete node;

            node = next_node;
        }
        counter = 0u;
    }

    void add_node(T&& value)
    {
        auto new_node = new node();
        new_node->value = new T(value);
        new_node->pos = counter;
        new_node->next = head->next;
        head->next = new_node;

        ++counter;
    }

    bool remove_node(std::size_t idx)
    {
        bool res{false};
        decltype(head) prev_node{nullptr}, node_to_remove{nullptr};
        for (auto node = head->next; node != nullptr; node = node->next)
        {
            if (node->pos == idx)
            {
                node_to_remove = node;
                break;
            }
            prev_node = node;
        }

        if (!node_to_remove)
        {
            return res;
        }

        if (prev_node)
        {
            prev_node->next = node_to_remove->next;
        }
        else
        {
            head->next = node_to_remove->next;
        }

        delete node_to_remove;
        res = true;
        --counter;

        return res;
    }

    node* find(T* value) const
    {
        for (auto node = head->next; node != nullptr; node = node->next)
        {
            if (node->value == value)
            {
                return node;
            }
        }

        return nullptr;
    }

    node* get(std::size_t idx) const
    {
        for (auto node = head->next; node != nullptr; node = node->next)
        {
            if (node->pos == idx)
            {
                return node;
            }
        }

        return nullptr;
    }

private:
    node *head{nullptr};
    std::size_t counter{0u};
};

template<typename T>
struct list<T>::node
{
    friend class list<T>;

    ~node()
    {
        delete value;
    }

    T* value{nullptr};
    std::size_t pos;

private:
    node *next{nullptr};
};

} // namespace mem_allocators