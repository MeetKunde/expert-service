#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Power printing", "[symbolicAlgebra]") {
  const Power powerAtom1(std::make_unique<Var>("a"), std::make_unique<Var>("b"));
  const Power powerAtom2(std::make_unique<Number>(2), std::make_unique<Number>(3));
  const Power powerAtom3(std::make_unique<Const>("e"), std::make_unique<Const>("pi"));
  const Power powerAtom4(std::make_unique<Sum>(std::make_unique<Var>("x"), std::make_unique<Var>("y")),
                         std::make_unique<Sum>(std::make_unique<Var>("a"), std::make_unique<Var>("b")));
  const Power powerAtom5(std::make_unique<Product>(std::make_unique<Var>("x"), std::make_unique<Var>("y")),
                         std::make_unique<Product>(std::make_unique<Var>("a"), std::make_unique<Var>("b")));
  const Power powerAtom6(std::make_unique<Power>(std::make_unique<Var>("x"), std::make_unique<Var>("y")),
                         std::make_unique<Power>(std::make_unique<Var>("a"), std::make_unique<Var>("b")));
  const Power powerAtom7(std::make_unique<Sin>(std::make_unique<Var>("x")), std::make_unique<Sin>(std::make_unique<Var>("y")));
  const Power powerAtom8(std::make_unique<Cos>(std::make_unique<Var>("x")), std::make_unique<Cos>(std::make_unique<Var>("y")));
  const Power powerAtom9(std::make_unique<Log>(std::make_unique<Var>("x"), std::make_unique<Var>("y")),
                         std::make_unique<Log>(std::make_unique<Var>("a"), std::make_unique<Var>("b")));

  std::stringstream streamOutput;

  powerAtom1.print(streamOutput);
  REQUIRE(streamOutput.str() == "a^b");
  streamOutput.str("");

  powerAtom2.print(streamOutput);
  REQUIRE(streamOutput.str() == "2^3");
  streamOutput.str("");

  powerAtom3.print(streamOutput);
  REQUIRE(streamOutput.str() == "e^pi");
  streamOutput.str("");

  powerAtom4.print(streamOutput);
  REQUIRE(streamOutput.str() == "(x+y)^(a+b)");
  streamOutput.str("");

  powerAtom5.print(streamOutput);
  REQUIRE(streamOutput.str() == "(x*y)^(a*b)");
  streamOutput.str("");

  powerAtom6.print(streamOutput);
  REQUIRE(streamOutput.str() == "(x^y)^(a^b)");
  streamOutput.str("");

  powerAtom7.print(streamOutput);
  REQUIRE(streamOutput.str() == "sin(x)^sin(y)");
  streamOutput.str("");

  powerAtom8.print(streamOutput);
  REQUIRE(streamOutput.str() == "cos(x)^cos(y)");
  streamOutput.str("");

  powerAtom9.print(streamOutput);
  REQUIRE(streamOutput.str() == "log_(x)(y)^log_(a)(b)");
  streamOutput.str("");
}

TEST_CASE("Power copying", "[symbolicAlgebra]") {
  Power powerAtom(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> powerAtomCopy = powerAtom.copy();

  REQUIRE(&powerAtom.args.front() != &powerAtomCopy->args.front());
  REQUIRE(&powerAtom.args.back() != &powerAtomCopy->args.back());
}

TEST_CASE("Power evaluating", "[symbolicAlgebra]") {
  constexpr float val1 = 3.6;
  constexpr float val2 = 7.8;
  constexpr float res = 21835.3;

  const Power powerAtom1(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const Power powerAtom2(std::make_unique<Var>("x"), std::make_unique<Number>(val1));
  const Power powerAtom3(std::make_unique<Number>(val2), std::make_unique<Var>("y"));
  const Power powerAtom4(std::make_unique<Number>(val1), std::make_unique<Number>(val2));

  REQUIRE_FALSE(powerAtom1.canBeEvaluated());
  REQUIRE_FALSE(powerAtom2.canBeEvaluated());
  REQUIRE_FALSE(powerAtom3.canBeEvaluated());
  REQUIRE_THAT(powerAtom4.evaluate(), Catch::Matchers::WithinAbs(res, BIG_EPSILON));
}

TEST_CASE("Power substituting", "[symbolicAlgebra]") {
  const Power powerAtom(std::make_unique<Var>("x"), std::make_unique<Var>("x"));
  const std::unique_ptr<Atom> powerAtomSubst = powerAtom.substitute("x", std::make_unique<Var>("o"));

  std::stringstream streamOutput;

  powerAtomSubst->print(streamOutput);
  REQUIRE(streamOutput.str() == "o^o");
}

TEST_CASE("Getting variable names from power", "[symbolicAlgebra]") {
  const Power powerAtom(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  std::set<std::string> variableNames;
  powerAtom.getVariablesNames(variableNames);

  REQUIRE(variableNames.size() == 2);
  REQUIRE(variableNames.find("x") != variableNames.end());
  REQUIRE(variableNames.find("y") != variableNames.end());
}

TEST_CASE("Power differentiating", "[symbolicAlgebra]") {
  const Power powerAtom(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> powerAtomDiff1 = powerAtom.differentiate("x");
  const std::unique_ptr<Atom> powerAtomDiff2 = powerAtom.differentiate("y");

  std::stringstream streamOutput;

  powerAtomDiff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "x^(y-1)*(y*1+x*0*log_(e)(x))");
  streamOutput.str("");

  powerAtomDiff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "x^(y-1)*(y*0+x*1*log_(e)(x))");
}

TEST_CASE("Power simplifying", "[symbolicAlgebra]") {
  constexpr float piVal = 3.14;
  constexpr float eVal = 2.72;
  constexpr int fiveToPowerTwo = 25;
  const Power powerAtom1(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const Power powerAtom2(std::make_unique<Var>("x"), std::make_unique<Number>(0));
  const Power powerAtom3(std::make_unique<Var>("x"), std::make_unique<Number>(1));
  const Power powerAtom4(std::make_unique<Number>(0), std::make_unique<Var>("y"));
  const Power powerAtom5(std::make_unique<Number>(1), std::make_unique<Var>("y"));
  const Power powerAtom6(std::make_unique<Power>(std::make_unique<Var>("a"), std::make_unique<Var>("x")),
                         std::make_unique<Var>("y"));
  const Power powerAtom7(std::make_unique<Number>(3), std::make_unique<Number>(4));
  const Power powerAtom8(std::make_unique<Number>(piVal), std::make_unique<Number>(eVal));
  const Power powerAtom9(std::make_unique<Var>("x"),
                         std::make_unique<Log>(std::make_unique<Var>("x"), std::make_unique<Var>("y")));
  const Power powerAtom10(
      std::make_unique<Number>(fiveToPowerTwo),
      std::make_unique<Product>(std::make_unique<Number>(1),
                                std::make_unique<Power>(std::make_unique<Number>(2), std::make_unique<Number>(-1))));

  const std::unique_ptr<Atom> powerAtom1Simplified = powerAtom1.simplify();
  const std::unique_ptr<Atom> powerAtom2Simplified = powerAtom2.simplify();
  const std::unique_ptr<Atom> powerAtom3Simplified = powerAtom3.simplify();
  const std::unique_ptr<Atom> powerAtom4Simplified = powerAtom4.simplify();
  const std::unique_ptr<Atom> powerAtom5Simplified = powerAtom5.simplify();
  const std::unique_ptr<Atom> powerAtom6Simplified = powerAtom6.simplify();
  const std::unique_ptr<Atom> powerAtom7Simplified = powerAtom7.simplify();
  const std::unique_ptr<Atom> powerAtom8Simplified = powerAtom8.simplify();
  const std::unique_ptr<Atom> powerAtom9Simplified = powerAtom9.simplify();
  const std::unique_ptr<Atom> powerAtom10Simplified = powerAtom10.simplify();

  std::stringstream streamOutput;

  powerAtom1Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x^y");
  streamOutput.str("");

  powerAtom2Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "1");
  streamOutput.str("");

  powerAtom3Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");

  powerAtom4Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "0");
  streamOutput.str("");

  powerAtom5Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "1");
  streamOutput.str("");

  powerAtom6Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a^(x*y)");
  streamOutput.str("");

  powerAtom7Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "81");
  streamOutput.str("");

  powerAtom8Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "3.14^2.72");
  streamOutput.str("");

  powerAtom9Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "y");
  streamOutput.str("");

  powerAtom10Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "5");
  streamOutput.str("");
}

TEST_CASE("Power expanding", "[symbolicAlgebra]") {
  const Power powerAtom1(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const Power powerAtom2(std::make_unique<Var>("x"),
                         std::make_unique<Sum>(std::make_unique<Var>("a"), std::make_unique<Var>("b")));
  const Power powerAtom3(std::make_unique<Product>(std::make_unique<Var>("a"), std::make_unique<Var>("b")),
                         std::make_unique<Var>("x"));

  const std::unique_ptr<Atom> powerAtom1Expanded = powerAtom1.expand();
  const std::unique_ptr<Atom> powerAtom2Expanded = powerAtom2.expand();
  const std::unique_ptr<Atom> powerAtom3Expanded = powerAtom3.expand();

  std::stringstream streamOutput;

  powerAtom1Expanded->print(streamOutput);
  REQUIRE(streamOutput.str() == "x^y");
  streamOutput.str("");

  powerAtom2Expanded->print(streamOutput);
  REQUIRE(streamOutput.str() == "x^a*x^b");
  streamOutput.str("");

  powerAtom3Expanded->print(streamOutput);
  REQUIRE(streamOutput.str() == "a^x*b^x");
  streamOutput.str("");
}

TEST_CASE("Power comparing", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> powerAtom1 = std::make_unique<Power>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> powerAtom2 = std::make_unique<Power>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> powerAtom3 = std::make_unique<Power>(std::make_unique<Var>("z"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> powerAtom4 = std::make_unique<Power>(std::make_unique<Var>("x"), std::make_unique<Var>("z"));
  const std::unique_ptr<Atom> logAtom5 = std::make_unique<Log>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));

  REQUIRE(powerAtom1->compare(powerAtom2));
  REQUIRE_FALSE(powerAtom1->compare(powerAtom3));
  REQUIRE_FALSE(powerAtom1->compare(powerAtom4));
  REQUIRE_FALSE(powerAtom1->compare(logAtom5));
}

TEST_CASE("Power coefficient getting", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> powerAtom1 = std::make_unique<Power>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> powerAtom2 = std::make_unique<Power>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> logAtom3 = std::make_unique<Log>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));

  const std::unique_ptr<Atom> coeff1 = powerAtom1->coefficient(powerAtom2);
  const std::unique_ptr<Atom> coeff2 = powerAtom1->coefficient(logAtom3);

  REQUIRE(coeff1->type == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff1->evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(coeff2->type == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff2->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}