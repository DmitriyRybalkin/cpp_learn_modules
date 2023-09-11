
#include <string>

#include <gtest/gtest.h>

#include "lib.h"

TEST(VersionTest, MajorVersionTest)
{
    EXPECT_TRUE(major_version() != 0);
}

TEST(VersionTest, MinorVersionTest)
{
    EXPECT_TRUE(minor_version() == 0);
}

TEST(VersionTest, PatchVersionTest)
{
    EXPECT_TRUE(patch_version() >= 0);
}

TEST(VersionTest, FullVersionTest)
{
    EXPECT_TRUE(!std::string(full_version()).empty());
}

TEST(VersionTest, AppNameTest)
{
    EXPECT_TRUE(!std::string(application_name()).empty());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}