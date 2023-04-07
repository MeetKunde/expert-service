#include "SymbolicAlgebraTestBase.h"

TEST_CASE("General rules", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("sin(x)");
  REQUIRE(expression.getString() == "sin(x)");

  expression = Expression("sin(-x)");
  REQUIRE(expression.getString() == "sin(-x)");

  expression = Expression("-sin(x)");
  REQUIRE(expression.getString() == "-sin(x)");

  expression = Expression("sin(x) + cos(x)");
  REQUIRE(expression.getString() == "sin(x)+cos(x)");

  expression = Expression("cos((x+y)*(a-b))");
  REQUIRE(expression.getString() == "cos((x+y)*(a-b))");

  expression = Expression("cos((x+y)*(a-(b-c)*7))");
  REQUIRE(expression.getString() == "cos((x+y)*(a-(b-c)*7))");
}

TEST_CASE("Sin creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("sin(x)");
  REQUIRE(expression.getString() == "sin(x)");

  expression = Expression("Sin(x)");
  REQUIRE(expression.getString() == "sin(x)");

  expression = Expression("SiN(x)");
  REQUIRE(expression.getString() == "sin(x)");

  expression = Expression("Sin(x + y)");
  REQUIRE(expression.getString() == "sin(x+y)");

  expression = Expression("Sin(x * y)");
  REQUIRE(expression.getString() == "sin(x*y)");

  expression = Expression("sin(sin(-sin(x) + sin(-x)))");
  REQUIRE(expression.getString() == "sin(sin(-sin(x)+sin(-x)))");
}

TEST_CASE("Cos creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("cos(x)");
  REQUIRE(expression.getString() == "cos(x)");

  expression = Expression("Cos(x)");
  REQUIRE(expression.getString() == "cos(x)");

  expression = Expression("CoS(x)");
  REQUIRE(expression.getString() == "cos(x)");

  expression = Expression("Cos(x + y)");
  REQUIRE(expression.getString() == "cos(x+y)");

  expression = Expression("Cos(x * y)");
  REQUIRE(expression.getString() == "cos(x*y)");

  expression = Expression("cos(cos(-cos(x) + cos(-x)))");
  REQUIRE(expression.getString() == "cos(cos(-cos(x)+cos(-x)))");
}

TEST_CASE("Tan creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("tan(x)");
  REQUIRE(expression.getString() == "sin(x)*cos(x)^-1");

  expression = Expression("Tan(x)");
  REQUIRE(expression.getString() == "sin(x)*cos(x)^-1");

  expression = Expression("TaN(x)");
  REQUIRE(expression.getString() == "sin(x)*cos(x)^-1");

  expression = Expression("Tan(x + y)");
  REQUIRE(expression.getString() == "sin(x+y)*cos(x+y)^-1");

  expression = Expression("taN(x * y)");
  REQUIRE(expression.getString() == "sin(x*y)*cos(x*y)^-1");

  expression = Expression("tan(tan(x))");
  REQUIRE(expression.getString() == "sin(sin(x)*cos(x)^-1)*cos(sin(x)*cos(x)^-1)^-1");
}

TEST_CASE("Cot creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("cot(x)");
  REQUIRE(expression.getString() == "cos(x)*sin(x)^-1");

  expression = Expression("Cot(x)");
  REQUIRE(expression.getString() == "cos(x)*sin(x)^-1");

  expression = Expression("Cot(x)");
  REQUIRE(expression.getString() == "cos(x)*sin(x)^-1");

  expression = Expression("Cot(x + y)");
  REQUIRE(expression.getString() == "cos(x+y)*sin(x+y)^-1");

  expression = Expression("coT(x * y)");
  REQUIRE(expression.getString() == "cos(x*y)*sin(x*y)^-1");

  expression = Expression("cot(cot(x))");
  REQUIRE(expression.getString() == "cos(cos(x)*sin(x)^-1)*sin(cos(x)*sin(x)^-1)^-1");
}

TEST_CASE("Log creating", "[symbolicAlgebra]")  // two argument functions must have arguments in parentheses
{
  Expression expression;

  expression = Expression("log(x, y)");
  REQUIRE(expression.getString() == "log_(x)(y)");

  expression = Expression("Log(x, y)");
  REQUIRE(expression.getString() == "log_(x)(y)");

  expression = Expression("LoG(x, y)");
  REQUIRE(expression.getString() == "log_(x)(y)");

  expression = Expression("Log((x + y), (3*a))");
  REQUIRE(expression.getString() == "log_(x+y)(3*a)");

  expression = Expression("Log((3*a), (x + y))");
  REQUIRE(expression.getString() == "log_(3*a)(x+y)");

  expression = Expression("log(-log((x*y), (a*b)) , log((-a*b), (-x*y)))");
  REQUIRE(expression.getString() == "log_(log_(x*y)(a*b))(-log_(-a*b)(-x*y))");
}

TEST_CASE("Ln creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("ln(x)");
  REQUIRE(expression.getString() == "log_(e)(x)");

  expression = Expression("Ln(x)");
  REQUIRE(expression.getString() == "log_(e)(x)");

  expression = Expression("LN(x)");
  REQUIRE(expression.getString() == "log_(e)(x)");

  expression = Expression("Ln(x + y)");
  REQUIRE(expression.getString() == "log_(e)(x+y)");

  expression = Expression("Ln(3*a)");
  REQUIRE(expression.getString() == "log_(e)(3*a)");

  expression = Expression("ln(ln(-ln(x) + ln(-x)))");
  REQUIRE(expression.getString() == "log_(e)(log_(e)(-log_(e)(x)+log_(e)(-x)))");
}

TEST_CASE("Sqrt creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("sqrt(x)");
  REQUIRE(expression.getString() == "x^(1*2^-1)");

  expression = Expression("Sqrt(x)");
  REQUIRE(expression.getString() == "x^(1*2^-1)");

  expression = Expression("SqRt(x)");
  REQUIRE(expression.getString() == "x^(1*2^-1)");

  expression = Expression("SqrT(x + y)");
  REQUIRE(expression.getString() == "(x+y)^(1*2^-1)");

  expression = Expression("SqrT(3*a)");
  REQUIRE(expression.getString() == "(3*a)^(1*2^-1)");

  expression = Expression("sqrt(sqrt(-sqrt(x) + sqrt(-x)))");
  REQUIRE(expression.getString() == "((-x^(1*2^-1)+(-x)^(1*2^-1))^(1*2^-1))^(1*2^-1)");
}

TEST_CASE("Exp creating", "[symbolicAlgebra]") {
  Expression expression;

  expression = Expression("exp(x)");
  REQUIRE(expression.getString() == "e^x");

  expression = Expression("Exp(x)");
  REQUIRE(expression.getString() == "e^x");

  expression = Expression("ExP(x)");
  REQUIRE(expression.getString() == "e^x");

  expression = Expression("eXp(x + y)");
  REQUIRE(expression.getString() == "e^(x+y)");

  expression = Expression("eXp(3*a)");
  REQUIRE(expression.getString() == "e^(3*a)");

  expression = Expression("exp(exp(-exp(x) + exp(-x)))");
  REQUIRE(expression.getString() == "e^(e^(-e^x+e^(-x)))");
}