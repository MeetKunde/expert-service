#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Sum printing", "[symbolicAlgebra]") {
  constexpr int val1 = -3;
  constexpr int val2 = -7;
  constexpr int val3 = -11;
  constexpr int val4 = 13;
  constexpr int val5 = 19;
  const Sum sumAtom1 = Sum();
  const Sum sumAtom2(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> args3;
  args3.emplace_back(std::make_unique<Var>("x"));
  args3.emplace_back(std::make_unique<Var>("y"));
  args3.emplace_back(std::make_unique<Var>("z"));
  const Sum sumAtom3(std::move(args3));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Number>(val1));
  args4.emplace_back(std::make_unique<Number>(val2));
  args4.emplace_back(std::make_unique<Number>(val3));
  const Sum sumAtom4(std::move(args4));

  std::vector<std::unique_ptr<Atom>> args5;
  args5.emplace_back(std::make_unique<Number>(val4));
  args5.emplace_back(std::make_unique<Number>(val2));
  args5.emplace_back(std::make_unique<Number>(val5));
  const Sum sumAtom5(std::move(args5));

  std::stringstream streamOutput;

  sumAtom1.print(streamOutput);
  REQUIRE(streamOutput.str() == "0");
  streamOutput.str("");

  sumAtom2.print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");

  sumAtom3.print(streamOutput);
  REQUIRE(streamOutput.str() == "x+y+z");
  streamOutput.str("");

  sumAtom4.print(streamOutput);
  REQUIRE(streamOutput.str() == "-3-7-11");
  streamOutput.str("");

  sumAtom5.print(streamOutput);
  REQUIRE(streamOutput.str() == "13-7+19");
}

TEST_CASE("Sum copying", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("z"));
  const Sum sumAtom(std::move(args));
  const std::unique_ptr<Atom> sumAtomCopy = sumAtom.copy();

  for (auto iter1 = sumAtom.getArgs().cbegin(), iter2 = sumAtomCopy->getArgs().cbegin();
       iter1 != sumAtom.getArgs().cend() && iter2 != sumAtomCopy->getArgs().cend(); ++iter1, ++iter2) {
    REQUIRE(&(*iter1) != &(*iter2));
  }
}

TEST_CASE("Sum evaluating", "[symbolicAlgebra]") {
  constexpr float val1 = 2.5;
  constexpr int val2 = 2;
  constexpr float val3 = 1.2;
  constexpr float res1 = 2.5;
  constexpr float res2 = 5.7;

  const Sum sumAtom1 = Sum();
  const Sum sumAtom2 = Sum(std::make_unique<Number>(val1));
  const Sum sumAtom3 = Sum(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Number>(val1));
  args4.emplace_back(std::make_unique<Number>(val2));
  args4.emplace_back(std::make_unique<Number>(val3));
  const Sum sumAtom4 = Sum(std::move(args4));

  std::vector<std::unique_ptr<Atom>> args5;
  args5.emplace_back(std::make_unique<Number>(val1));
  args5.emplace_back(std::make_unique<Number>(val2));
  args5.emplace_back(std::make_unique<Var>("x"));
  const Sum sumAtom5 = Sum(std::move(args5));

  REQUIRE(sumAtom1.canBeEvaluated());
  REQUIRE_THAT(sumAtom1.evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));

  REQUIRE(sumAtom2.canBeEvaluated());
  REQUIRE_THAT(sumAtom2.evaluate(), Catch::Matchers::WithinAbs(res1, EPSILON));

  REQUIRE_FALSE(sumAtom3.canBeEvaluated());

  REQUIRE(sumAtom4.canBeEvaluated());
  REQUIRE_THAT(sumAtom4.evaluate(), Catch::Matchers::WithinAbs(res2, EPSILON));

  REQUIRE_FALSE(sumAtom5.canBeEvaluated());
}

TEST_CASE("Sum substituting", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("x"));
  const Sum sumAtom(std::move(args));
  const std::unique_ptr<Atom> sumAtomSubst = sumAtom.substitute("x", std::make_unique<Var>("o"));

  std::stringstream streamOutput;

  sumAtomSubst->print(streamOutput);
  REQUIRE(streamOutput.str() == "2*o+y");
}

TEST_CASE("Getting variable names from sum", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("x"));
  const Sum sumAtom(std::move(args));
  std::set<std::string> variableNames;
  sumAtom.getVariablesNames(variableNames);

  REQUIRE(variableNames.size() == 2);
  REQUIRE(variableNames.find("x") != variableNames.end());
  REQUIRE(variableNames.find("y") != variableNames.end());
}

TEST_CASE("Sum differentiating", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("x"));
  const Sum sumAtom(std::move(args));
  const std::unique_ptr<Atom> sumAtomDiff = sumAtom.differentiate("x");

  std::stringstream streamOutput;

  sumAtomDiff->print(streamOutput);
  REQUIRE(streamOutput.str() == "1+0+1");
}

TEST_CASE("Sum simplifying", "[symbolicAlgebra]") {
  constexpr int two = 2;
  constexpr int three = 3;
  constexpr int five = 5;
  constexpr int seven = 7;
  const Sum sumAtom1 = Sum();
  const Sum sumAtom2 = Sum(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> args3;
  args3.emplace_back(std::make_unique<Var>("x"));
  args3.emplace_back(std::make_unique<Var>("y"));
  args3.emplace_back(std::make_unique<Var>("z"));
  const Sum sumAtom3 = Sum(std::move(args3));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Var>("a"));
  args4.emplace_back(std::make_unique<Sum>(std::make_unique<Var>("b"), std::make_unique<Var>("c")));
  args4.emplace_back(std::make_unique<Var>("d"));
  const Sum sumAtom4 = Sum(std::move(args4));

  std::vector<std::unique_ptr<Atom>> args5;
  args5.emplace_back(std::make_unique<Var>("x"));
  args5.emplace_back(std::make_unique<Var>("x"));
  args5.emplace_back(std::make_unique<Var>("x"));
  const Sum sumAtom5 = Sum(std::move(args5));

  std::vector<std::unique_ptr<Atom>> args6;
  args6.emplace_back(std::make_unique<Product>(std::make_unique<Var>("x"), std::make_unique<Var>("y")));
  args6.emplace_back(std::make_unique<Product>(std::make_unique<Var>("y"), std::make_unique<Var>("x")));
  args6.emplace_back(std::make_unique<Product>(std::make_unique<Var>("x"), std::make_unique<Var>("y")));
  const Sum sumAtom6 = Sum(std::move(args6));

  const Sum sumAtom7 =
      Sum(std::make_unique<Product>(std::make_unique<Number>(two),
                                    std::make_unique<Power>(std::make_unique<Number>(five), std::make_unique<Number>(-1))),
          std::make_unique<Product>(std::make_unique<Number>(three),
                                    std::make_unique<Power>(std::make_unique<Number>(seven), std::make_unique<Number>(-1))));

  const std::unique_ptr<Atom> sumAtom1Simplified = sumAtom1.simplify();
  const std::unique_ptr<Atom> sumAtom2Simplified = sumAtom2.simplify();
  const std::unique_ptr<Atom> sumAtom3Simplified = sumAtom3.simplify();
  const std::unique_ptr<Atom> sumAtom4Simplified = sumAtom4.simplify();
  const std::unique_ptr<Atom> sumAtom5Simplified = sumAtom5.simplify();
  const std::unique_ptr<Atom> sumAtom6Simplified = sumAtom6.simplify();
  const std::unique_ptr<Atom> sumAtom7Simplified = sumAtom7.simplify();

  std::stringstream streamOutput;

  //REQUIRE(symbolicAlgebra::implementation::Atom::isNumber(sumAtom1Simplified));
  REQUIRE(sumAtom1Simplified->getType() == symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(sumAtom1Simplified->evaluate(), Catch::Matchers::WithinAbs(0.0, EPSILON));

  REQUIRE(sumAtom2Simplified->getType() == symbolicAlgebra::implementation::Atom::AtomType::VARIABLE);
  sumAtom2Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");

  REQUIRE(sumAtom3Simplified->getType() == symbolicAlgebra::implementation::Atom::AtomType::SUM);
  sumAtom3Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x+y+z");
  streamOutput.str("");

  REQUIRE(sumAtom4Simplified->getType() == symbolicAlgebra::implementation::Atom::AtomType::SUM);
  REQUIRE(sumAtom4Simplified->getArgs().size() == 4);
  sumAtom4Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a+b+c+d");
  streamOutput.str("");

  sumAtom5Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "3*x");
  streamOutput.str("");

  sumAtom6Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "3*x*y");
  streamOutput.str("");

  sumAtom7Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "29/35");
  streamOutput.str("");
}

TEST_CASE("Sum expanding", "[symbolicAlgebra]") {
  const Sum sumAtom(std::make_unique<Var>("x"), std::make_unique<Var>("y"));

  std::stringstream streamOutput;

  sumAtom.print(streamOutput);

  REQUIRE(streamOutput.str() == "x+y");
}

TEST_CASE("Sum comparing", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> sumAtom1 = std::make_unique<Sum>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> sumAtom2 = std::make_unique<Sum>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> sumAtom3 = std::make_unique<Sum>(std::make_unique<Var>("y"), std::make_unique<Var>("y"));

  REQUIRE(sumAtom1->compare(sumAtom2));
  REQUIRE_FALSE(sumAtom1->compare(sumAtom3));
}

TEST_CASE("Sum coefficient getting", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> sumAtom1 = std::make_unique<Sum>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> sumAtom2 = std::make_unique<Sum>(std::make_unique<Var>("x"), std::make_unique<Var>("y"));

  std::vector<std::unique_ptr<Atom>> args3;
  args3.emplace_back(std::make_unique<Var>("x"));
  args3.emplace_back(std::make_unique<Var>("y"));
  args3.emplace_back(std::make_unique<Var>("x"));
  const std::unique_ptr<Atom> sumAtom3 = std::make_unique<Sum>(std::move(args3));
  const std::unique_ptr<Atom> var = std::make_unique<Var>("x");

  const std::unique_ptr<Atom> coeff1 = sumAtom1->coefficient(sumAtom2);
  const std::unique_ptr<Atom> coeff2 = sumAtom3->coefficient(var);

  std::stringstream streamOutput;

  coeff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "1");
  streamOutput.str("");

  coeff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "2");
}
