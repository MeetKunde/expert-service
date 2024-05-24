#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Sin printing", "[symbolicAlgebra]") {
  const Sin sinAtom1(std::make_unique<Var>("x"));
  const Sin sinAtom2(std::make_unique<Const>("pi"));
  constexpr float sinAtom3Value = 1.5;
  const Sin sinAtom3(std::make_unique<Number>(sinAtom3Value));

  std::stringstream streamOutput;

  sinAtom1.print(streamOutput);
  REQUIRE(streamOutput.str() == "sin(x)");
  streamOutput.str("");

  sinAtom2.print(streamOutput);
  REQUIRE(streamOutput.str() == "sin(pi)");
  streamOutput.str("");

  sinAtom3.print(streamOutput);
  REQUIRE(streamOutput.str() == "sin(1.5)");
}

TEST_CASE("Sin evaluating", "[symbolicAlgebra]") {
  const Sin sinAtom1(std::make_unique<Var>("x"));
  const Sin sinAtom2(std::make_unique<Number>(0));
  const Sin sinAtom3(std::make_unique<Number>(PI));
  const Sin sinAtom4(std::make_unique<Number>(PI_HALF));

  REQUIRE_FALSE(sinAtom1.canBeEvaluated());

  REQUIRE(sinAtom2.canBeEvaluated());
  REQUIRE_THAT(sinAtom2.evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
  REQUIRE(sinAtom3.canBeEvaluated());
  REQUIRE_THAT(sinAtom3.evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
  REQUIRE(sinAtom4.canBeEvaluated());
  REQUIRE_THAT(sinAtom4.evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));
}

TEST_CASE("Sin differentiating", "[symbolicAlgebra]") {
  const Sin sinAtom(std::make_unique<Var>("x"));
  const Sin sinAtom2(std::make_unique<Sin>(std::make_unique<Var>("x")));
  std::stringstream streamOutput;

  const std::unique_ptr<Atom> diff1 = sinAtom.differentiate("x");
  diff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(x)*1");
  streamOutput.str("");

  const std::unique_ptr<Atom> diff2 = sinAtom.differentiate("y");
  diff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(x)*0");
  streamOutput.str("");

  const std::unique_ptr<Atom> diff3 = sinAtom2.differentiate("x");
  diff3->print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(sin(x))*cos(x)*1");
}

TEST_CASE("Sin simplifying", "[symbolicAlgebra]") {
  std::stringstream streamOutput;

  const Sin sinAtom1(std::make_unique<Number>(0));
  const std::unique_ptr<Atom> simplifiedAtom1 = sinAtom1.simplify();
  REQUIRE(simplifiedAtom1->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(simplifiedAtom1->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));

  const Sin sinAtom2(std::make_unique<Product>(std::make_unique<Number>(-2), std::make_unique<Var>("x")));
  const std::unique_ptr<Atom> simplifiedAtom2 = sinAtom2.simplify();

  simplifiedAtom2->print(streamOutput);
  REQUIRE(streamOutput.str() == "-sin(2*x)");
  streamOutput.str("");

  const Sin sinAtom3(std::make_unique<Var>("x"));
  const std::unique_ptr<Atom> simplifiedAtom3 = sinAtom3.simplify();
  simplifiedAtom3->print(streamOutput);
  REQUIRE(streamOutput.str() == "sin(x)");
}

TEST_CASE("Cos printing", "[symbolicAlgebra]") {
  const Cos cosAtom1(std::make_unique<Var>("x"));
  const Cos cosAtom2(std::make_unique<Const>("pi"));
  constexpr float cosAtom3Value = 1.5;
  const Cos cosAtom3(std::make_unique<Number>(cosAtom3Value));

  std::stringstream streamOutput;

  cosAtom1.print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(x)");
  streamOutput.str("");

  cosAtom2.print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(pi)");
  streamOutput.str("");

  cosAtom3.print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(1.5)");
}

TEST_CASE("Cos evaluating", "[symbolicAlgebra]") {
  const Cos cosAtom1(std::make_unique<Var>("x"));
  const Cos cosAtom2(std::make_unique<Number>(0));
  const Cos cosAtom3(std::make_unique<Number>(PI));
  const Cos cosAtom4(std::make_unique<Number>(PI_HALF));

  REQUIRE_FALSE(cosAtom1.canBeEvaluated());

  REQUIRE(cosAtom2.canBeEvaluated());
  REQUIRE_THAT(cosAtom2.evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));
  REQUIRE(cosAtom3.canBeEvaluated());
  REQUIRE_THAT(cosAtom3.evaluate(), Catch::Matchers::WithinAbs(-1.0, EPSILON));
  REQUIRE(cosAtom4.canBeEvaluated());
  REQUIRE_THAT(cosAtom4.evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Cos differentiating", "[symbolicAlgebra]") {
  const Cos cosAtom(std::make_unique<Var>("x"));
  const Cos cosAtom2(std::make_unique<Cos>(std::make_unique<Var>("x")));
  std::stringstream streamOutput;

  const std::unique_ptr<Atom> diff1 = cosAtom.differentiate("x");
  diff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "-sin(x)*1");
  streamOutput.str("");

  const std::unique_ptr<Atom> diff2 = cosAtom.differentiate("y");
  diff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "-sin(x)*0");
  streamOutput.str("");

  const std::unique_ptr<Atom> diff3 = cosAtom2.differentiate("x");
  diff3->print(streamOutput);
  REQUIRE(streamOutput.str() == "1*sin(cos(x))*1*sin(x)*1");
}

TEST_CASE("Cos simplifying", "[symbolicAlgebra]") {
  std::stringstream streamOutput;

  const Cos cosAtom1(std::make_unique<Number>(0));
  const std::unique_ptr<Atom> simplifiedAtom1 = cosAtom1.simplify();
  REQUIRE(simplifiedAtom1->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(simplifiedAtom1->evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));

  const Cos cosAtom2(std::make_unique<Product>(std::make_unique<Number>(-2), std::make_unique<Var>("x")));
  const std::unique_ptr<Atom> simplifiedAtom2 = cosAtom2.simplify();

  simplifiedAtom2->print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(2*x)");
  streamOutput.str("");

  const Cos sinAtom3(std::make_unique<Var>("x"));
  const std::unique_ptr<Atom> simplifiedAtom3 = sinAtom3.simplify();
  simplifiedAtom3->print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(x)");
}

// Tan(x) is product of Sin(x), Cos(x)^-1 so tested no there
// Cot(x) is product of Cos(x), Sin(x)^-1 so tested no there

TEST_CASE("Log printing", "[symbolicAlgebra]") {
  const Log logAtom1(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const Log logAtom2(std::make_unique<Const>("e"), std::make_unique<Const>("pi"));
  constexpr float logAtom3BaseValue = 2.1;
  constexpr float logAtom3ArgValue = 1.5;
  const Log logAtom3(std::make_unique<Number>(logAtom3BaseValue), std::make_unique<Number>(logAtom3ArgValue));

  std::stringstream streamOutput;

  logAtom1.print(streamOutput);
  REQUIRE(streamOutput.str() == "log_(x)(y)");
  streamOutput.str("");

  logAtom2.print(streamOutput);
  REQUIRE(streamOutput.str() == "log_(e)(pi)");
  streamOutput.str("");

  logAtom3.print(streamOutput);
  REQUIRE(streamOutput.str() == "log_(2.1)(1.5)");
}

TEST_CASE("Log evaluating", "[symbolicAlgebra]") {
  constexpr int logBaseVal = 5;
  constexpr float logArg1 = 0.2;
  constexpr int logArg2 = 1;
  constexpr float logArg3 = 0.2;
  constexpr float logArg4 = 25.0;
  constexpr float logArg5 = 5.0;
  constexpr float logRes2 = 0.0;
  constexpr float logRes3 = -1.0;
  constexpr float logRes4 = 2.0;
  constexpr float logRes5 = 1.0;

  const Log logAtom1(std::make_unique<Var>("x"), std::make_unique<Number>(logArg1));
  const Log logAtom2(std::make_unique<Number>(logBaseVal), std::make_unique<Number>(logArg2));
  const Log logAtom3(std::make_unique<Number>(logBaseVal), std::make_unique<Number>(logArg3));
  const Log logAtom4(std::make_unique<Number>(logBaseVal), std::make_unique<Number>(logArg4));
  const Log logAtom5(std::make_unique<Number>(logBaseVal), std::make_unique<Number>(logArg5));

  REQUIRE_FALSE(logAtom1.canBeEvaluated());

  REQUIRE(logAtom2.canBeEvaluated());
  REQUIRE_THAT(logAtom2.evaluate(), Catch::Matchers::WithinAbs(logRes2, EPSILON));
  REQUIRE(logAtom3.canBeEvaluated());
  REQUIRE_THAT(logAtom3.evaluate(), Catch::Matchers::WithinAbs(logRes3, EPSILON));
  REQUIRE(logAtom4.canBeEvaluated());
  REQUIRE_THAT(logAtom4.evaluate(), Catch::Matchers::WithinAbs(logRes4, EPSILON));
  REQUIRE_THAT(logAtom5.evaluate(), Catch::Matchers::WithinAbs(logRes5, EPSILON));
}

TEST_CASE("Log differentiating", "[symbolicAlgebra]") {
  const Log logAtom1(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> diff1 = logAtom1.differentiate("x");
  const std::unique_ptr<Atom> diff2 = logAtom1.differentiate("y");
  const std::unique_ptr<Atom> diff3 = logAtom1.differentiate("z");

  std::stringstream streamOutput;

  diff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "(log_(e)(x)*0*y^-1-log_(e)(y)*1*x^-1)*(log_(e)(x)^2)^-1");
  streamOutput.str("");

  diff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "(log_(e)(x)*1*y^-1-log_(e)(y)*0*x^-1)*(log_(e)(x)^2)^-1");
  streamOutput.str("");

  diff3->print(streamOutput);
  REQUIRE(streamOutput.str() == "(log_(e)(x)*0*y^-1-log_(e)(y)*0*x^-1)*(log_(e)(x)^2)^-1");
}

TEST_CASE("Log simplifying", "[symbolicAlgebra]") {
  const Log logAtom1(std::make_unique<Const>("e"), std::make_unique<Number>(1));
  const Log logAtom2(std::make_unique<Var>("x"), std::make_unique<Var>("x"));
  const Log logAtom3(std::make_unique<Const>("e"),
                     std::make_unique<Power>(std::make_unique<Const>("e"), std::make_unique<Var>("x")));
  const Log logAtom4(std::make_unique<Var>("x"), std::make_unique<Var>("y"));

  const std::unique_ptr<Atom> diff1 = logAtom1.simplify();
  const std::unique_ptr<Atom> diff2 = logAtom2.simplify();
  const std::unique_ptr<Atom> diff3 = logAtom3.simplify();
  const std::unique_ptr<Atom> diff4 = logAtom4.simplify();

  std::stringstream streamOutput;

  diff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "0");
  streamOutput.str("");

  diff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "1");
  streamOutput.str("");

  diff3->print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");

  diff4->print(streamOutput);
  REQUIRE(streamOutput.str() == "log_(x)(y)");
}

// Sqrt(x) is Product of x, 2^-1 so tested no there