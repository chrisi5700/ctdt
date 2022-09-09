#include "ctdt.hpp"
#include <utility>
#include <cmath>




int main()
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = Sin(Exp(x) * y);
    auto val = f(0, 0);
    std::cout << "Expression: " << f << '\n' << "Derivative: " << derivative<0>(f) << '\n' << "Simplified: " << simplify(derivative<0>(f)) << '\n' << "Val: " << val << '\n'; 
}