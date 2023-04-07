#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Variable/number in parenthesis", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("(a)");
  REQUIRE(expression.getString() == "a");

  expression = Expression("(((a)))");
  REQUIRE(expression.getString() == "a");

  expression = Expression("(-a)");
  REQUIRE(expression.getString() == "-a");

  expression = Expression("((11))");
  REQUIRE(expression.getString() == "11");

  expression = Expression("(-11)");
  REQUIRE(expression.getString() == "-11");
}

TEST_CASE("Multiple sum creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a + b + c");
  REQUIRE(expression.getString() == "a+b+c");

  expression = Expression("a*x + b*y + c*z");
  REQUIRE(expression.getString() == "a*x+b*y+c*z");
}

TEST_CASE("Multiple difference creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a - b - c");
  REQUIRE(expression.getString() == "a-b-c");

  expression = Expression("a*x - b*y - c*z");
  REQUIRE(expression.getString() == "a*x-b*y-c*z");
}

TEST_CASE("Multiple product creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a * b * c");
  REQUIRE(expression.getString() == "a*b*c");

  expression = Expression("(a+x) * (b+y) * (c+z)");
  REQUIRE(expression.getString() == "(a+x)*(b+y)*(c+z)");
}

TEST_CASE("Multiple quatient creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a / b / c");
  REQUIRE(expression.getString() == "a*b^-1*c^-1");

  expression = Expression("(a+x) / (b+y) / (c+z)");
  REQUIRE(expression.getString() == "(a+x)*(b+y)^-1*(c+z)^-1");
}

TEST_CASE("Multiple power creating", "[symbolicAlgebra]") {
  const Expression expression("a^b^c");
  REQUIRE(expression.getString() == "a^(b^c)");
}

TEST_CASE("Order of operations", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("(a + b) * (c + d)");
  REQUIRE(expression.getString() == "(a+b)*(c+d)");

  expression = Expression("a + b * c + d");
  REQUIRE(expression.getString() == "a+b*c+d");

  expression = Expression("(a+b)^c");
  REQUIRE(expression.getString() == "(a+b)^c");
}