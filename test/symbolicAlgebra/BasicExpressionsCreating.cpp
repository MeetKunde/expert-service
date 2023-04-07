#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Creating empty expression", "[symbolicAlgebra]") {
  const Expression expression;
  REQUIRE(expression.getString() == "0");
}

TEST_CASE("Creating expression from empty string", "[symbolicAlgebra]") {
  const Expression expression("");
  REQUIRE(expression.getString().empty());
}

TEST_CASE("Creating integers from string", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("1");
  REQUIRE(expression.canBeFullEvaluated());
  REQUIRE_THAT(expression.evaluate(), Catch::Matchers::WithinAbs(1, EPSILON));

  expression = Expression("-1");
  REQUIRE(expression.canBeFullEvaluated());
  REQUIRE_THAT(expression.evaluate(), Catch::Matchers::WithinAbs(-1, EPSILON));

  expression = Expression("1234");
  REQUIRE(expression.canBeFullEvaluated());
  REQUIRE_THAT(expression.evaluate(), Catch::Matchers::WithinAbs(1234, EPSILON));

  expression = Expression("-1234");
  REQUIRE(expression.canBeFullEvaluated());
  REQUIRE_THAT(expression.evaluate(), Catch::Matchers::WithinAbs(-1234, EPSILON));
}

TEST_CASE("Creating variables from string", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("a");
  REQUIRE(expression.getString() == "a");

  expression = Expression("temporary");
  REQUIRE(expression.getString() == "temporary");

  expression = Expression("|AB|");
  REQUIRE(expression.getString() == "|AB|");

  expression = Expression("|<CAB|");
  REQUIRE(expression.getString() == "|<CAB|");

  expression = Expression("|>CAB|");
  REQUIRE(expression.getString() == "|>CAB|");

  expression = Expression("|A'B''|");
  REQUIRE(expression.getString() == "|A'B''|");
}
