#include "ctdt.hpp"
#include <utility>
#include <cmath>




int main()
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    One<double> one;
    auto expr = one / Sin(Exp(x) * y);

    std::cout << "Expression: " <<expr << '\n' << "Derivative: " << derivative<0>(expr) << '\n' << "Simplified: " << simplify(derivative<0>(expr)) << '\n';
}