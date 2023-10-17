
#include <iostream>
#include <cstdlib>
#include <map>
#include <memory>

#include "utility.h"
#include "static_allocator.h"
#include "dynamic_allocator.h"
#include "linked_list.h"

namespace {
static constexpr std::size_t test_map_size = 10u;
static constexpr char test_name[]{"test"};

struct test_str
{
    test_str()
    {
        std::cout << "test_str" << std::endl;
    }
    ~test_str()
    {
        std::cout << "~test_str" << std::endl;
    }
    test_str(const int v)
        : val{v}
    {
        std::cout << "test_str(const int v)" << std::endl;
    };
    test_str(const test_str &other)
    {
        std::cout << "test_str(const test_str& other)" << std::endl;
        val = other.val;
    }
    test_str &operator=(const test_str &other)
    {
        std::cout << "test_str& operator=(const test_str& other)" << std::endl;
        val = other.val;

        return *this;
    }

    int val;
};

static std::ostream &operator<<(std::ostream &os, const test_str &str)
{
    os << str.val;
    return os;
}

} // namespace

using namespace mem_allocators;

int main()
{
    // task 1
    {
        std::cout << "*** " << test_name << " 1 ***" << std::endl;
        auto test_map_1 = std::map<int, int, std::less<int>>{};
        for (auto i = 0u; i < test_map_size; ++i)
        {
            test_map_1[i] = utility::fact(i);
        }
        utility::print_container(std::cout, test_map_1.cbegin(), test_map_1.cend());
    }

    // task 2
    {
        std::cout << "*** " << test_name << " 2 ***" << std::endl;
        auto test_map_2 = std::map<int, int, std::less<int>, static_allocator<std::pair<const int, int>, test_map_size>>{};
        for (auto i = 0u; i < test_map_size; ++i)
        {
            test_map_2[i] = utility::fact(i);
        }
        utility::print_container(std::cout, test_map_2.cbegin(), test_map_2.cend());
    }

    // task 3.1
    {
        std::cout << "*** " << test_name << " 3 ***" << std::endl;
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
    }

    // task 4
    {
        std::cout << "*** " << test_name << " 4 ***" << std::endl;
        auto test_list = list<int>{};
        for (auto i = 0u; i < test_map_size; ++i)
        {
            test_list.push_back(i);
        }

        utility::print_container(std::cout, test_list.begin(), test_list.end());
    }

    // task 5
    {
        std::cout << "*** " << test_name << " 5 ***" << std::endl;
        auto test_list_2 = list<int, dynamic_allocator<int, test_map_size>>{};
        std::cout << "test list #5::max size: " << test_list_2.max_size() << std::endl;
        for (auto i = 0u; i < test_map_size + 5; ++i)
        {
            test_list_2.push_back(i);
        }

        std::cout << "test list #5::max size: " << test_list_2.max_size() << std::endl;

        std::cout << "test list #5::size: " << test_list_2.size() << ": ";
        utility::print_container(std::cout, test_list_2.begin(), test_list_2.end());

        test_list_2.push_front(100);
        test_list_2.push_front(200);

        test_list_2.pop_back();
        test_list_2.pop_back();
        test_list_2.pop_back();

        std::cout << "test list #5::size: " << test_list_2.size() << ": ";
        utility::print_container(std::cout, test_list_2.begin(), test_list_2.end());

        list<int, dynamic_allocator<int, test_map_size>> test_list_3 = test_list_2;

        utility::print_container(std::cout, test_list_3.begin(), test_list_3.end());

        test_list_3.push_front(100);
        test_list_3.push_front(200);

        test_list_3.pop_back();
        test_list_3.pop_back();
        test_list_3.pop_back();

        utility::print_container(std::cout, test_list_3.begin(), test_list_3.end());

        test_list_3 = test_list_2;

        utility::print_container(std::cout, test_list_3.begin(), test_list_3.end());
    }

    {
        std::cout << "*** " << test_name << " 6 ***" << std::endl;
        auto test_list_4 = list<test_str, dynamic_allocator<test_str, test_map_size>>{};
        for (auto i = 0; i < static_cast<int>(test_map_size); ++i)
        {
            test_list_4.emplace_back(test_str{i});
        }

        utility::print_container(std::cout, test_list_4.begin(), test_list_4.end());
    }

    {
        std::cout << "*** " << test_name << " 7 ***" << std::endl;
        dynamic_allocator<int, 42> ann;
        auto ptr = ann.allocate(1);
        std::cout << "ann allocator size: " << ann.size() << std::endl;
        auto bob = ann;
        std::cout << "bob allocator size: " << bob.size() << std::endl;
        ann.deallocate(ptr, 1);
        std::cout << "ann allocator size: " << ann.size() << std::endl;
    }

    {
        std::cout << "*** " << test_name << " 8 ***" << std::endl;
        dynamic_allocator<int, 10> al1;
        std::cout << "al1 allocator size: " << al1.size() << std::endl;
        dynamic_allocator<int, 10> al2;
        std::cout << "al2 allocator size: " << al2.size() << std::endl;
        auto ptr = al1.allocate(1);
        std::cout << "al1 allocator size: " << al1.size() << std::endl;
        al2 = al1;
        std::cout << "al2 allocator size: " << al2.size() << std::endl;
        al1.deallocate(ptr, 1);
        std::cout << "al1 allocator size: " << al1.size() << std::endl;
        al2.deallocate(ptr, 1);
        std::cout << "al2 allocator size: " << al2.size() << std::endl;
    }

    return 0;
}