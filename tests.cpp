#include "ctdt.hpp"
#include <utility>
#include <cmath>


int main()
{
    Variable<double, 0, 'x'> x;
    Variable<double, 1, 'y'> y;
    auto f = x - y * y;
    std::cout << f << '\n' << derivative<1>(f) << '\n';
}