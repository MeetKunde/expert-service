#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Opposite variable/number creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("-a");
  REQUIRE(expression.getString() == "-a");

  expression = Expression("-17");
  REQUIRE(expression.getString() == "-17");
}

TEST_CASE("Positive variable/number creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("+a");
  REQUIRE(expression.getString() == "a");

  expression = Expression("+17");
  REQUIRE(expression.getString() == "17");
}

TEST_CASE("Sums creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a + b");
  REQUIRE(expression.getString() == "a+b");

  expression = Expression("13 + 17");
  REQUIRE(expression.getString() == "13+17");

  expression = Expression("a + 13");
  REQUIRE(expression.getString() == "a+13");

  expression = Expression("17 + b");
  REQUIRE(expression.getString() == "17+b");

  expression = Expression("-a + b");
  REQUIRE(expression.getString() == "-a+b");

  expression = Expression("a + -b");
  REQUIRE(expression.getString() == "a-b");

  expression = Expression("-a + -b");
  REQUIRE(expression.getString() == "-a-b");
}

TEST_CASE("Differences creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a - b");
  REQUIRE(expression.getString() == "a-b");

  expression = Expression("13 - 17");
  REQUIRE(expression.getString() == "13-17");

  expression = Expression("a - 13");
  REQUIRE(expression.getString() == "a-13");

  expression = Expression("17 - b");
  REQUIRE(expression.getString() == "17-b");

  expression = Expression("-a - b");
  REQUIRE(expression.getString() == "-a-b");

  expression = Expression("a - -b");
  REQUIRE(expression.getString() == "a+1*1*b");

  expression = Expression("-a - -b");
  REQUIRE(expression.getString() == "-a+1*1*b");
}

TEST_CASE("Products creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a * b");
  REQUIRE(expression.getString() == "a*b");

  expression = Expression("13 * 17");
  REQUIRE(expression.getString() == "13*17");

  expression = Expression("a * 13");
  REQUIRE(expression.getString() == "a*13");

  expression = Expression("17 * b");
  REQUIRE(expression.getString() == "17*b");

  expression = Expression("-a * b");
  REQUIRE(expression.getString() == "-a*b");

  expression = Expression("a * -b");
  REQUIRE(expression.getString() == "a*-b");

  expression = Expression("-a * -b");
  REQUIRE(expression.getString() == "1*a*1*b");
}

TEST_CASE("Quotients creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a / b");
  REQUIRE(expression.getString() == "a*b^-1");

  expression = Expression("13 / 17");
  REQUIRE(expression.getString() == "13*17^-1");

  expression = Expression("a / 13");
  REQUIRE(expression.getString() == "a*13^-1");

  expression = Expression("17 / b");
  REQUIRE(expression.getString() == "17*b^-1");

  expression = Expression("-a / b");
  REQUIRE(expression.getString() == "-a*b^-1");

  expression = Expression("a / -b");
  REQUIRE(expression.getString() == "a*(-b)^-1");

  expression = Expression("-a / -b");
  REQUIRE(expression.getString() == "-a*(-b)^-1");
}

TEST_CASE("Powers creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a ^ b");
  REQUIRE(expression.getString() == "a^b");

  expression = Expression("13 ^ 17");
  REQUIRE(expression.getString() == "13^17");

  expression = Expression("a ^ 13");
  REQUIRE(expression.getString() == "a^13");

  expression = Expression("17 ^ b");
  REQUIRE(expression.getString() == "17^b");

  expression = Expression("-a ^ b");
  REQUIRE(expression.getString() == "-a^b");

  expression = Expression("a ^ -b");
  REQUIRE(expression.getString() == "a^(-b)");

  expression = Expression("-a ^ -b");
  REQUIRE(expression.getString() == "-a^(-b)");
}