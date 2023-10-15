#include "VariablesGraph.h"

namespace expertBackground {
  VariablesGraph::VariablesGraph() {}

  void VariablesGraph::addEdge(const std::string& vertex1, const std::string& vertex2, MathHelper::Fraction weight, size_t identifier) {
    if (adjacencyList.find(vertex1) == adjacencyList.end()) {
      adjacencyList.insert({vertex1, {}});
      determinable.insert({vertex1, false});
    }
    if (adjacencyList.find(vertex2) == adjacencyList.end()) {
      adjacencyList.insert({vertex2, {}});
      determinable.insert({vertex2, false});
    }

    initWeights(vertex1, vertex2);

    const auto& neighbour1 = std::find(adjacencyList.at(vertex1).begin(), adjacencyList.at(vertex1).end(), vertex2);
    if (neighbour1 == adjacencyList.at(vertex1).end()) {
      adjacencyList.at(vertex1).emplace_back(vertex2);
    }

    if (vertex1 != vertex2) {  // do not add loops twice
      const auto& neighbour2 = std::find(adjacencyList.at(vertex2).begin(), adjacencyList.at(vertex2).end(), vertex1);
      if (neighbour2 == adjacencyList.at(vertex2).end()) {
        adjacencyList.at(vertex2).emplace_back(vertex1);
      }
    }

    addWeight(vertex1, vertex2, weight, identifier);
  }

  void VariablesGraph::deleteEdges(const std::string& vertex1, const std::string& vertex2) {
    const auto& neighbour1 = std::find(adjacencyList.at(vertex1).begin(), adjacencyList.at(vertex1).end(), vertex2);
    if (neighbour1 != adjacencyList.at(vertex1).end()) {
      adjacencyList.at(vertex1).erase(neighbour1);
    }

    const auto& neighbour2 = std::find(adjacencyList.at(vertex2).begin(), adjacencyList.at(vertex2).end(), vertex1);
    if (neighbour2 != adjacencyList.at(vertex2).end()) {
      adjacencyList.at(vertex2).erase(neighbour2);
    }

    clearWeights(vertex1, vertex2);
  }

  std::vector<std::pair<const std::string&, bool>> VariablesGraph::getVertices() {
      std::vector<std::pair<const std::string&, bool>> vertices{};

      for (auto vertex = adjacencyList.begin(); vertex != adjacencyList.end(); ++vertex) {
        vertices.emplace_back(vertex->first, determinable[vertex->first]);
      }

      return vertices;
  }

  std::vector<std::pair<EdgeEnds, const Fractions&>> VariablesGraph::getEdges() {
      std::vector<std::pair<EdgeEnds, const Fractions&>> edges{};

      for (auto vertex = adjacencyList.begin(); vertex != adjacencyList.end(); ++vertex) {
        for (auto neighbour = vertex->second.begin(); neighbour != vertex->second.end(); ++neighbour) {
          if(vertex->first <= *neighbour) {
            edges.emplace_back(EdgeEnds{vertex->first, *neighbour}, getWeights(vertex->first, *neighbour));
          }
        }
      }

      return edges;
  }

  void VariablesGraph::addFormula(const std::vector<std::string>& formulaVariables) {
      const size_t variablesNumber = formulaVariables.size();
      if (variablesNumber == 1) {
        addEdge(formulaVariables.at(0), formulaVariables.at(0),
                MathHelper::Fraction{1, 1}, formulaIdentifierCounter);
        formulaIdentifierCounter++;
      }
      else if (variablesNumber > 1) {
        for (size_t pos1 = 0; pos1 < variablesNumber; pos1++) {
          for (size_t pos2 = pos1 + 1; pos2 < variablesNumber; pos2++) {
            addEdge(formulaVariables.at(pos1), formulaVariables.at(pos2),
                    MathHelper::Fraction{1, static_cast<int>(variablesNumber)}, formulaIdentifierCounter);
          }
        }
        formulaIdentifierCounter++;
      }
  }

  size_t VariablesGraph::findDeterminableVariables() {
      const size_t founds = 0;

      return founds;
  }

  bool VariablesGraph::equationsAreDeterminable(const std::set<std::string>& variables) const {
      if (variables.size() < 2) {
        return true;
      }

      std::map<std::string, bool> visited;
      for (auto vertex = adjacencyList.begin(); vertex != adjacencyList.end(); ++vertex) {
        visited.insert({vertex->first, false});
      }

      std::stack<std::string> dfsStack{};
      dfsStack.push(*variables.begin());
      while (!dfsStack.empty()) {
        const std::string vertex = dfsStack.top();
        dfsStack.pop();

        if (!visited.at(vertex)) {
          visited.at(vertex) = true;
        }

        for (auto neighbour = adjacencyList.at(vertex).begin(); neighbour != adjacencyList.at(vertex).end(); ++neighbour) {
          if(!visited.at(*neighbour)) {
            dfsStack.push(*neighbour);
          }
        }
      }

      for (auto variable = variables.begin(); variable != variables.end(); ++variable) {
        if (!visited.at(*variable)) {
          return false;
        }
      }

      return true;
  }

  std::ostream& operator<<(std::ostream& stream, const VariablesGraph& variablesGraph) {
    for (auto it = variablesGraph.adjacencyList.begin(); it != variablesGraph.adjacencyList.end(); ++it) {
      for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
        stream << "({" << it->first << "(" << (variablesGraph.determinable.at(it->first) ? "Ava" : "UnAva")  << "), " << *jt << "}, {";
        bool first = true;
        const auto& weights = variablesGraph.getConstWeights(it->first, *jt);
        for (auto kt = weights.begin(); kt != weights.end(); ++kt) {
          if(first) {
            first = false;
          }
          else {
            stream << ", ";
          }
          stream << kt->second.nominator << "/" << kt->second.denominator << "(" << kt->first << ")";
        }
        stream << "})\n";
      }
    }

    return stream;
  }

  void VariablesGraph::initWeights(const std::string& vertex1, const std::string& vertex2) {
    if (vertex1 < vertex2) {
      if (weights.find(vertex1) == weights.end()) {
        weights.insert({vertex1, {}});
      }

      if(weights.at(vertex1).find(vertex2) == weights.at(vertex1).end()) {
        weights.at(vertex1).insert({vertex2, {}});
      }
    }
    else {
      if (weights.find(vertex2) == weights.end()) {
        weights.insert({vertex2, {}});
      }

      if(weights.at(vertex2).find(vertex1) == weights.at(vertex2).end()) {
        weights.at(vertex2).insert({vertex1, {}});
      }
    }
  }

  void VariablesGraph::addWeight(const std::string& vertex1, const std::string& vertex2, MathHelper::Fraction weight, size_t identifier) {
    if (vertex1 < vertex2) {
      weights[vertex1][vertex2].emplace_back(identifier, weight);
    }
    else {
      weights[vertex2][vertex1].emplace_back(identifier, weight);
    }
  }

  Fractions& VariablesGraph::getWeights(const std::string& vertex1, const std::string& vertex2) {
    if (vertex1 < vertex2) {
      return weights[vertex1][vertex2];
    }

    return weights[vertex2][vertex1];
  }

  const Fractions& VariablesGraph::getConstWeights(const std::string& vertex1, const std::string& vertex2) const {
    if (vertex1 < vertex2) {
      return weights.at(vertex1).at(vertex2);
    }

    return weights.at(vertex2).at(vertex1);
  }

  void VariablesGraph::clearWeights(const std::string& vertex1, const std::string& vertex2) {
    if(vertex1 < vertex2) {
      weights[vertex1][vertex2] = {};
    }
    else {
      weights[vertex2][vertex1] = {};
    }
  }

  /*
  void VariablesGraph::findCycles(const std::string& vertex, const std::string& parent, std::map<std::string, std::string>& parents,
                                  std::map<std::string, int>& colors, std::vector<std::vector<std::string>>& cycles) {
    std::cout << "find cycles: " << vertex << " " << parent << std::endl;
    if (colors.at(vertex) == 2) { // vertex is completely visited.
      return;
    }

    if (colors[vertex] == 1) {  // vertex was not completely visited -> cycle detected
      std::vector<std::string> cycle;

      std::string currentVertex = parent;
      cycle.push_back(currentVertex);

      while (currentVertex != vertex) {
        currentVertex = parents[currentVertex];
        cycle.push_back(currentVertex);
      }
      cycles.push_back(cycle);
      return;
    }
    parents[vertex] = parent;

    colors[vertex] = 1;  // first visit

    // dfs
    for (const std::string& neighbour : adjacencyList[vertex]) {
      if (neighbour == parents[vertex]) {   // vertex has not been visited previously
        continue;
      }
      findCycles(neighbour, vertex, parents, colors, cycles);
    }

    colors[vertex] = 2;  // vertex has been completely visited
  }
   */
} // namespace expertBackground