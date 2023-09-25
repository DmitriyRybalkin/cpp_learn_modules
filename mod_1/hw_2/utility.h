#pragma once

#include <array>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace utility {

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> res;
    res.reserve(str.size());

    decltype(str.size()) start = 0;
    decltype(str.size()) stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        res.emplace_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    res.emplace_back(str.substr(start));

    return res;
}

std::array<uint8_t, 4> parse_ip(const std::string &str)
{
    std::array<uint8_t, 4> res{0u, 0u, 0u, 0u};

    decltype(str.size()) start = 0;
    decltype(str.size()) stop = str.find_first_of('.');
    size_t idx{0u};

    while (stop != std::string::npos)
    {
        try
        {
            res.at(idx) = std::stoi(str.substr(start, stop - start));
            ++idx;
        }
        catch (const std::invalid_argument &)
        {
            std::cerr << __func__ << ": failed to convert string to decimal type!" << std::endl;
            return res;
        }
        catch (const std::out_of_range &)
        {
            std::cerr << __func__ << ": out of range error!" << std::endl;
            return res;
        }
        start = stop + 1;
        stop = str.find_first_of('.', start);
    }

    try
    {
        res.at(idx) = std::stoi(str.substr(start));
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << __func__ << ": failed to convert string to decimal type!" << std::endl;
    }
    catch (const std::out_of_range &)
    {
        std::cerr << __func__ << ": out of range error!" << std::endl;
    }
    return res;
}

template<typename InputItr1, typename InputItr2, typename CompFunc>
bool lex_compare(InputItr1 first1, InputItr1 last1, InputItr2 first2, InputItr2 last2, CompFunc cmpFunc)
{
    for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
    {
        // first check whether comparison is true for first iterators
        if (cmpFunc(*first1, *first2))
            return true;

        // flip around and check again
        if (cmpFunc(*first2, *first1))
            return false;
    }

    return (first1 == last1) && (first2 != last2);
}

template<typename Container>
bool numeric_comparator_greater(const Container &a, const Container &b)
{
    return lex_compare(a.begin(), a.end(), b.begin(), b.end(), std::greater<typename Container::value_type>());
}

template<typename Container>
bool numeric_comparator_less(const Container &a, const Container &b)
{
    return lex_compare(a.begin(), a.end(), b.begin(), b.end(), std::less<typename Container::value_type>());
}

template<typename T, typename Pred>
auto filter_copy_if(const std::vector<T> &vec, Pred p)
{
    std::vector<T> res;
    std::copy_if(begin(vec), end(vec), std::back_inserter(res), p);
    return res;
}

} // namespace utility