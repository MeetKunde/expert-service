#ifndef EXPERT_SERVICE_VARIABLES_GRAPH_H
#define EXPERT_SERVICE_VARIABLES_GRAPH_H

#include "string"
#include "utility"
#include "vector"
#include "map"
#include "stack"
#include "set"
#include "algorithm"
#include "ostream"
#include <iostream>

#include "MathHelper.h"

namespace expertBackground {

typedef std::vector<std::pair<size_t, MathHelper::Fraction>> Fractions;
typedef std::pair<std::string, std::string> EdgeEnds;

class VariablesGraph {
 private:
  std::map<std::string, std::vector<std::string>> adjacencyList;
  std::map<std::string, std::map<std::string, Fractions>> weights;
  std::map<std::string, bool> determinable;
  size_t formulaIdentifierCounter;

 public:
  VariablesGraph();

  void addEdge(const std::string& vertex1, const std::string& vertex2, MathHelper::Fraction weight, size_t identifier);
  void deleteEdges(const std::string& vertex1, const std::string& vertex2);
  std::vector<std::pair<const std::string&, bool>> getVertices();
  std::vector<std::pair<EdgeEnds, const Fractions&>> getEdges();

  void addFormula(const std::vector<std::string>& formulaVariables);
  size_t findDeterminableVariables();
  bool equationsAreDeterminable(const std::set<std::string>& variables) const;

  friend std::ostream& operator<<(std::ostream& stream, const VariablesGraph& variablesGraph);

 private:
  void initWeights(const std::string& vertex1, const std::string& vertex2);
  void addWeight(const std::string& vertex1, const std::string& vertex2, MathHelper::Fraction weight, size_t identifier);
  Fractions& getWeights(const std::string& vertex1, const std::string& vertex2);
  const Fractions& getConstWeights(const std::string& vertex1, const std::string& vertex2) const;
  void clearWeights(const std::string& vertex1, const std::string& vertex2);
  //void findCycles(const std::string& vertex, const std::string& parent, std::map<std::string, std::string>& parents,
  //                std::map<std::string, int>& colors, std::vector<std::vector<std::string>>& cycles);
};

} // namespace expertBackground

#endif  //EXPERT_SERVICE_VARIABLES_GRAPH_H
