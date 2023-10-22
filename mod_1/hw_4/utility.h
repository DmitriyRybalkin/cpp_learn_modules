#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <list>
#include <tuple>
#include <type_traits>
#include <string>
#include <utility>
#include <vector>

namespace utility {
template<typename>
struct is_tuple : std::false_type
{};

template<typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type
{};

template<typename H, typename... T>
struct is_tuple_uniform
{
    static constexpr bool value = std::is_same_v<std::tuple<H, T...>, std::tuple<T..., H>>;
};

template<typename... T>
struct is_tuple_uniform<std::tuple<T...>> : is_tuple_uniform<T...>
{};

template<typename... T>
constexpr bool is_tuple_uniform_v = is_tuple_uniform<T...>::value;

template<typename T>
struct is_container : std::false_type
{};

template<typename T, typename Alloc>
struct is_container<std::vector<T, Alloc>> : std::true_type
{};

template<typename T, typename Alloc>
struct is_container<std::list<T, Alloc>> : std::true_type
{};

template<typename T>
struct is_cpp_string : std::false_type
{};

template<typename T, typename Traits, typename Alloc>
struct is_cpp_string<std::basic_string<T, Traits, Alloc>> : std::true_type
{};

/*!
 * \brief convert from integer to corresponding size std::array
 *
 * \tparam T - any integer type
 * \param[in] source integer value
 * \param[in] little_endian - set to false, if input value should be converted to big endian array
 * \return std::array<uint8_t, sizeof(T)>
 */
template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
constexpr std::array<uint8_t, sizeof(T)> integer_to_array_of_bytes(T source, bool little_endian = true)
{
    std::array<uint8_t, sizeof(T)> output;

    auto int_source = static_cast<T>(source);
    auto setter = [&int_source](auto &entry) -> void {
        entry = static_cast<uint8_t>(int_source);
        int_source >>= 8;
    };

    if (little_endian)
    {
        static_cast<void>(std::for_each(output.begin(), output.end(), setter));
    }
    else
    {
        static_cast<void>(std::for_each(output.rbegin(), output.rend(), setter));
    }

    return output;
}

} // namespace utility