#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Number creating", "[symbolicAlgebra]") {
  constexpr int intVal = 17;
  constexpr float floatVal = 3.14;

  const Number num1(intVal);
  const Number num2(floatVal);

  REQUIRE_THAT(num1.evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));
  REQUIRE_THAT(num2.evaluate(), Catch::Matchers::WithinAbs(floatVal, EPSILON));
}

TEST_CASE("Number copying", "[symbolicAlgebra]") {
  constexpr int intVal = 17;
  const Number numAtom(intVal);

  const std::unique_ptr<Atom> numAtomCopy1 = numAtom.copy();
  const std::unique_ptr<Atom> numAtomCopy2 = numAtom.copy();

  const Number* atom1 = static_cast<Number*>(numAtomCopy1.get());
  const Number* atom2 = static_cast<Number*>(numAtomCopy2.get());

  REQUIRE_THAT(atom1->evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));
  REQUIRE_THAT(atom2->evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));
  REQUIRE(&numAtomCopy1 != &numAtomCopy2);
}

TEST_CASE("Number printing", "[symbolicAlgebra]") {
  constexpr int intVal = 17;
  const Number numAtom(intVal);

  std::stringstream streamOutput;
  numAtom.print(streamOutput);
  REQUIRE(streamOutput.str() == "17");
}

TEST_CASE("Number evaluating", "[symbolicAlgebra]") {
  constexpr int intVal = 17;
  constexpr float floatVal = 3.14;

  const Number num1(intVal);
  const Number num2(floatVal);

  REQUIRE(num1.canBeEvaluated());
  REQUIRE_THAT(num1.evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));

  REQUIRE(num2.canBeEvaluated());
  REQUIRE_THAT(num2.evaluate(), Catch::Matchers::WithinAbs(floatVal, EPSILON));
}

TEST_CASE("Number substituting", "[symbolicAlgebra]") {
  constexpr int intVal = 17;
  constexpr int newVal = 19;
  const Number numAtom(intVal);

  const std::unique_ptr<Atom> numAtomSubst = numAtom.substitute("a", std::make_unique<Number>(newVal));
  Number* atom = static_cast<Number*>(numAtomSubst.get());

  REQUIRE(&numAtom != atom);
  REQUIRE_THAT(atom->evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));
}

TEST_CASE("Getting variable names from number", "[symbolicAlgebra]") {
  constexpr int intVal = 19;
  const Number numAtom(intVal);

  std::set<std::string> variablesNames;
  numAtom.getVariablesNames(variablesNames);

  REQUIRE(variablesNames.empty());
}

TEST_CASE("Number differentiating", "[symbolicAlgebra]") {
  constexpr int intVal = 19;
  const Number numAtom(intVal);

  const std::unique_ptr<Atom> numDiff = numAtom.differentiate("e");
  REQUIRE(numDiff->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(numDiff->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Number simplifying", "[symbolicAlgebra]") {
  constexpr int intVal = 19;
  Number numAtom(intVal);

  const std::unique_ptr<Atom> numAtomSimplified = numAtom.simplify();
  Number* atom = static_cast<Number*>(numAtomSimplified.get());

  REQUIRE(&numAtom != atom);
  REQUIRE_THAT(atom->evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));
}

TEST_CASE("Number expanding", "[symbolicAlgebra]") {
  constexpr int intVal = 19;
  Number numAtom(intVal);

  const std::unique_ptr<Atom> numAtomExpanded = numAtom.expand();
  Number* atom = static_cast<Number*>(numAtomExpanded.get());

  REQUIRE(&numAtom != atom);
  REQUIRE_THAT(atom->evaluate(), Catch::Matchers::WithinAbs(intVal, EPSILON));
}

TEST_CASE("Number comparing", "[symbolicAlgebra]") {
  constexpr int intVal1 = 17;
  constexpr int intVal2 = 19;

  const std::unique_ptr<Atom> numAtom1 = std::make_unique<Number>(intVal1);
  const std::unique_ptr<Atom> numAtom2 = std::make_unique<Number>(intVal1);
  const std::unique_ptr<Atom> numAtom3 = std::make_unique<Number>(intVal2);
  const std::unique_ptr<Atom> varAtom = std::make_unique<Var>("x");

  REQUIRE(numAtom1->compare(numAtom2));
  REQUIRE_FALSE(numAtom1->compare(numAtom3));
  REQUIRE_FALSE(numAtom3->compare(varAtom));
}

TEST_CASE("Number coefficient getting", "[symbolicAlgebra]") {
  constexpr int intVal1 = 17;
  constexpr int intVal2 = 19;

  const std::unique_ptr<Atom> numAtom1 = std::make_unique<Number>(intVal1);
  const std::unique_ptr<Atom> numAtom2 = std::make_unique<Number>(intVal1);
  const std::unique_ptr<Atom> numAtom3 = std::make_unique<Number>(intVal2);

  const std::unique_ptr<Atom> coeff1 = numAtom1->coefficient(numAtom2);
  const std::unique_ptr<Atom> coeff2 = numAtom1->coefficient(numAtom3);

  REQUIRE(coeff1->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff1->evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(coeff2->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff2->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Number operations", "[symbolicAlgebra]") {
  const Number rat1(-3.14);
  const Number rat2(2.73);

  const Number int1(3);
  const Number int2(-4);

  const Number frac1(-3, 4);
  const Number frac2(5, -8);

  Number res(0);
  std::stringstream streamOutput;

  // ADDITION

  res = rat1 + rat2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-0.41");
  streamOutput.str("");

  res = rat1 + int1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-0.14");
  streamOutput.str("");

  res = int1 + int2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-1");
  streamOutput.str("");

  res = frac1 + frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-11/8");
  streamOutput.str("");

  res = int1 + frac1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "9/4");
  streamOutput.str("");

  res = int2 + frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-37/8");
  streamOutput.str("");

  // SUBTRACTION

  res = rat1 - rat2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-5.87");
  streamOutput.str("");

  res = rat1 - int1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-6.14");
  streamOutput.str("");

  res = int1 - int2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "7");
  streamOutput.str("");

  res = frac1 - frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-1/8");
  streamOutput.str("");

  res = int1 - frac1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "15/4");
  streamOutput.str("");

  res = int2 - frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-27/8");
  streamOutput.str("");

  // MULTIPLICATION

  res = rat1 * rat2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-8.5722");
  streamOutput.str("");

  res = rat1 * int1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-9.42");
  streamOutput.str("");

  res = int1 * int2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-12");
  streamOutput.str("");

  res = frac1 * frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "15/32");
  streamOutput.str("");

  res = int1 * frac1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-9/4");
  streamOutput.str("");

  res = int2 * frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "5/2");
  streamOutput.str("");

  // DIVISION

  res = rat1 / rat2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-1.15018");
  streamOutput.str("");

  res = rat1 / int1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-1.04667");
  streamOutput.str("");

  res = int1 / int2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-3/4");
  streamOutput.str("");

  res = frac1 / frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "6/5");
  streamOutput.str("");

  res = int1 / frac1;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "-4");
  streamOutput.str("");

  res = int2 / frac2;
  res.print(streamOutput);
  REQUIRE(streamOutput.str() == "32/5");
  streamOutput.str("");
}