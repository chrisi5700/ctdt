#include "../ctdt.hpp"
#include <utility>
#include <cmath>




int main()
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    Constant<double, 2.0> two;
    auto f = two ^ x;
    std::cout << f << '\n' << derivative<0>(f) << '\n';
}