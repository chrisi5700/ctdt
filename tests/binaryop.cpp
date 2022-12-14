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

TEST(Functions, Pow)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x ^ y;
    EXPECT_NEAR(f(2, 3), 8.0, 0.00001); // 2³ = 8
    EXPECT_NEAR(f(1, 5), 1.0, 0.00001); // 1⁵ = 1
    EXPECT_NEAR(f(0, 12), 0.0, 0.00001); // 0¹² = 0;
}

TEST(Derivatives, Add)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x + y;
    auto dx = derivative<0>(f); // 1 + 0
    auto dy = derivative<1>(f); // 0 + 1

    EXPECT_FLOAT_EQ(dx(0, 0), 1.0);
    EXPECT_FLOAT_EQ(dy(0, 0), 1.0);

}

TEST(Derivatives, Sub)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x - y;
    auto dx = derivative<0>(f); // 1 + 0
    auto dy = derivative<1>(f); // 0 + 1

    EXPECT_FLOAT_EQ(dx(0, 0), 1.0);
    EXPECT_FLOAT_EQ(dy(0, 0), -1.0);
}

TEST(Derivatives, Mul)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x * y;
    auto dx = derivative<0>(f); // y
    auto dy = derivative<1>(f); 

    EXPECT_FLOAT_EQ(dx(0, 5), 5.0);
    EXPECT_FLOAT_EQ(dx(0, -1), -1.0);
    EXPECT_FLOAT_EQ(dy(1, 0), 1.0);
    EXPECT_FLOAT_EQ(dy(2, 0), 2.0);
}

TEST(Derivatives, Div)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x / y;
    auto dx = derivative<0>(f); // 1/y 
    auto dy = derivative<1>(f); // -x/y²

    EXPECT_FLOAT_EQ(dx(0, 5), 1.0/5.0);
    EXPECT_FLOAT_EQ(dx(0, -1), -1.0);
    EXPECT_FLOAT_EQ(dy(1, 2), -1.0/(2.0 * 2.0));
    EXPECT_FLOAT_EQ(dy(2, 10), -2.0/(10.0 * 10.0));
}

TEST(Derivatives, Pow)
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x ^ y;
    auto dxf = derivative<0>(f);    // dx x^y = y * x^(y-1)
    auto dyf = derivative<1>(f);    // dy x^y = x^y ln(x)
    EXPECT_NEAR(dxf(2, 2), 4, 0.00001);  // 2 * 2^1 = 4
    EXPECT_NEAR(dxf(3, 4), 4* 27, 0.00001);            // 4 * 3^3 
    EXPECT_NEAR(dyf(1, 5), 0, 0.00001);        // 1^5 * ln(1) = 0
    EXPECT_NEAR(dyf(2, 0), std::log(2.0), 0.00001);                   // 2^0 * ln(2) = ln(2)
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
