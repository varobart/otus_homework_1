#include <gtest/gtest.h>

#include <hello.h>

TEST(Version, is_valid_version) { ASSERT_TRUE(version::version() > 0); }
