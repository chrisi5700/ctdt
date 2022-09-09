# CTDT

## C++ Compile Time Derivatives

### Example:

```cpp
    Variable<double, 0, 'x'> x;      // 1
    Variable<double, 1, 'y'> y;
    auto f = Sin(Exp(x) * y);        // 2
    double val = f(0, 0, 42);          // 3
    auto df = derivative<0>(f);      // 4
```

(1)    First we declare our variables using the underlying type we want to operate on (here *double*), the *ID* of the variable and optionally a character to represent the variable with  when printing the function (*x* in this case). 

The ID is used if you want to differentiate the function with respect to that variable and will also determine the place of the variable when calling the function.

Keep in mind the representation of the variable is just for printing and has no other uses besides that. The underlying type **must** be constructible from 0 and 1.



(2)    Here the function $f(x, y) = \sin{(e^x \cdot y)}$ is defined. 



(3)    $f$ can be invoked like any other function. Here this call would resolve to $f(0, 0) = \sin{(e^0 \cdot 0)} = 0$ . Notice that the third argument (42) is ignored because there is no variable defined that has the ID 2. 



(4)    Here  $\frac{\partial f}{\partial x}$ is evaluated to $\cos{(e^x y)} \cdot e^x y$  this function can be further differentiated and also invoked.





## Already Defined Functions:

- Variables:     Variable<Type, ID, Representation>

- Basic Arithmetic (+, -, *, /) 

- Constants: 
  
  - One<T>,  Zero<T>,  NegativeOne<T>
  
  - Other Constants need to be defined as Constant<T, Value>

- Unary Functions:
  
  - Sin, Cos
  
  -  Exp, Ln
  
  -  Sqrt, Cbrt



## Todo (in order of priority):

- Better system for simplifications often inverses aren't cancelled properly
  
  - This issue can be entirely avoided by compiling with -ffast-math

- Tan, Sinh, Cosh, Tanh, Pow (^)  (makes sense to implement after rewriting the simplification system)

- 


