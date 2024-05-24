#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Product printing", "[symbolicAlgebra]") {
  constexpr int val1 = -3;
  constexpr int val2 = -7;
  constexpr int val3 = -11;
  constexpr int val4 = 13;
  constexpr int val5 = 19;
  constexpr int val6 = -1;

  const Product productAtom1 = Product();
  const Product productAtom2(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> vec;
  auto derived = std::make_unique<Var>("x");
  vec.push_back(std::move(derived));

  std::vector<std::unique_ptr<Atom>> productAtom3Args;
  productAtom3Args.emplace_back(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> args3;
  args3.emplace_back(std::make_unique<Var>("x"));
  args3.emplace_back(std::make_unique<Var>("y"));
  args3.emplace_back(std::make_unique<Var>("z"));
  const Product productAtom3(std::move(args3));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Number>(val1));
  args4.emplace_back(std::make_unique<Number>(val2));
  args4.emplace_back(std::make_unique<Number>(val3));
  const Product productAtom4(std::move(args4));

  std::vector<std::unique_ptr<Atom>> args5;
  args5.emplace_back(std::make_unique<Number>(val4));
  args5.emplace_back(std::make_unique<Number>(val2));
  args5.emplace_back(std::make_unique<Number>(val5));
  const Product productAtom5(std::move(args5));

  std::vector<std::unique_ptr<Atom>> args6;
  args6.emplace_back(std::make_unique<Number>(val6));
  args6.emplace_back(std::make_unique<Number>(val4));
  args6.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom6(std::move(args6));

  std::vector<std::unique_ptr<Atom>> args7;
  args7.emplace_back(std::make_unique<Number>(val6));
  args7.emplace_back(std::make_unique<Number>(val2));
  args7.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom7(std::move(args7));

  std::stringstream streamOutput;

  productAtom1.print(streamOutput);
  REQUIRE(streamOutput.str() == "1");
  streamOutput.str("");

  productAtom2.print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");

  productAtom3.print(streamOutput);
  REQUIRE(streamOutput.str() == "x*y*z");
  streamOutput.str("");

  productAtom4.print(streamOutput);
  REQUIRE(streamOutput.str() == "-3*7*11");
  streamOutput.str("");

  productAtom5.print(streamOutput);
  REQUIRE(streamOutput.str() == "13*-7*19");
  streamOutput.str("");

  productAtom6.print(streamOutput);
  REQUIRE(streamOutput.str() == "-13*x");
  streamOutput.str("");

  productAtom7.print(streamOutput);
  REQUIRE(streamOutput.str() == "1*7*x");
  streamOutput.str("");
}

TEST_CASE("Product copying", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("z"));
  const Product productAtom(std::move(args));
  const std::unique_ptr<Atom> productAtomCopy = productAtom.copy();

  for (auto iter1 = productAtom.getArgs().cbegin(),
            iter2 = productAtomCopy->getArgs().cbegin();
       iter1 != productAtom.getArgs().cend() &&
       iter2 != productAtomCopy->getArgs().cend();
       ++iter1, ++iter2) {
    REQUIRE(&(*iter1) != &(*iter2));
  }
}

TEST_CASE("Product evaluating", "[symbolicAlgebra]") {
  constexpr float val1 = 2.5;
  constexpr int val2 = 2;
  constexpr float val3 = 1.2;
  constexpr float res1 = 2.5;
  constexpr float res2 = 6.0;

  const Product productAtom1 = Product();
  const Product productAtom2 = Product(std::make_unique<Number>(val1));
  const Product productAtom3 = Product(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Number>(val1));
  args4.emplace_back(std::make_unique<Number>(val2));
  args4.emplace_back(std::make_unique<Number>(val3));
  const Product productAtom4 = Product(std::move(args4));

  std::vector<std::unique_ptr<Atom>> args5;
  args5.emplace_back(std::make_unique<Number>(val1));
  args5.emplace_back(std::make_unique<Number>(val2));
  args5.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom5 = Product(std::move(args5));

  REQUIRE(productAtom1.canBeEvaluated());
  REQUIRE_THAT(productAtom1.evaluate(),
               Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(productAtom2.canBeEvaluated());
  REQUIRE_THAT(productAtom2.evaluate(),
               Catch::Matchers::WithinAbs(res1, EPSILON));

  REQUIRE_FALSE(productAtom3.canBeEvaluated());

  REQUIRE(productAtom4.canBeEvaluated());
  REQUIRE_THAT(productAtom4.evaluate(),
               Catch::Matchers::WithinAbs(res2, EPSILON));

  REQUIRE_FALSE(productAtom5.canBeEvaluated());
}

TEST_CASE("Product substituting", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args1;
  args1.emplace_back(std::make_unique<Var>("x"));
  args1.emplace_back(std::make_unique<Var>("y"));
  args1.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom1(std::move(args1));

  const Product productAtom2(std::make_unique<Number>(-1),std::make_unique<Var>("x"));
  const Product productAtom3(std::make_unique<Number>(-1),std::make_unique<Var>("x"));

  const std::unique_ptr<Atom> productAtomSubst1 = productAtom1.substitute("x", std::make_unique<Var>("o"));
  const std::unique_ptr<Atom> productAtomSubst2 = productAtom2.substitute("x", std::make_unique<Var>("o"));
  const std::unique_ptr<Atom> productAtomSubst3 = productAtom3.substitute("x", std::make_unique<Product>(std::make_unique<Number>(-1),std::make_unique<Var>("o")));

  std::stringstream streamOutput;

  productAtomSubst1->print(streamOutput);
  REQUIRE(streamOutput.str() == "o^2*y");
  streamOutput.str("");

  productAtomSubst2->print(streamOutput);
  REQUIRE(streamOutput.str() == "-o");
  streamOutput.str("");

  productAtomSubst3->print(streamOutput);
  REQUIRE(streamOutput.str() == "o");
  streamOutput.str("");
}

TEST_CASE("Getting variable names from product", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom(std::move(args));

  std::set<std::string> variableNames;
  productAtom.getVariablesNames(variableNames);

  REQUIRE(variableNames.size() == 2);
  REQUIRE(variableNames.find("x") != variableNames.end());
  REQUIRE(variableNames.find("y") != variableNames.end());
}

TEST_CASE("Product differentiating", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args;
  args.emplace_back(std::make_unique<Var>("x"));
  args.emplace_back(std::make_unique<Var>("y"));
  args.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom(std::move(args));
  const std::unique_ptr<Atom> productAtomDiff = productAtom.differentiate("x");

  std::stringstream streamOutput;

  productAtomDiff->print(streamOutput);
  REQUIRE(streamOutput.str() == "1*y*x+x*0*x+x*y*1");
}

TEST_CASE("Product simplifying", "[symbolicAlgebra]") {
  constexpr int two = 2;
  constexpr int three = 3;
  constexpr int five = 5;
  constexpr int seven = 7.0;
  const Product productAtom1 = Product();
  const Product productAtom2 = Product(std::make_unique<Var>("x"));

  std::vector<std::unique_ptr<Atom>> args3;
  args3.emplace_back(std::make_unique<Var>("x"));
  args3.emplace_back(std::make_unique<Var>("y"));
  args3.emplace_back(std::make_unique<Var>("z"));
  const Product productAtom3 = Product(std::move(args3));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Var>("a"));
  args4.emplace_back(std::make_unique<Product>(std::make_unique<Var>("b"),
                                               std::make_unique<Var>("c")));
  args4.emplace_back(std::make_unique<Var>("d"));
  const Product productAtom4 = Product(std::move(args4));

  std::vector<std::unique_ptr<Atom>> args5;
  args5.emplace_back(std::make_unique<Var>("a"));
  args5.emplace_back(std::make_unique<Var>("b"));
  args5.emplace_back(std::make_unique<Var>("c"));
  args5.emplace_back(std::make_unique<Var>("b"));
  args5.emplace_back(std::make_unique<Var>("b"));
  const Product productAtom5 = Product(std::move(args5));

  const Product productAtom6 = Product(std::make_unique<Power>(
      std::make_unique<Var>("a"), std::make_unique<Var>("x")));
  const Product productAtom7 =
      Product(std::make_unique<Var>("a"),
              std::make_unique<Power>(std::make_unique<Var>("a"),
                                      std::make_unique<Var>("x")));
  const Product productAtom8 =
      Product(std::make_unique<Power>(std::make_unique<Var>("a"),
                                      std::make_unique<Var>("x")),
              std::make_unique<Power>(std::make_unique<Var>("a"),
                                      std::make_unique<Var>("y")));

  std::vector<std::unique_ptr<Atom>> args9;
  args9.emplace_back(std::make_unique<Number>(two));
  args9.emplace_back(std::make_unique<Var>("x"));
  args9.emplace_back(std::make_unique<Number>(seven));
  args9.emplace_back(std::make_unique<Const>("e"));
  const Product productAtom9 = Product(std::move(args9));

  std::vector<std::unique_ptr<Atom>> args10;
  args10.emplace_back(std::make_unique<Number>(two));
  args10.emplace_back(std::make_unique<Var>("x"));
  args10.emplace_back(std::make_unique<Number>(0.0));
  args10.emplace_back(std::make_unique<Number>(seven));
  args10.emplace_back(std::make_unique<Const>("e"));
  const Product productAtom10 = Product(std::move(args10));

  std::vector<std::unique_ptr<Atom>> args11;
  args11.emplace_back(std::make_unique<Number>(seven));
  args11.emplace_back(std::make_unique<Var>("x"));
  args11.emplace_back(std::make_unique<Power>(std::make_unique<Number>(seven),
                                              std::make_unique<Number>(-1)));
  args11.emplace_back(std::make_unique<Const>("e"));
  const Product productAtom11 = Product(std::move(args11));

  const Product productAtom12 =
      Product(std::make_unique<Number>(seven), std::make_unique<Number>(two));
  const Product productAtom13 =
      Product(std::make_unique<Product>(
                  std::make_unique<Number>(two),
                  std::make_unique<Power>(std::make_unique<Number>(five),
                                          std::make_unique<Number>(-1))),
              std::make_unique<Product>(
                  std::make_unique<Number>(three),
                  std::make_unique<Power>(std::make_unique<Number>(seven),
                                          std::make_unique<Number>(-1))));

  const Product productAtom14 =
      Product(std::make_unique<Number>(2), std::make_unique<Number>(-3));
  const Product productAtom15 =
      Product(std::make_unique<Number>(2), std::make_unique<Number>(-1));
  const Product productAtom16 =
      Product(std::make_unique<Number>(-1), std::make_unique<Number>(2));
  const Product productAtom17 =
      Product(std::make_unique<Number>(1),
              std::make_unique<Sum>(
                  std::make_unique<Var>("a"),
                  std::make_unique<Product>(std::make_unique<Number>(-1),
                                            std::make_unique<Var>("b"))));
  const Product productAtom18 =
      Product(std::make_unique<Number>(-1),
              std::make_unique<Sum>(
                  std::make_unique<Var>("a"),
                  std::make_unique<Product>(std::make_unique<Number>(-1),
                                            std::make_unique<Var>("b"))));

  std::vector<std::unique_ptr<Atom>> args19;
  args19.emplace_back(std::make_unique<Number>(-1));
  args19.emplace_back(std::make_unique<Number>(-1));
  args19.emplace_back(std::make_unique<Var>("x"));
  const Product productAtom19 = Product(std::move(args19));

  const std::unique_ptr<Atom> productAtom1Simplified = productAtom1.simplify();
  const std::unique_ptr<Atom> productAtom2Simplified = productAtom2.simplify();
  const std::unique_ptr<Atom> productAtom3Simplified = productAtom3.simplify();
  const std::unique_ptr<Atom> productAtom4Simplified = productAtom4.simplify();
  const std::unique_ptr<Atom> productAtom5Simplified = productAtom5.simplify();
  const std::unique_ptr<Atom> productAtom6Simplified = productAtom6.simplify();
  const std::unique_ptr<Atom> productAtom7Simplified = productAtom7.simplify();
  const std::unique_ptr<Atom> productAtom8Simplified = productAtom8.simplify();
  const std::unique_ptr<Atom> productAtom9Simplified = productAtom9.simplify();
  const std::unique_ptr<Atom> productAtom10Simplified =
      productAtom10.simplify();
  const std::unique_ptr<Atom> productAtom11Simplified =
      productAtom11.simplify();
  const std::unique_ptr<Atom> productAtom12Simplified =
      productAtom12.simplify();
  const std::unique_ptr<Atom> productAtom13Simplified =
      productAtom13.simplify();
  const std::unique_ptr<Atom> productAtom14Simplified =
      productAtom14.simplify();
  const std::unique_ptr<Atom> productAtom15Simplified =
      productAtom15.simplify();
  const std::unique_ptr<Atom> productAtom16Simplified =
      productAtom16.simplify();
  const std::unique_ptr<Atom> productAtom17Simplified =
      productAtom17.simplify();
  const std::unique_ptr<Atom> productAtom18Simplified =
      productAtom18.simplify();
  const std::unique_ptr<Atom> productAtom19Simplified =
      productAtom19.simplify();

  std::stringstream streamOutput;

  //REQUIRE(symbolicAlgebra::implementation::Atom::isNumber(productAtom1Simplified));
  REQUIRE(productAtom1Simplified->getType() ==
          symbolicAlgebra::implementation::Atom::AtomType::NUMBER_INT);
  REQUIRE_THAT(productAtom1Simplified->evaluate(),
               Catch::Matchers::WithinAbs(1.0, EPSILON));

  REQUIRE(productAtom2Simplified->getType() ==
          symbolicAlgebra::implementation::Atom::AtomType::VARIABLE);
  productAtom2Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");

  REQUIRE(productAtom3Simplified->getType() ==
          symbolicAlgebra::implementation::Atom::AtomType::PRODUCT);
  productAtom3Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x*y*z");
  streamOutput.str("");

  REQUIRE(productAtom4Simplified->getType() ==
          symbolicAlgebra::implementation::Atom::AtomType::PRODUCT);
  REQUIRE(productAtom4Simplified->getArgs().size() == 4);
  productAtom4Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a*b*c*d");
  streamOutput.str("");

  productAtom5Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a*b^3*c");
  streamOutput.str("");

  productAtom6Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a^x");
  streamOutput.str("");

  productAtom7Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a^(1+x)");
  streamOutput.str("");

  productAtom8Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a^(x+y)");
  streamOutput.str("");

  productAtom9Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "14*x*e");
  streamOutput.str("");

  productAtom10Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "0");
  streamOutput.str("");

  productAtom11Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x*e");
  streamOutput.str("");

  productAtom12Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "14");
  streamOutput.str("");

  productAtom13Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "6/35");
  streamOutput.str("");

  productAtom14Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "-6");
  streamOutput.str("");

  productAtom15Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "-2");
  streamOutput.str("");

  productAtom16Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "-2");
  streamOutput.str("");

  productAtom17Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "a-b");
  streamOutput.str("");

  productAtom18Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "-a+b");
  streamOutput.str("");

  productAtom19Simplified->print(streamOutput);
  REQUIRE(streamOutput.str() == "x");
  streamOutput.str("");
}

TEST_CASE("Product expanding", "[symbolicAlgebra]") {
  std::vector<std::unique_ptr<Atom>> args1;
  std::vector<std::unique_ptr<Atom>> args1Sum;
  args1Sum.emplace_back(std::make_unique<Var>("a"));
  args1Sum.emplace_back(std::make_unique<Var>("b"));
  args1Sum.emplace_back(std::make_unique<Var>("c"));
  args1.emplace_back(std::make_unique<Var>("x"));
  args1.emplace_back(std::make_unique<Sum>(std::move(args1Sum)));
  args1.emplace_back(std::make_unique<Var>("y"));
  const Product atomProduct1 = Product(std::move(args1));

  std::vector<std::unique_ptr<Atom>> args2;
  args2.emplace_back(std::make_unique<Var>("x"));
  args2.emplace_back(std::make_unique<Sum>(std::make_unique<Var>("a"),
                                           std::make_unique<Var>("b")));
  args2.emplace_back(std::make_unique<Sum>(std::make_unique<Var>("c"),
                                           std::make_unique<Var>("d")));
  args2.emplace_back(std::make_unique<Var>("y"));
  const Product atomProduct2 = Product(std::move(args2));

  const std::unique_ptr<Atom> atomProduct1Expanded = atomProduct1.expand();
  const std::unique_ptr<Atom> atomProduct2Expanded = atomProduct2.expand();

  std::stringstream streamOutput;

  atomProduct1Expanded->print(streamOutput);
  REQUIRE(streamOutput.str() == "a*x*y+b*x*y+c*x*y");
  streamOutput.str("");

  atomProduct2Expanded->print(streamOutput);
  REQUIRE(streamOutput.str() == "c*a*x*y+d*a*x*y+c*b*x*y+d*b*x*y");
  streamOutput.str("");
}

TEST_CASE("Product comparing", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> productAtom1 = std::make_unique<Product>(
      std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> productAtom2 = std::make_unique<Product>(
      std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> productAtom3 = std::make_unique<Product>(
      std::make_unique<Var>("y"), std::make_unique<Var>("y"));

  REQUIRE(productAtom1->compare(productAtom2));
  REQUIRE_FALSE(productAtom1->compare(productAtom3));
}

TEST_CASE("Product coefficient getting", "[symbolicAlgebra]") {
  const std::unique_ptr<Atom> productAtom1 = std::make_unique<Product>(
      std::make_unique<Var>("x"), std::make_unique<Var>("y"));
  const std::unique_ptr<Atom> productAtom2 = std::make_unique<Product>(
      std::make_unique<Var>("x"), std::make_unique<Var>("y"));

  std::vector<std::unique_ptr<Atom>> args3;
  args3.emplace_back(std::make_unique<Var>("x"));
  args3.emplace_back(std::make_unique<Var>("y"));
  args3.emplace_back(std::make_unique<Var>("z"));
  const std::unique_ptr<Atom> productAtom3 =
      std::make_unique<Product>(std::move(args3));

  std::vector<std::unique_ptr<Atom>> args4;
  args4.emplace_back(std::make_unique<Var>("x"));
  args4.emplace_back(std::make_unique<Var>("q"));
  args4.emplace_back(std::make_unique<Var>("z"));
  const std::unique_ptr<Atom> productAtom4 =
      std::make_unique<Product>(std::move(args4));

  const std::unique_ptr<Atom> productAtom5 = std::make_unique<Product>(
      std::make_unique<Number>(-1), std::make_unique<Var>("x"));
  const std::unique_ptr<Atom> productAtom6 = std::make_unique<Product>(
      std::make_unique<Var>("x"), std::make_unique<Number>(-1));
  const std::unique_ptr<Atom> productAtom7 = std::make_unique<Product>(
      std::make_unique<Number>(-1),
      std::make_unique<Sum>(
          std::make_unique<Product>(std::make_unique<Number>(2),
                                    std::make_unique<Var>("x")),
          std::make_unique<Var>("a")));

  const std::unique_ptr<Atom> varAtom1 = std::make_unique<Var>("x");
  const std::unique_ptr<Atom> varAtom2 = std::make_unique<Var>("z");

  const std::unique_ptr<Atom> coeff1 = productAtom1->coefficient(productAtom2);
  const std::unique_ptr<Atom> coeff2 = productAtom1->coefficient(varAtom1);
  const std::unique_ptr<Atom> coeff3 = productAtom1->coefficient(varAtom2);
  const std::unique_ptr<Atom> coeff4 = productAtom3->coefficient(productAtom1);
  const std::unique_ptr<Atom> coeff5 = productAtom4->coefficient(productAtom1);
  const std::unique_ptr<Atom> coeff6 = productAtom5->coefficient(varAtom1);
  const std::unique_ptr<Atom> coeff7 = productAtom6->coefficient(varAtom1);

  const std::unique_ptr<Atom> coeff8 = productAtom7->coefficient(varAtom1);

  std::stringstream streamOutput;

  coeff1->print(streamOutput);
  REQUIRE(streamOutput.str() == "1");
  streamOutput.str("");

  coeff2->print(streamOutput);
  REQUIRE(streamOutput.str() == "y");
  streamOutput.str("");

  coeff3->print(streamOutput);
  REQUIRE(streamOutput.str() == "0");
  streamOutput.str("");

  coeff4->print(streamOutput);
  REQUIRE(streamOutput.str() == "z");
  streamOutput.str("");

  coeff5->print(streamOutput);
  REQUIRE(streamOutput.str() == "0");
  streamOutput.str("");

  coeff6->print(streamOutput);
  REQUIRE(streamOutput.str() == "-1");
  streamOutput.str("");

  coeff7->print(streamOutput);
  REQUIRE(streamOutput.str() == "-1");
  streamOutput.str("");

  coeff8->print(streamOutput);
  REQUIRE(streamOutput.str() == "-2");
  streamOutput.str("");
}
