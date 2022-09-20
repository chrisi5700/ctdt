#include "../ctdt.hpp"
#include <gtest/gtest.h>
#include <numbers>

TEST(Functions, SinCosTan)
{
    Variable<double, 0, 'x'> x;
    auto pi = std::numbers::pi_v<double>;
    auto sin_f = Sin(x);
    auto cos_f = Cos(x);
    auto tan_f = Tan(x);
    EXPECT_NEAR(sin_f(0.0), 0.0, 0.00001);
    EXPECT_NEAR(sin_f(pi / 2.0), 1.0, 0.00001);
    EXPECT_NEAR(cos_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(cos_f(pi / 2.0), 0.0, 0.00001);
    EXPECT_NEAR(tan_f(0.0), 0.0, 0.00001);
    EXPECT_NEAR(tan_f(pi), 0.0, 0.00001);   // cmath functions arent that precise 
}

TEST(Functions, SinhCoshTanh)
{
    Variable<double, 0, 'x'> x;
    auto sinh_f = Sinh(x);
    auto cosh_f = Cosh(x);
    auto tanh_f = Tanh(x);
    EXPECT_NEAR(sinh_f(0.0), 0.0, 0.00001);
    EXPECT_NEAR(cosh_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(tanh_f(0.0), 0.0, 0.00001);
}

TEST(Functions, ExpLn)
{
    Variable<double, 0, 'x'> x;
    auto exp_f = Exp(x);
    auto ln_f = Ln(x);
    EXPECT_NEAR(exp_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(ln_f(1.0), 0.0, 0.00001);
}

TEST(Functions, SqrtCbrt)
{
    Variable<double, 0, 'x'> x;
    auto sqrt_f = Sqrt(x);
    auto cbrt_f = Cbrt(x);
    EXPECT_NEAR(sqrt(16.0), 4.0, 0.00001);
    EXPECT_NEAR(cbrt(8.0), 2.0, 0.00001);
}



TEST(Derivative, SinCosTan)
{
    Variable<double, 0, 'x'> x;
    auto pi = std::numbers::pi_v<double>;
    auto sin_f = Sin(x);
    auto dxsin_f = derivative<0>(sin_f);    // cos
    auto cos_f = Cos(x);
    auto dxcos_f = derivative<0>(cos_f);    // -sin
    auto tan_f = Tan(x);
    auto dxtan_f = derivative<0>(tan_f);    // 1/sec² = 1 / cos²
    EXPECT_NEAR(dxsin_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(dxsin_f(pi / 2.0), 0.0, 0.00001);
    EXPECT_NEAR(dxcos_f(0.0), 0.0, 0.00001);
    EXPECT_NEAR(dxcos_f(pi / 2.0), -1.0, 0.00001);
    EXPECT_NEAR(dxtan_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(dxtan_f(pi), 1.0, 0.00001);   
}

TEST(Derivative, SinhCoshTanh)
{
    Variable<double, 0, 'x'> x;
    auto sinh_f = Sinh(x);
    auto dxsinh_f = derivative<0>(sinh_f);  // cosh
    auto cosh_f = Cosh(x);
    auto dxcosh_f = derivative<0>(cosh_f);  // sinh
    auto tanh_f = Tanh(x);
    auto dxtanh_f = derivative<0>(tanh_f);  // 1 / sech = 1/cosh²

    EXPECT_NEAR(dxsinh_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(dxcosh_f(0.0), 0.0, 0.00001);
    EXPECT_NEAR(dxtanh_f(0.0), 1.0, 0.00001);
}

TEST(Derivative, ExpLn)
{
    Variable<double, 0, 'x'> x;
    auto exp_f = Exp(x);
    auto dxexp_f = derivative<0>(exp_f);    // e^x
    auto ln_f = Ln(x);
    auto dxln_f = derivative<0>(ln_f);      // 1 / x
    EXPECT_NEAR(dxexp_f(0.0), 1.0, 0.00001);
    EXPECT_NEAR(dxln_f(1.0), 1.0, 0.00001);
    EXPECT_NEAR(dxln_f(2.0), 0.5, 0.00001);
}

TEST(Derivative, SqrtCbrt)
{
    Variable<double, 0, 'x'> x;
    auto sqrt_f = Sqrt(x);
    auto dxsqrt_f = derivative<0>(sqrt_f);  // 1 / (2sqrt(x))
    auto cbrt_f = Cbrt(x);
    auto dxcbrt_f = derivative<0>(cbrt_f);  // 1 / (3 x^(2/3))
    EXPECT_NEAR(dxsqrt_f(4.0), 0.25, 0.00001);  // 1 / 2 * 2 = 1 / 4
    EXPECT_NEAR(dxcbrt_f(8.0), 1.0 / 12.0, 0.00001);   // 1 / (3 * 2^3^(2/3)) = 1/ (3 * 2^2) = 1 / 12
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
