
#pragma once

#include <iostream>

#include "utility.h"

namespace print_lib {

/*!
 * \brief Prints IP address of string type
 *
 * \details This method outputs string value to stdout as is
 *
 * \tparam T - Type of IP address value
 * \param[in] s ip address value
 */
template<typename T, typename = std::enable_if_t<utility::is_cpp_string<T>::value>>
constexpr void _print_string(T &&s)
{
    std::cout << s << std::endl;
}

/*!
 * \brief Prints IP address of tuple type
 *
 * \details This method assures that value types are identical and recursively prints its elements to stdout
 * by applying corresponding lambda function for head element and tail elements
 *
 * \tparam T - Type of IP address value
 * \param[in] tp ip address value
 */
template<typename T, typename = std::enable_if_t<utility::is_tuple<T>::value>>
constexpr void _print_tuple(T &&tp)
{
    static_assert(utility::is_tuple_uniform_v<std::remove_reference_t<T>>, "Tuple should contain sequence of identical types");

    std::apply(
        [](const auto &head, const auto &... tail) {
            auto print_elem = [](const auto &v) { std::cout << "." << v; };
            std::cout << head;
            (print_elem(tail), ...);
        },
        tp);
    std::cout << std::endl;
}

/*!
 * \brief Prints IP address of container type
 *
 * \details This method assures that value type of the container is integer and then iterates over container type
 * to print values to stdout
 *
 * \tparam T - Type of IP address value
 * \param[in] c ip address value
 */
template<typename T, typename = std::enable_if_t<utility::is_container<T>::value>>
constexpr void _print_container(T &&c)
{
    static_assert(std::numeric_limits<typename std::remove_reference_t<T>::value_type>::is_integer, "Container should keep integer types");

    auto it = c.cbegin();
    std::cout << static_cast<int>(*it);
    for (it = std::next(c.cbegin()); it != c.cend(); ++it)
    {
        std::cout << '.' << static_cast<int>(*it);
    }
    std::cout << std::endl;
}

/*!
 * \brief Prints IP address of integer type
 *
 * \details This method splits integer type into array of bytes within function \ref utility::integer_to_array_of_bytes then
 * it prints output array in function for containter types \ref _print_container
 *
 * \tparam T - Type of IP address value
 * \param[in] i ip address value
 */
template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
constexpr void _print_integer(T &&i)
{
    const auto int_seq = utility::integer_to_array_of_bytes(i, false);
    _print_container(std::vector<typename std::decay_t<decltype(int_seq)>::value_type>{int_seq.cbegin(), int_seq.cend()});
}

/*!
 * \brief Print IP address with dot delimeter
 *
 * \details This method allows to print IP of certain types: integer, tuple, container std::list or std::vector, std::string.
 * Tuple should include only identical types.
 *
 * \tparam T - Type of IP address value
 * \param[in] v ip address value
 */
template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer || utility::is_tuple<T>::value ||
                                                 utility::is_container<T>::value || utility::is_cpp_string<T>::value>>
constexpr void print_ip(T &&v)
{
    if constexpr (std::numeric_limits<T>::is_integer)
    {
        _print_integer(std::forward<T>(v));
    }
    else if constexpr (utility::is_cpp_string<T>::value)
    {
        _print_string(std::forward<T>(v));
    }
    else if constexpr (utility::is_tuple<T>::value)
    {
        _print_tuple(std::forward<T>(v));
    }
    else if constexpr (utility::is_container<T>::value)
    {
        _print_container(std::forward<T>(v));
    }
}

} // namespace print_lib