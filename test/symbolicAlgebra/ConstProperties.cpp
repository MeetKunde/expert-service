#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Const creating", "[symbolicAlgebra]") {
  const Const constAtom("e");
  REQUIRE(constAtom.getName() == "e");
}

TEST_CASE("Const copying", "[symbolicAlgebra]") {
  const Const constAtom("e");

  const std::unique_ptr<Atom> constAtomCopy1 = constAtom.copy();
  const std::unique_ptr<Atom> constAtomCopy2 = constAtom.copy();

  const Const* atom1 = static_cast<Const*>(constAtomCopy1.get());
  const Const* atom2 = static_cast<Const*>(constAtomCopy2.get());

  REQUIRE(atom1->getName() == "e");
  REQUIRE(atom2->getName() == "e");
  REQUIRE(&constAtomCopy1 != &constAtomCopy2);
}

TEST_CASE("Const printing", "[symbolicAlgebra]") {
  const Const constAtom("e");
  std::stringstream streamOutput;
  constAtom.print(streamOutput);
  REQUIRE(streamOutput.str() == "e");
}

TEST_CASE("Const evaluating", "[symbolicAlgebra]") {
  const Const constAtom("e");
  REQUIRE_FALSE(constAtom.canBeEvaluated());
  REQUIRE_THAT(constAtom.evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Const substituting", "[symbolicAlgebra]") {
  const Const constAtom("e");

  const std::unique_ptr<Atom> constAtomSubst = constAtom.substitute("e", std::make_unique<Const>("o"));
  Const* atom = static_cast<Const*>(constAtomSubst.get());

  REQUIRE(&constAtom != atom);
  REQUIRE(atom->getName() == "e");
}

TEST_CASE("Getting variable names from const", "[symbolicAlgebra]") {
  const Const constAtom("e");
  std::set<std::string> variablesNames;
  constAtom.getVariablesNames(variablesNames);

  REQUIRE(variablesNames.empty());
}

TEST_CASE("Const differentiating", "[symbolicAlgebra]") {
  const Const constAtom("e");
  const std::unique_ptr<Atom> constDiff = constAtom.differentiate("e");
  REQUIRE(constDiff->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(constDiff->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Const simplifying", "[symbolicAlgebra]") {
  Const constAtom("e");

  const std::unique_ptr<Atom> constAtomSimplified = constAtom.simplify();
  Const* atom = static_cast<Const*>(constAtomSimplified.get());

  REQUIRE(&constAtom != atom);
  REQUIRE(atom->getName() == "e");
}

TEST_CASE("Const expanding", "[symbolicAlgebra]") {
  Const constAtom("e");

  const std::unique_ptr<Atom> constAtomExpanded = constAtom.expand();
  Const* atom = static_cast<Const*>(constAtomExpanded.get());

  REQUIRE(&constAtom != atom);
  REQUIRE(atom->getName() == "e");
}

TEST_CASE("Const comparing", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> constAtom1 = std::make_unique<Const>("a");
  const std::unique_ptr<Atom> constAtom2 = std::make_unique<Const>("a");
  const std::unique_ptr<Atom> constAtom3 = std::make_unique<Const>("b");
  const std::unique_ptr<Atom> varAtom = std::make_unique<Var>("b");

  REQUIRE(constAtom1->compare(constAtom2));
  REQUIRE_FALSE(constAtom1->compare(constAtom3));
  REQUIRE_FALSE(constAtom3->compare(varAtom));
}

TEST_CASE("Const coefficient getting", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> constAtom1 = std::make_unique<Const>("a");
  const std::unique_ptr<Atom> constAtom2 = std::make_unique<Const>("a");
  const std::unique_ptr<Atom> constAtom3 = std::make_unique<Const>("b");

  const std::unique_ptr<Atom> coeff1 = constAtom1->coefficient(constAtom2);
  const std::unique_ptr<Atom> coeff2 = constAtom1->coefficient(constAtom3);

  REQUIRE(coeff1->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff1->evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(coeff2->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff2->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}