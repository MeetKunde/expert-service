#ifndef SYMBOLIC_ALGEBRA_TEST_BASE_H
#define SYMBOLIC_ALGEBRA_TEST_BASE_H

#include <catch2/catch.hpp>

#include <symbolicAlgebra.h>

constexpr float PI = 3.14159265359;
constexpr float PI_HALF = 1.5707963268;
constexpr float EPSILON = 0.00001;
constexpr float BIG_EPSILON = 0.1;

typedef symbolicAlgebra::implementation::Atom Atom;

typedef symbolicAlgebra::implementation::Var Var;
typedef symbolicAlgebra::implementation::Const Const;
typedef symbolicAlgebra::implementation::Number Number;

typedef symbolicAlgebra::implementation::Sum Sum;
typedef symbolicAlgebra::implementation::Product Product;
typedef symbolicAlgebra::implementation::Power Power;

typedef symbolicAlgebra::implementation::Sin Sin;
typedef symbolicAlgebra::implementation::Cos Cos;
typedef symbolicAlgebra::implementation::Tan Tan;
typedef symbolicAlgebra::implementation::Cot Cot;
typedef symbolicAlgebra::implementation::Log Log;
typedef symbolicAlgebra::implementation::Sqrt Sqrt;

typedef symbolicAlgebra::Expression Expression;
typedef symbolicAlgebra::Equation Equation;
typedef symbolicAlgebra::Solve Solve;

#endif  // SYMBOLIC_ALGEBRA_TEST_BASE_H