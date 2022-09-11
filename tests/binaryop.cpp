#include "../ctdt.hpp"
#include <gtest/gtest.h>

TEST(Functions, Add)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x + y;
    EXPECT_FLOAT_EQ(f(2, 3), 5);
    EXPECT_FLOAT_EQ(f(3, -1), 2);
    EXPECT_FLOAT_EQ(f(0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55), 1);
}

TEST(Functions, Sub)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x - y;
    EXPECT_FLOAT_EQ(f(2, 3), -1);
    EXPECT_FLOAT_EQ(f(3, -1), 4);
    EXPECT_FLOAT_EQ(f(0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55), -1);
}

TEST(Functions, Mul)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x * y;
    EXPECT_FLOAT_EQ(f(2, 3), 6);
    EXPECT_FLOAT_EQ(f(3, -1), -3);
    EXPECT_FLOAT_EQ(f(0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55), 0);
}

TEST(Functions, Div)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x / y;
    EXPECT_FLOAT_EQ(f(2, 3), 2.0/3.0);
    EXPECT_FLOAT_EQ(f(3, -1), -3);
    EXPECT_FLOAT_EQ(f(0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55), 0);
}



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
