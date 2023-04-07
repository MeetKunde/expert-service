#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Var creating", "[symbolicAlgebra]") {
  const Var varAtom("a");
  REQUIRE(varAtom.getName() == "a");
}

TEST_CASE("Var copying", "[symbolicAlgebra]") {
  const Var varAtom("a");

  const std::unique_ptr<Atom> varAtomCopy1 = varAtom.copy();
  const std::unique_ptr<Atom> varAtomCopy2 = varAtom.copy();

  Var* atom1 = dynamic_cast<Var*>(varAtomCopy1.get());
  Var* atom2 = dynamic_cast<Var*>(varAtomCopy2.get());

  REQUIRE(atom1->getName() == "a");
  REQUIRE(atom2->getName() == "a");
  REQUIRE(&varAtomCopy1 != &varAtomCopy2);
}

TEST_CASE("Var printing", "[symbolicAlgebra]") {
  const Var varAtom("a");
  std::stringstream streamOutput;
  varAtom.print(streamOutput);
  REQUIRE(streamOutput.str() == "a");
}

TEST_CASE("Var evaluating", "[symbolicAlgebra]") {
  const Var varAtom("a");
  REQUIRE_FALSE(varAtom.canBeEvaluated());
  REQUIRE_THAT(varAtom.evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Var substituting", "[symbolicAlgebra]") {
  const Var varAtom1("a");
  const Var varAtom2("a");

  const std::unique_ptr<Atom> varAtomSubst1 = varAtom1.substitute("a", std::make_unique<Var>("x"));
  const std::unique_ptr<Atom> varAtomSubst2 = varAtom2.substitute("b", std::make_unique<Var>("x"));
  Var* atom1 = static_cast<Var*>(varAtomSubst1.get());
  Var* atom2 = static_cast<Var*>(varAtomSubst2.get());

  REQUIRE(atom1->getName() == "x");
  REQUIRE(atom2->getName() == "a");
}

TEST_CASE("Getting variable names from vars", "[symbolicAlgebra]") {
  const Var varAtom1("a");
  const Var varAtom2("b");
  const Var varAtom3("b");
  std::set<std::string> variablesNames;
  varAtom1.getVariablesNames(variablesNames);
  varAtom2.getVariablesNames(variablesNames);
  varAtom3.getVariablesNames(variablesNames);

  REQUIRE(variablesNames.size() == 2);
  REQUIRE(variablesNames.find("a") != variablesNames.end());
  REQUIRE(variablesNames.find("b") != variablesNames.end());
}

TEST_CASE("Var differentiating", "[symbolicAlgebra]") {
  const Var varAtom("a");
  const std::unique_ptr<Atom> varDiff1 = varAtom.differentiate("a");
  const std::unique_ptr<Atom> varDiff2 = varAtom.differentiate("x");

  REQUIRE(varDiff1->type == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(varDiff1->evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(varDiff2->type == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(varDiff2->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}

TEST_CASE("Var simplifying", "[symbolicAlgebra]") {
  const Var varAtom("a");

  const std::unique_ptr<Atom> varAtomSimplified = varAtom.simplify();
  Var* atom = static_cast<Var*>(varAtomSimplified.get());

  REQUIRE(&varAtom != atom);
  REQUIRE(atom->getName() == "a");
}

TEST_CASE("Var expanding", "[symbolicAlgebra]") {
  const Var varAtom("a");

  const std::unique_ptr<Atom> varAtomExpanded = varAtom.expand();
  Var* atom = static_cast<Var*>(varAtomExpanded.get());

  REQUIRE(&varAtom != atom);
  REQUIRE(atom->getName() == "a");
}

TEST_CASE("Var comparing", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> varAtom1 = std::make_unique<Var>("a");
  const std::unique_ptr<Atom> varAtom2 = std::make_unique<Var>("a");
  const std::unique_ptr<Atom> varAtom3 = std::make_unique<Var>("b");
  const std::unique_ptr<Atom> varAtom = std::make_unique<Const>("b");

  REQUIRE(varAtom1->compare(varAtom2));
  REQUIRE_FALSE(varAtom1->compare(varAtom3));
  REQUIRE_FALSE(varAtom3->compare(varAtom));
}

TEST_CASE("Var coefficient getting", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> varAtom1 = std::make_unique<Var>("a");
  const std::unique_ptr<Atom> varAtom2 = std::make_unique<Var>("a");
  const std::unique_ptr<Atom> varAtom3 = std::make_unique<Var>("b");

  const std::unique_ptr<Atom> coeff1 = varAtom1->coefficient(varAtom2);
  const std::unique_ptr<Atom> coeff2 = varAtom1->coefficient(varAtom3);

  REQUIRE(coeff1->type == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff1->evaluate(), Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(coeff2->type == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(coeff2->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));
}