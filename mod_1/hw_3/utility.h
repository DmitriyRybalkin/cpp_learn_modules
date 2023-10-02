#pragma once

#include <ostream>
#include <type_traits>
#include <utility>

namespace utility {

int fact(int val)
{
    if (val == 0)
        return 1;

    return val * fact(val - 1);
}

template<typename T>
struct is_pair : std::false_type
{};

template<typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type
{};

template<typename ContainerIterator>
void print_container(std::ostream &os, ContainerIterator beg, ContainerIterator end)
{
    for (; beg != end; ++beg)
    {
        if constexpr (is_pair<std::decay_t<decltype(*beg)>>::value)
        {
            os << beg->second << ' ';
        }
        else
        {
            os << *beg << ' ';
        }
    }
    os << std::endl;
}

} // namespace utility