
#include <iostream>
#include <cstdlib>
#include <map>
#include <memory>

#include "utility.h"
#include "static_allocator.h"
#include "dynamic_allocator.h"
#include "linked_list.h"

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
    std::cout << "*** task 3 ***" << std::endl;
    auto test_map_3 = std::map<int, int, std::less<int>, dynamic_allocator<std::pair<const int, int>, test_map_size>>{};
    std::cout << "test map #3::max size: " << test_map_3.max_size() << std::endl;
    for (auto i = 0u; i < test_map_size + 1; ++i)
    {
        test_map_3[i] = utility::fact(i);
    }
    std::cout << "test map #3::max size: " << test_map_3.max_size() << std::endl;

    test_map_3.erase(5);
    test_map_3.insert(std::make_pair(13, utility::fact(12)));
    test_map_3.erase(8);

    utility::print_container(std::cout, test_map_3.cbegin(), test_map_3.cend());

    // task 4
    std::cout << "*** task 4 ***" << std::endl;
    auto test_list = list<int>{};
    for (auto i = 0u; i < test_map_size; ++i)
    {
        test_list.push_back(i);
    }

    utility::print_container(std::cout, test_list.begin(), test_list.end());

    // task 5
    std::cout << "*** task 5 ***" << std::endl;
    auto test_list_2 = list<int, dynamic_allocator<int, test_map_size>>{};
    std::cout << "test list #5::max size: " << test_list_2.max_size() << std::endl;
    for (auto i = 0u; i < test_map_size + 5; ++i)
    {
        test_list_2.push_back(i);
    }

    std::cout << "test list #5::max size: " << test_list_2.max_size() << std::endl;

    std::cout << "test list #5::size: " << test_list_2.size() << ": ";
    utility::print_container(std::cout, test_list_2.begin(), test_list_2.end());

    test_list_2.pop_back();
    test_list_2.pop_back();
    test_list_2.pop_back();

    std::cout << "test list #5::size: " << test_list_2.size() << ": ";
    utility::print_container(std::cout, test_list_2.begin(), test_list_2.end());

    return 0;
}