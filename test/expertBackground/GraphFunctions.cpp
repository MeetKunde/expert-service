#include "ExpertBackgroundTestBase.h"

TEST_CASE("Scheme Graph", "[expertBackground]") {
  SchemeGraph graph{};
  const std::vector<std::pair<size_t, size_t>> edges = {
      {0, 1},
      {0, 15},
      {16, 1024},
      {9, 19},
      {19, 29}
  };

  REQUIRE(graph.getAllEdges().empty());

  for (size_t edgeNum = 0; edgeNum < edges.size(); edgeNum++) {
    graph.addEdge(edges[edgeNum].first, edges[edgeNum].second);
    REQUIRE(graph.getAllEdges().size() == (edgeNum + 1));
  }

  for (size_t edgeNum = 0; edgeNum < edges.size(); edgeNum++) {
    REQUIRE(std::find(graph.getAllEdges().begin(), graph.getAllEdges().end(), edges[edgeNum]) != graph.getAllEdges().end());
  }

  for (size_t edgeNum = 0; edgeNum < edges.size(); edgeNum++) {
    graph.deleteEdge(edges[edgeNum].first, edges[edgeNum].second);
    REQUIRE(graph.getAllEdges().size() == (edges.size() - edgeNum - 1));
  }
}

TEST_CASE("Variables Graph - Basic Functions", "[expertBackground]") {
  VariablesGraph graph{};
  const std::vector<std::pair<std::pair<std::string, std::string>, Fraction>> inEdges = {
      { {"a", "b"}, {1, 2} },
      { {"a", "c"}, {2, 1} },
      { {"d", "e"}, {7, 5} },
      { {"d", "e"}, {3, 15} },
      { {"g", "g"}, {2, 3} },
      { {"h", "h"}, {1, 3} },
      { {"h", "h"}, {2, 3} }
  };

  const std::vector<std::pair<std::string, bool>> expVertices = {
      {"a", false},
      {"b", false},
      {"c", false},
      {"d", false},
      {"e", false},
      {"g", false},
      {"h", false}
  };

  const std::vector<size_t> expVerticesNums = {2, 3, 5, 5, 6, 7, 7};

  const std::vector<std::pair<std::pair<std::string, std::string>, std::vector<Fraction>>> expEdges = {
      { {"a", "b"}, {{1, 2}} },
      { {"a", "c"}, {{2, 1}} },
      { {"d", "e"}, {{7, 5}, {3, 15}} },
      { {"g", "g"}, {{2, 3}} },
      { {"h", "h"}, {{1, 3}, {2, 3}} }
  };

  const std::vector<std::size_t> expEdgesNums = { 1, 2, 3, 3, 4, 5, 5 };

  REQUIRE(graph.getVertices().empty());
  REQUIRE(graph.getEdges().empty());

  for (size_t num = 0; num < inEdges.size(); num++) {
    graph.addEdge(inEdges[num].first.first, inEdges[num].first.second, inEdges[num].second, num);
    REQUIRE(graph.getVertices().size() == expVerticesNums[num]);
    REQUIRE(graph.getEdges().size() == expEdgesNums[num]);
  }

  const auto& vertices = graph.getVertices();
  for (size_t num = 0; num < expVertices.size(); num++) {
    REQUIRE(vertices[num].first == expVertices[num].first);
    REQUIRE(vertices[num].second == expVertices[num].second);
  }

  const auto& edges = graph.getEdges();
  for (size_t num = 0; num < expEdges.size(); num++) {
    REQUIRE(edges[num].first == expEdges[num].first);

    for (size_t fracNum = 0; fracNum < edges[num].second.size(); fracNum++) {
      const Fraction& frac1 = edges[num].second[fracNum].second;
      const Fraction& frac2 = expEdges[num].second[fracNum];
      REQUIRE(frac1.nominator == frac2.nominator);
      REQUIRE(frac1.denominator == frac2.denominator);
    }
  }

  for (size_t edgeNum = 0; edgeNum < expEdges.size(); edgeNum++) {
    graph.deleteEdges(expEdges[edgeNum].first.first, expEdges[edgeNum].first.second);
    REQUIRE(graph.getEdges().size() == (expEdges.size() - edgeNum - 1));
  }
  REQUIRE(graph.getVertices().size() == expVertices.size());
}


TEST_CASE("Variables Graph - Computation", "[expertBackground]") {
  typedef std::vector<std::vector<std::string>> FormulaDesc;
  typedef std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::pair<size_t, Fraction>>>> EdgesDesc;
  typedef std::vector<std::pair<std::string, bool>> VerticesDesc;

  const std::vector<std::pair<FormulaDesc, std::pair<VerticesDesc, EdgesDesc>>> testCases {
      /*
      {
          // system of equations:
          // a + b = 6
          // a = 2 * b
          {
           {"a", "b"},
           {"a", "b"}
          },
          {
           {
            {"a", true},
            {"b", true}
           },
           {
            { {"a", "b"}, {{0, Fraction{2, 2}}, {1, Fraction{2, 2}}} }
           }
          }
      },

      {
          // system of equations
          // a = b
          // b = c
          // c = d
          // d = 5
          {
           { {"a", "b"}, {Fraction{1, 2}} },
           { {"b", "c"}, {Fraction{1, 2}} },
           { {"c", "d"}, {Fraction{1, 2}} },
           { {"d", "d"}, {Fraction{1, 1}} }
          },
          {
           {
            {"a", true},
            {"b", true},
            {"c", true},
            {"d", true}
           },
           {
            { {"a", "b"}, {Fraction{2, 2}} },
            { {"b", "c"}, {Fraction{2, 2}} },
            { {"c", "d"}, {Fraction{2, 2}} },
            { {"d", "d"}, {Fraction{1, 1}} }
           }
          }
      },

      {
          // system of equations
          // a = b
          // b = c
          // c = d
          // d = a
          {
           { {"a", "b"}, {Fraction{1, 2}} },
           { {"b", "c"}, {Fraction{1, 2}} },
           { {"c", "d"}, {Fraction{1, 2}} },
           { {"d", "a"}, {Fraction{1, 2}} }
          },
          {
           {
            {"a", true},
            {"b", true},
            {"c", true},
            {"d", true}
           },
           {
            { {"a", "b"}, {Fraction{2, 2}} },
            { {"a", "d"}, {Fraction{2, 2}} },
            { {"b", "c"}, {Fraction{2, 2}} },
            { {"c", "d"}, {Fraction{2, 2}} }
           }
          }
      },

      {
          // system of equations
          // a = b
          // b = c
          // c = d
          {
           { {"a", "b"}, {Fraction{1, 2}} },
           { {"b", "c"}, {Fraction{1, 2}} },
           { {"c", "d"}, {Fraction{1, 2}} }
           },
           {
               {
                   {"a", false},
                   {"b", false},
                   {"c", false},
                   {"d", false}
               },
               {
                   { {"a", "b"}, {Fraction{1, 2}} },
                   { {"b", "c"}, {Fraction{1, 2}} },
                   { {"c", "d"}, {Fraction{1, 2}} }
               }
           }
      }

      {
          // system of equations:
          // 3a_1 + 2a_2 + a_3 = 7
          // 4a_4 = 2a_1
          // a_6 = a_5
          // a_7 - a_6 = 0
          // a_6 + a_7 = 8
          {
           { {"a_1", "a_2"}, {Fraction{1, 3}} },
           { {"a_1", "a_3"}, {Fraction{1, 3}} },
           { {"a_2", "a_3"}, {Fraction{1, 3}} },
           { {"a_4", "a_1"}, {Fraction{1, 2}} },
           { {"a_6", "a_5"}, {Fraction{1, 2}} },
           { {"a_7", "a_6"}, {Fraction{1, 2}} },
           { {"a_7", "a_6"}, {Fraction{1, 2}} }
          },
          {
           {
            {"a_1", false},
            {"a_2", false},
            {"a_3", false},
            {"a_4", false},
            {"a_5", true},
            {"a_6", true},
            {"a_7", true}
           },
           {
               { {"a_1", "a_2"}, {Fraction{1, 3}} },
               { {"a_1", "a_3"}, {Fraction{1, 3}} },
               { {"a_2", "a_3"}, {Fraction{1, 3}} },
               { {"a_4", "a_1"}, {Fraction{1, 2}} },
               { {"a_6", "a_5"}, {Fraction{1, 2}} },
               { {"a_7", "a_6"}, {Fraction{1, 2}} },
               { {"a_7", "a_6"}, {Fraction{1, 2}} }
           }
          }
      }
      */
  };

  for (auto testCase = testCases.begin(); testCase != testCases.end(); ++testCase) {
    VariablesGraph graph;
    const auto& input = testCase->first;
    const auto& output = testCase->second;

    // input
    for (auto formula = input.begin(); formula != input.end(); ++formula) {
      graph.addFormula(*formula);
    }

    graph.findDeterminableVariables();

    std::cout << graph << std::endl;

    // output
    const auto& expectedVertices = output.first;
    const auto& expectedEdges = output.second;
    const auto& resultVertices = graph.getVertices();
    const auto& resultEdges = graph.getEdges();

    REQUIRE(resultVertices.size() == expectedVertices.size());
    REQUIRE(resultEdges.size() == expectedEdges.size());

    for (size_t vertexNum = 0; vertexNum < expectedVertices.size(); vertexNum++) {
      REQUIRE(resultVertices[vertexNum].first == expectedVertices[vertexNum].first);
      REQUIRE(resultVertices[vertexNum].second == expectedVertices[vertexNum].second);
    }

    for (size_t edgeNum = 0; edgeNum < expectedEdges.size(); edgeNum++) {
      REQUIRE(resultEdges[edgeNum].first.first == expectedEdges[edgeNum].first.first);
      REQUIRE(resultEdges[edgeNum].first.second == expectedEdges[edgeNum].first.second);

      REQUIRE(resultEdges[edgeNum].second.size() == expectedEdges[edgeNum].second.size());
      for (size_t fracNum = 0; fracNum < expectedEdges[edgeNum].second.size(); fracNum++) {
        REQUIRE(resultEdges[edgeNum].second[fracNum].first == expectedEdges[edgeNum].second[fracNum].first);
        const Fraction& frac1 = resultEdges[edgeNum].second[fracNum].second;
        const Fraction& frac2 = expectedEdges[edgeNum].second[fracNum].second;
        REQUIRE(frac1.nominator == frac2.nominator);
        REQUIRE(frac1.denominator == frac2.denominator);
      }
    }
  }
}
