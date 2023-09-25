
#include <fstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ip_filter.h"

using namespace user_types;
using namespace utility;
using namespace ip_filter;

TEST(utility_test, test_split_parse)
{
    // Arrange
    ip_pool_t ip_pool;

    // Act
    std::ifstream ifs("ip_filter.tsv");
    if (ifs.good())
    {
        for (std::string line; std::getline(ifs, line);)
        {
            auto v = utility::split(line, '\t');
            ip_pool.emplace_back(utility::parse_ip(v.at(0)));
        }
    }

    // Assert
    ASSERT_TRUE(!ip_pool.empty());
}

TEST(utility_test, test_lex_compare)
{
    // Arrange
    constexpr ip_t ip1{192u, 168u, 0u, 1u};
    constexpr ip_t ip2{192u, 168u, 0u, 2u};
    constexpr ip_t ip3{192u, 168u, 0u, 1u};
    struct equal_cmp
    {
        bool operator()(const ip_t::value_type lhs, const ip_t::value_type rhs)
        {
            return lhs == rhs;
        }
    };

    // Act and Assert
    ASSERT_TRUE(lex_compare(ip2.begin(), ip2.end(), ip1.begin(), ip1.end(), std::greater<ip_t::value_type>()));
    ASSERT_TRUE(lex_compare(ip1.begin(), ip1.end(), ip2.begin(), ip2.end(), std::less<ip_t::value_type>()));
    ASSERT_TRUE(lex_compare(ip1.begin(), ip1.end(), ip3.begin(), ip3.end(), equal_cmp()));
}

TEST(utility_test, test_filter_copy_if)
{
    // Arrange
    static const ip_pool_t ip_pool{{192u, 168u, 0u, 1u}, {192u, 168u, 0u, 2u}, {255u, 255u, 255u, 248u}, {14u, 177u, 248u, 167u}};
    const user_types::ip_t::value_type mask_1{177u};
    const user_types::ip_t mask_2{192u, 168u, 0u, 0u};
    const user_types::ip_t mask_3{255u, 255u, 255u, 248u};

    // Act
    const auto filtered_pool_1 = utility::filter_copy_if(ip_pool, [mask_1](const user_types::ip_t &ip) {
        const auto it = std::find_if(ip.cbegin(), ip.cend(), [mask_1](const auto val) { return val == mask_1; });
        return it != ip.cend();
    });
    const auto filtered_pool_2 = utility::filter_copy_if(ip_pool,
        [&mask_2](const user_types::ip_t &ip) { return std::equal(ip.cbegin(), ip.cbegin() + 2, mask_2.cbegin(), mask_2.cbegin() + 2); });
    const auto filtered_pool_3 = utility::filter_copy_if(
        ip_pool, [&mask_3](const user_types::ip_t &ip) { return std::equal(ip.cbegin(), ip.cend(), mask_3.cbegin(), mask_3.cend()); });

    // Assert
    ASSERT_TRUE(!filtered_pool_1.empty());
    for (auto &&ip : filtered_pool_1)
    {
        ASSERT_THAT(ip, ::testing::ElementsAreArray({14u, 177u, 248u, 167u}));
    }

    ASSERT_TRUE(!filtered_pool_2.empty());
    for (auto idx = 0u; idx < mask_2.size() - 2; ++idx)
    {
        ASSERT_EQ(filtered_pool_2[0][idx], mask_2[idx]);
        ASSERT_EQ(filtered_pool_2[1][idx], mask_2[idx]);
    }

    ASSERT_TRUE(!filtered_pool_3.empty());
    ASSERT_THAT(filtered_pool_3.at(0), ::testing::ElementsAreArray(mask_3));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}