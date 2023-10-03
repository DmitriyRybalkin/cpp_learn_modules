
#include <iostream>
#include <cstdlib>
#include <map>
#include <memory>

#include "utility.h"
#include "static_allocator.h"
#include "dynamic_allocator.h"
#include "dynamic_list_allocator.h"

static constexpr std::size_t test_map_size = 10u;

using namespace mem_allocators;

int main()
{
    // task 1
    std::cout << "*** task 1 ***" << std::endl;
    auto test_map_1 = std::map<int, int, std::less<int>>{};
    for (auto i = 0u; i < test_map_size; ++i)
    {
        test_map_1[i] = utility::fact(i);
    }
    utility::print_container(std::cout, test_map_1.cbegin(), test_map_1.cend());

    // task 2
    std::cout << "*** task 2 ***" << std::endl;
    auto test_map_2 = std::map<int, int, std::less<int>, static_allocator<std::pair<const int, int>, test_map_size>>{};
    for (auto i = 0u; i < test_map_size; ++i)
    {
        test_map_2[i] = utility::fact(i);
    }
    utility::print_container(std::cout, test_map_2.cbegin(), test_map_2.cend());

    // task 3.1
    std::cout << "*** task 3.1 ***" << std::endl;
    auto test_map_3 = std::map<int, int, std::less<int>, dynamic_allocator<std::pair<const int, int>, test_map_size>>{};
    std::cout << "test map #3.1::max size: " << test_map_3.max_size() << std::endl;
    for (auto i = 0u; i < test_map_size + 1; ++i)
    {
        test_map_3[i] = utility::fact(i);
    }
    std::cout << "test map #3.1::max size: " << test_map_3.max_size() << std::endl;

    test_map_3.erase(5);
    test_map_3.insert(std::make_pair(13, utility::fact(13)));

    utility::print_container(std::cout, test_map_3.cbegin(), test_map_3.cend());

    // task 3.2
    std::cout << "*** task 3.2 ***" << std::endl;
    auto test_map_3_2 = std::map<int, int, std::less<int>, dynamic_list_allocator<std::pair<const int, int>, test_map_size>>{};
    std::cout << "test map #3.2::max size: " << test_map_3_2.max_size() << std::endl;
    for (auto i = 0u; i < test_map_size + 1; ++i)
    {
        test_map_3_2[i] = utility::fact(i);
    }
    std::cout << "test map #3.2::max size: " << test_map_3_2.max_size() << std::endl;

    test_map_3_2.erase(5);
    test_map_3_2.insert(std::make_pair(5, utility::fact(5)));
    test_map_3_2.erase(7);

    utility::print_container(std::cout, test_map_3_2.cbegin(), test_map_3_2.cend());

    return 0;
}