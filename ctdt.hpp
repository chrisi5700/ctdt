/*
CTDerivatives/ctdt2.hpp by Chris
02.09.2022
*/

#ifndef CTDT
#define CTDT

#include <type_traits>
#include <iostream>
#include <tuple>
#include <cmath>
#include <numbers>

// ------------------------------------------------------------------------------------------------
// Basic Function Atoms

template <class T>
concept MathType = requires(T t1, T t2)
{
    {t1 * t2};
    {t1 + t2};
    {t1 / t2};
    {t1 - t2};
}
&&std::constructible_from<T, int>;

template <class T>
concept Expression = true;

// ---------------------------------------------- Constant Definition ----------------------------------------------

template <MathType T, T Value>
struct Constant
{
    static constexpr auto function = []<std::convertible_to<T>... Ts>(Ts...) { return Value; };
    using Type = T; // used for operator overloads
};

template <MathType T>
using Zero = Constant<T, T{0}>;

template <MathType T>
using One = Constant<T, T{1}>;

template <MathType T>
using NegativeOne = Constant<T, T{-1}>;

template <MathType T, T Val>
std::ostream &operator<<(std::ostream &os, Constant<T, Val>)
{
    return os << Val;
}

template <std::size_t, MathType T, T Val>
auto derivative(Constant<T, Val>)
{
    return Constant<T, T{0}>{}; // n / dx = 0
}

template <MathType T, T Val>
auto simplify(Constant<T, Val>)
{
    return Constant<T, Val>{}; // unsimplifiable
}

// ---------------------------------------------- Variable Definition ----------------------------------------------

template <MathType T, std::size_t Var, char Repr = '\0'>
struct Variable
{
    static constexpr auto function = []<std::convertible_to<T>... Ts>(Ts... args) { return std::get<Var>(std::tuple<Ts...>{args...}); };
    using Type = T; // used for operator overloads
};

template <MathType T, std::size_t Var, char Repr>
std::ostream &operator<<(std::ostream &os, Variable<T, Var, Repr>)
{
    if (Repr != '\0')
        return os << Repr;
    return os << "Var<" << Var << '>';
}

template <std::size_t DVar, MathType T, std::size_t Var, char Repr>
auto derivative(Variable<T, Var, Repr>)
{
    if constexpr (Var == DVar)
        return One<T>{}; // x / dx = 1
    if constexpr (Var != DVar)
        return Zero<T>{}; // x / dy = 0;
}

template <MathType T, std::size_t Var, char Repr>
auto simplify(Variable<T, Var, Repr>)
{
    return Variable<T, Var, Repr>{}; // unsimplifiable
}

// ---------------------------------------------- Utility and clean up ----------------------------------------------

template <Expression E>
using simplified = decltype(simplify(std::declval<E>()));

// Expressions are made up from Constants Variables (atomics) and Operations
// Sometimes useful for printing might be obsolete tho.
template <Expression E>
constexpr bool is_atomic = false;

template <MathType T, T V>
constexpr bool is_atomic<Constant<T, V>> = true;

template <MathType T, std::size_t V, char R>
constexpr bool is_atomic<Variable<T, V, R>> = true;

// Used for macro fiddeling    (removing braces from type in macro)
// Macro(T) argument_type<void(T)>::type == Macro((ConcT<T1, T2>)) -> argument_type<void((ConcT<T1, T2>))>::type -> ConcT<T1, T2>
template <typename T>
struct argument_type;
template <typename T, typename U>
struct argument_type<T(U)>
{
    using type = U;
};

// ---------------------------------------------- Binary Operator Macros and Definitions ----------------------------------------------

// -------------------------------------------------Class Macro-------------------------------------------------
#define GenerateBinaryOperator(name, op)                                                                                                            \
    template <MathType T, Expression E1, Expression E2>                                                                                             \
    struct name                                                                                                                                     \
    {                                                                                                                                               \
        using SE1 = simplified<E1>;                                                                                                                 \
        using SE2 = simplified<E2>;                                                                                                                 \
                                                                                                                                                    \
        template <std::size_t DVar>                                                                                                                 \
        using DE1 = decltype(derivative<DVar>(SE1{}));                                                                                              \
                                                                                                                                                    \
        template <std::size_t DVar>                                                                                                                 \
        using DE2 = decltype(derivative<DVar>(SE2{}));                                                                                              \
                                                                                                                                                    \
        using Type = T;                                                                                                                             \
                                                                                                                                                    \
        static constexpr auto function = []<std::convertible_to<T>... Ts>(Ts... args) { return SE1::function(args...) op SE2::function(args...); }; \
        template <std::convertible_to<T>... Ts>                                                                                                     \
        T operator()(Ts... args)                                                                                                                    \
        {                                                                                                                                           \
            return function(args...);                                                                                                               \
        }                                                                                                                                           \
    };                                                                                                                                              \
                                                                                                                                                    \
    template <Expression E1, Expression E2>                                                                                                         \
    auto operator op(E1, E2)                                                                                                                        \
    {                                                                                                                                               \
        static_assert(std::same_as<typename E1::Type, typename E2::Type>);                                                                          \
        return name<typename E1::Type, E1, E2>{};                                                                                                   \
    }                                                                                                                                               \
                                                                                                                                                    \
    template <MathType T, Expression E1, Expression E2>                                                                                             \
    std::ostream &operator<<(std::ostream &os, name<T, E1, E2>)                                                                                     \
    {                                                                                                                                               \
        return os << '(' << E1{} << #op << E2{} << ')';                                                                                             \
    }                                                                                                                                               \
                                                                                                                                                    \
    template <MathType T, T V1, T V2>                                                                                                               \
    auto simplify(name<T, Constant<T, V1>, Constant<T, V2>>)                                                                                        \
    {                                                                                                                                               \
        return Constant<T, V1 op V2>{};                                                                                                             \
    }

// -------------------------------------------------Derivative Macro-------------------------------------------------
#define GenerateBinaryOperatorDerivative(name, dt)                        \
    template <std::size_t DVar, MathType T, Expression E1, Expression E2> \
    auto derivative(name<T, E1, E2>)                                      \
    {                                                                     \
        using E = name<T, E1, E2>;                                        \
        using DE1 = typename E::template DE1<DVar>;                       \
        using DE2 = typename E::template DE2<DVar>;                       \
        using SE1 = decltype(simplify(E1{}));                             \
        using SE2 = decltype(simplify(E2{}));                             \
        using DT = typename argument_type<void(dt)>::type;                \
        return simplify(DT{});                                            \
    }

// -------------------------------------------------Simplify Macro-------------------------------------------------
#define StartBinaryOperatorSimplification(name)         \
    template <MathType T, Expression E1, Expression E2> \
    auto simplify(name<T, E1, E2>)                      \
    {                                                   \
        using E = name<T, E1, E2>;                      \
        using SE1 = typename E::SE1;                    \
        using SE2 = typename E::SE2;                    \
        if constexpr (false)                            \
        {                                               \
            return;                                     \
        }

#define AddSimplification(cond, res)                      \
    else if constexpr (cond)                              \
    {                                                     \
        return typename argument_type<void(res)>::type{}; \
    }

#define EndBinaryOperatorSimplification(name) \
    else                                      \
    {                                         \
        return name<T, SE1, SE2>{};           \
    }                                         \
    }

/************************************************
 * GenerateBinaryOperator: Generates the type representing the operation, the operator overload,  the ostream overload and basic constant optimization
 * GenerateDerivative:  The name declares the derivative for the representing class. When defining the derivative DE(1/2) is used for the differentiated lhs / rhs expr
 *                      SE(1/2) for the simplified expression which should be prefered over E(1/2) which is just the regular expression gained through template deduction
 * StartBinaryOperatorSimplification: Sets up SE(1/2) to refer to the simplified expr cant be ended with semicolons
 * AddSimplification: if cond true -> return type will be res which implies that order of the simplifications matters   cant be ended with semicolons
 * EndBinaryOperator: if no optimization is valid -> returns the same expr but with the lhs / rhs simplified
 ***********************************************/

// ------------------------------------------------- Add -------------------------------------------------

GenerateBinaryOperator(Add, +);
GenerateBinaryOperatorDerivative(Add, (Add<T, DE1, DE2>)); // (f + g)' = f' + g'

// ------------------------------------------------- Sub -------------------------------------------------

GenerateBinaryOperator(Sub, -);
GenerateBinaryOperatorDerivative(Sub, (Sub<T, DE1, DE2>)); // (f - g)' = f' - g'

// ------------------------------------------------- Mul -------------------------------------------------

GenerateBinaryOperator(Mul, *);
GenerateBinaryOperatorDerivative(Mul, (Add<T, Mul<T, DE1, SE2>, Mul<T, SE1, DE2>>)); // (f * g)' = f'*g + f*g'

// ------------------------------------------------- Div -------------------------------------------------

GenerateBinaryOperator(Div, /);
GenerateBinaryOperatorDerivative(Div, (Div<T, Sub<T, Mul<T, DE1, SE2>, Mul<T, SE1, DE2>>, Mul<T, SE2, SE2>>)); // (f / g)' = (f' * g - f * g') / (g * g)

// ------------------------------------------------- Unary Minus -------------------------------------------------

template <MathType T, Expression E1>
struct UnaryMinus
{
    using SE1 = simplified<E1>;
    template <std::size_t DVar>
    using DE1 = decltype(derivative<DVar>(SE1{}));

    using Type = T;
    static constexpr auto function = []<std::convertible_to<T>... Ts>(Ts... args) { return -SE1::function(args...); };
    template <std::convertible_to<T>... Ts>
    T operator()(Ts... args) { return function(args...); }
};
template <Expression E1>
auto operator-(E1)
{
    return UnaryMinus<typename E1::Type, E1>{};
}
template <MathType T, Expression E1>
std::ostream &operator<<(std::ostream &os, UnaryMinus<T, E1>) { return os << "-(" << E1{} << ')'; }

template <MathType T, T V1>
auto simplify(UnaryMinus<T, Constant<T, V1>>) { return Constant<T, -V1>{}; };

template <std::size_t DVar, MathType T, Expression E1>
auto derivative(UnaryMinus<T, E1>)
{
    using E = UnaryMinus<T, E1>;
    using DE1 = typename E::template DE1<DVar>;
    using SE1 = decltype(simplify(E1{}));
    using DT = UnaryMinus<T, DE1>;
    return simplify(DT{});
};

// ------------------------------------------------- Simplifications -------------------------------------------------

StartBinaryOperatorSimplification(Add)
    AddSimplification((std::same_as<SE1, Zero<T>>), SE2) // 0 + y = y
    AddSimplification((std::same_as<SE2, Zero<T>>), SE1) // x + 0 = x
    AddSimplification((std::same_as<SE1, SE2>), (Mul<T, Constant<T, T{2}>, SE1>))
        EndBinaryOperatorSimplification(Add);

StartBinaryOperatorSimplification(Sub)
    AddSimplification((std::same_as<SE2, Zero<T>>), SE1)                  // x - 0 = x
    AddSimplification((std::same_as<SE1, SE2>), Zero<double>)             // x - x = 0
    AddSimplification((std::same_as<SE1, Zero<T>>), (UnaryMinus<T, SE2>)) // 0 - y = -y
    EndBinaryOperatorSimplification(Sub);

StartBinaryOperatorSimplification(Mul)
    AddSimplification((std::same_as<SE1, Zero<T>> || std::same_as<SE2, Zero<T>>), Zero<T>) // x * 0 = 0 * y = 0
    AddSimplification((std::same_as<SE1, One<T>>), SE2)                                    // 1 * y = y
    AddSimplification((std::same_as<SE2, One<T>>), SE1)                                    // x * 1 = x
    EndBinaryOperatorSimplification(Mul);

StartBinaryOperatorSimplification(Div)
    AddSimplification((std::same_as<SE1, Zero<T>>), Zero<T>) // 0 / y = 0
    AddSimplification((std::same_as<SE2, One<T>>), SE1)      // x / 1 = x
    AddSimplification((std::same_as<SE1, SE2>), One<T>)      // x / x = 1 beware this also implies 0 / 0 = 1 maybe fix that
    EndBinaryOperatorSimplification(Div);

// ---------------------------------------------- Unary Function Macros and Definitions ----------------------------------------------

// -------------------------------------------------Class Macro-------------------------------------------------
#define GenerateUnaryFunction(name, func)                                                                                       \
    template <MathType T, Expression E1>                                                                                        \
    struct name##_impl                                                                                                          \
    {                                                                                                                           \
        using SE1 = simplified<E1>;                                                                                             \
                                                                                                                                \
        template <std::size_t DVar>                                                                                             \
        using DE1 = decltype(derivative<DVar>(SE1{}));                                                                          \
                                                                                                                                \
        using Type = T;                                                                                                         \
                                                                                                                                \
        static constexpr auto function = []<std::convertible_to<T>... Ts>(Ts... args) { return func(SE1::function(args...)); }; \
        template <std::convertible_to<T>... Ts>                                                                                 \
        T operator()(Ts... args)                                                                                                \
        {                                                                                                                       \
            return function(args...);                                                                                           \
        }                                                                                                                       \
    };                                                                                                                          \
                                                                                                                                \
    template <Expression E1>                                                                                                    \
    auto name(E1)                                                                                                               \
    {                                                                                                                           \
        return name##_impl<typename E1::Type, E1>{};                                                                            \
    }                                                                                                                           \
                                                                                                                                \
    template <MathType T, Expression E1>                                                                                        \
    std::ostream &operator<<(std::ostream &os, name##_impl<T, E1>)                                                              \
    {                                                                                                                           \
        if (is_atomic<E1>)                                                                                                      \
        {                                                                                                                       \
            return os << '(' << #name << '(' << E1{} << ')' << ')';                                                             \
        }                                                                                                                       \
        return os << '(' << #name << E1{} << ')';                                                                               \
    }                                                                                                                           \
                                                                                                                                \
    template <MathType T, T V1>                                                                                                 \
    auto simplify(name##_impl<T, Constant<T, V1>>)                                                                              \
    {                                                                                                                           \
        return Constant<T, func(V1)>{};                                                                                         \
    }

// ------------------------------------------------- Derivative Macro -------------------------------------------------

#define GenerateUnaryFunctionDerivative(name, dt)          \
    template <std::size_t DVar, MathType T, Expression E1> \
    auto derivative(name##_impl<T, E1>)                    \
    {                                                      \
        using E = name##_impl<T, E1>;                      \
        using DE1 = typename E::template DE1<DVar>;        \
        using SE1 = decltype(simplify(E1{}));              \
        using DT = typename argument_type<void(dt)>::type; \
        return simplify(DT{});                             \
    }

// ------------------------------------------------- Simplify Macro -------------------------------------------------

#define StartUnaryFunctionSimplification(name) \
    template <MathType T, Expression E1>       \
    auto simplify(name##_impl<T, E1>)          \
    {                                          \
        using E = name##_impl<T, E1>;          \
        using SE1 = typename E::SE1;           \
        if constexpr (false)                   \
        {                                      \
            return;                            \
        }

#define AddSimplification(cond, res)                      \
    else if constexpr (cond)                              \
    {                                                     \
        return typename argument_type<void(res)>::type{}; \
    }

#define EndUnaryFunctionSimplification(name) \
    else                                     \
    {                                        \
        return name##_impl<T, SE1>{};        \
    }                                        \
    }

// Docs:
// GenerateUnaryFunction: Generates the type representing the operation, the operator overload,  the ostream overload and basic constant optimization
// GenerateUnaryFunctionDerivative:  The name declares the derivative for the representing class. When defining the derivative DE1 is used for the differentiated arg
//                      SE1 for the simplified arg which should be prefered over E1 which is just the regular arg gained through template deduction
// StartUnaryFunctionSimplification: Sets up SE(1/2) to refer to the simplified expr    cant be ended with semicolons
// AddSimplification: if cond true -> return type will be res which implies that order of the simplifications matters   cant be ended with semicolons
// EndUnaryFunctionSimplification: if no optimization is valid -> returns the same expr but with the lhs / rhs simplified

// ------------------------------------------------- Sin, Cos, Tan -------------------------------------------------

GenerateUnaryFunction(Sin, std::sin);
StartUnaryFunctionSimplification(Sin)
    EndUnaryFunctionSimplification(Sin); // got no simplification on my mind rn

GenerateUnaryFunction(Cos, std::cos);
StartUnaryFunctionSimplification(Cos)
    EndUnaryFunctionSimplification(Cos); // got no simplification on my mind rn

// defined in a different block because of the interdependence of the types
GenerateUnaryFunctionDerivative(Sin, (Mul<T, Cos_impl<T, SE1>, DE1>));                         // (sin(f))' = cos(f) * f'
GenerateUnaryFunctionDerivative(Cos, (Mul<T, Mul<T, NegativeOne<T>, Sin_impl<T, SE1>>, DE1>)); // (cos(f))' = -sin(f) * f'


GenerateUnaryFunction(Tan, std::tan);
StartUnaryFunctionSimplification(Tan)
    EndUnaryFunctionSimplification(Tan);

GenerateUnaryFunctionDerivative(Tan, (Div<T, DE1, Mul<T, Cos_impl<T, SE1>, Cos_impl<T, SE1>>>)) // (tan(f))' = f'/(cos(f) * cos(f))

// ------------------------------------------------- exp / ln Function -------------------------------------------------
// todo: add inverses maybe theres a smooth way to do it for multiple function types requires minor rewrite otherwise...

GenerateUnaryFunction(Exp, std::exp);
StartUnaryFunctionSimplification(Exp)
    EndUnaryFunctionSimplification(Exp);
GenerateUnaryFunctionDerivative(Exp, (Mul<T, Exp_impl<T, SE1>, DE1>));

GenerateUnaryFunction(Ln, std::log);
StartUnaryFunctionSimplification(Ln)
    EndUnaryFunctionSimplification(Ln);
GenerateUnaryFunctionDerivative(Ln, (Div<T, DE1, SE1>));


// ------------------------------------------------- Sqrt / Cbrt -------------------------------------------------

GenerateUnaryFunction(Sqrt, std::sqrt);
StartUnaryFunctionSimplification(Sqrt)
    EndUnaryFunctionSimplification(Sqrt);
GenerateUnaryFunctionDerivative(Sqrt, (Div<T, DE1, Mul<T, Constant<T, T{2}>, Sqrt_impl<T, SE1>>>));

GenerateUnaryFunction(Cbrt, std::cbrt);
StartUnaryFunctionSimplification(Cbrt)
    EndUnaryFunctionSimplification(Cbrt);
GenerateUnaryFunctionDerivative(Cbrt, (Div<T, DE1, Mul<T, Constant<T, T{3}>, Mul<T, Cbrt_impl<T, SE1>, Cbrt_impl<T, SE1>>>>));


// ------------------------------------------------- Sinh, Cosh, Tanh -------------------------------------------------
GenerateUnaryFunction(Sinh, std::sinh);
StartUnaryFunctionSimplification(Sinh)
    EndUnaryFunctionSimplification(Sinh);

GenerateUnaryFunction(Cosh, std::cosh);
StartUnaryFunctionSimplification(Cosh)
    EndUnaryFunctionSimplification(Cosh);

GenerateUnaryFunctionDerivative(Sinh, (Mul<T, DE1, Cosh_impl<T, SE1>>)); // (sinh(f))' = f' * cosh(f)
GenerateUnaryFunctionDerivative(Cosh, (Mul<T, DE1, Sinh_impl<T, SE1>>)); // (cosh(f))' = f' * sinh(f)

GenerateUnaryFunction(Tanh, std::tanh);
StartUnaryFunctionSimplification(Tanh)
    EndUnaryFunctionSimplification(Tanh);
GenerateUnaryFunctionDerivative(Tanh, (Div<T, DE1, Mul<T, Cosh_impl<T, SE1>, Cosh_impl<T, SE1>>>));


#endif