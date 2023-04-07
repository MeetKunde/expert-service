#include "SymbolicAlgebraTestBase.h"

TEST_CASE("Solving equations without variables", "[symbolicAlgebra]") {
  const Equation equation1(Expression("a"), Expression("b"));
  const Equation equation2(Expression("a"), Expression("a"));

  std::stringstream streamOutput;

  const std::vector<Equation> result1 = Solve::equation(equation1, "x");
  const std::vector<Equation> result2 = Solve::equation(equation2, "x");

  REQUIRE(result1.empty());
  REQUIRE(result2.size() == 1);

  streamOutput << result2.front();
  REQUIRE(streamOutput.str() == "x = x");
}

TEST_CASE("Solving equations with variable in power 1", "[symbolicAlgebra]") {
  const Equation equation1(Expression("a*x"), Expression("-b"));
  const Equation equation2(Expression("3*x+4"), Expression("-5"));
  const Equation equation3(Expression("5*x"), Expression("7"));
  const Equation equation4(Expression("5*x"), Expression("5*x"));
  const Equation equation5(Expression("5*x"), Expression("8*x"));
  const Equation equation6(Expression("x"), Expression("3"));

  std::stringstream streamOutput;

  std::vector<Equation> result1 = Solve::equation(equation1, "x");
  std::vector<Equation> result2 = Solve::equation(equation2, "x");
  std::vector<Equation> result3 = Solve::equation(equation3, "x");
  std::vector<Equation> result4 = Solve::equation(equation4, "x");
  std::vector<Equation> result5 = Solve::equation(equation5, "x");
  std::vector<Equation> result6 = Solve::equation(equation6, "x");

  REQUIRE(result1.size() == 1);
  REQUIRE(result2.size() == 1);
  REQUIRE(result3.size() == 1);
  REQUIRE(result4.size() == 1);
  REQUIRE(result5.size() == 1);
  REQUIRE(result6.size() == 1);

  streamOutput << result1.front();
  REQUIRE(streamOutput.str() == "x = -b*a^-1");
  streamOutput.str("");

  streamOutput << result2.front();
  REQUIRE(streamOutput.str() == "x = -3");
  streamOutput.str("");

  streamOutput << result3.front();
  REQUIRE(streamOutput.str() == "x = 7/5");
  streamOutput.str("");

  streamOutput << result4.front();
  REQUIRE(streamOutput.str() == "x = x");
  streamOutput.str("");

  streamOutput << result5.front();
  REQUIRE(streamOutput.str() == "x = 0");
  streamOutput.str("");

  streamOutput << result6.front();
  REQUIRE(streamOutput.str() == "x = 3");
  streamOutput.str("");
}

TEST_CASE("Solving equations with variable in power 2", "[symbolicAlgebra]") {
  const Equation equation1(Expression("a*x^2 + b*x + c"), Expression("0"));

  std::stringstream streamOutput;

  std::vector<Equation> result1 = Solve::equation(equation1, "x");

  REQUIRE(result1.size() == 2);

  streamOutput << result1.front();
  REQUIRE(streamOutput.str() == "x = (-b+(b^2-4*a*c)^1/2)*(2*a)^-1");
  streamOutput.str("");

  streamOutput << result1.back();
  REQUIRE(streamOutput.str() == "x = (-b-(b^2-4*a*c)^1/2)*(2*a)^-1");
}

TEST_CASE("Solving systems of linear equations", "[symbolicAlgebra]") {
  std::stringstream streamOutput;

  const std::vector<Equation> system1(
      {Equation("x", "3"), Equation("x + y", "5"), Equation("z", "a - b"), Equation("d + g", "f")});
  std::vector<Equation> solution1 = Solve::systemOfEquations(system1);

  REQUIRE(solution1.size() == 8);
  streamOutput << solution1[0];
  REQUIRE(streamOutput.str() == "x = 3");
  streamOutput.str("");
  streamOutput << solution1[1];
  REQUIRE(streamOutput.str() == "y = 2");
  streamOutput.str("");
  streamOutput << solution1[2];
  REQUIRE(streamOutput.str() == "d = -g+f");
  streamOutput.str("");
  streamOutput << solution1[3];
  REQUIRE(streamOutput.str() == "f = d+g");
  streamOutput.str("");
  streamOutput << solution1[4];
  REQUIRE(streamOutput.str() == "g = -d+f");
  streamOutput.str("");
  streamOutput << solution1[4 + 1];
  REQUIRE(streamOutput.str() == "a = z+b");
  streamOutput.str("");
  streamOutput << solution1[4 + 2];
  REQUIRE(streamOutput.str() == "b = a-z");
  streamOutput.str("");
  streamOutput << solution1[4 + 3];
  REQUIRE(streamOutput.str() == "z = a-b");
  streamOutput.str("");

  const std::vector<Equation> system2({Equation("x + 2*y", "8"), Equation("2*x - y", "1")});
  std::vector<Equation> solution2 = Solve::systemOfEquations(system2);

  REQUIRE(solution2.size() == 2);
  streamOutput << solution2[0];
  REQUIRE(streamOutput.str() == "y = 3");
  streamOutput.str("");
  streamOutput << solution2[1];
  REQUIRE(streamOutput.str() == "x = 2");
  streamOutput.str("");

  const std::vector<Equation> system3({
      Equation("2*x + 3*y - z", "11"),
      Equation("x - y + z", "0"),
      Equation("-x + 2*y - 3*z", "12"),
  });

  std::vector<Equation> solution3 = Solve::systemOfEquations(system3);

  REQUIRE(solution3.size() == 3);
  streamOutput << solution3[0];
  REQUIRE(streamOutput.str() == "z = -7");
  streamOutput.str("");
  streamOutput << solution3[1];
  REQUIRE(streamOutput.str() == "y = -2");
  streamOutput.str("");
  streamOutput << solution3[2];
  REQUIRE(streamOutput.str() == "x = 5");
  streamOutput.str("");

  const std::vector<symbolicAlgebra::Equation> system4(
      {symbolicAlgebra::Equation("2*x + y - z + 4*w - t", "7"), symbolicAlgebra::Equation("x - y + z - w - t", "47/4"),
       symbolicAlgebra::Equation("-3*x + 2*y - z - w + t", "-95/4"), symbolicAlgebra::Equation("x + y + z", "-3/2"),
       symbolicAlgebra::Equation("w - t", "17/4")});

  std::vector<Equation> solution4 = Solve::systemOfEquations(system4);

  REQUIRE(solution4.size() == 5);
  streamOutput << solution4[0];
  REQUIRE(streamOutput.str() == "z = -1/28");
  streamOutput.str("");
  streamOutput << solution4[1];
  REQUIRE(streamOutput.str() == "y = -67/14");
  streamOutput.str("");
  streamOutput << solution4[2];
  REQUIRE(streamOutput.str() == "x = 93/28");
  streamOutput.str("");
  streamOutput << solution4[3];
  REQUIRE(streamOutput.str() == "w = 2/7");
  streamOutput.str("");
  streamOutput << solution4[4];
  REQUIRE(streamOutput.str() == "t = -111/28");
  streamOutput.str("");

  const std::vector<symbolicAlgebra::Equation> system5(
      {symbolicAlgebra::Equation("x - z", "1"), symbolicAlgebra::Equation("x + y + z", "3")});

  std::vector<Equation> solution5 = Solve::systemOfEquations(system5);

  REQUIRE(solution5.size() == 4);
  streamOutput << solution5[0];
  REQUIRE(streamOutput.str() == "y = 2-2*z");
  streamOutput.str("");
  streamOutput << solution5[1];
  REQUIRE(streamOutput.str() == "z = -1/2*(-2+y)");
  streamOutput.str("");
  streamOutput << solution5[2];
  REQUIRE(streamOutput.str() == "x = 1+z");
  streamOutput.str("");
  streamOutput << solution5[3];
  REQUIRE(streamOutput.str() == "z = -1+x");
  streamOutput.str("");
}