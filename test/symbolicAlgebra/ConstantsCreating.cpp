#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Constant e creating", "[symbolicAlgebra]") {
  const Expression expression("e");
  REQUIRE(expression.getString() == "e");
}

TEST_CASE("Constant pi creating", "[symbolicAlgebra]") {
  const Expression expression("pi");
  REQUIRE(expression.getString() == "pi");
}