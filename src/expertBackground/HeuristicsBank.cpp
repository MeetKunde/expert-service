#include "HeuristicsBank.h"
#include "ShapesBank.h"
#include "DependenciesBank.h"

namespace expertBackground {
HeuristicsBank::HeuristicsBank() : shapesBank{nullptr}, dependenciesBank{nullptr} { }

HeuristicsBank::HeuristicsBank(ShapesBank* shapesBankPointer, DependenciesBank* dependenciesBankPointer):
      shapesBank(shapesBankPointer), dependenciesBank(dependenciesBankPointer) {

  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    newDependencyHasBeenAdded.insert({(*iter), false});
  }

  shapesBank->addPointerToHeuristicsBank(this);
  dependenciesBankPointer->addPointerToHeuristicsBank(this);

  clearAllFlags();
}

void HeuristicsBank::initializeSchemeGraph() {
  for(const auto& line: shapesBank->getLinesVector()) {
    const auto& includedPoints = line.getIncludedPoints();
    for (size_t firstPointOnLineId = 0; firstPointOnLineId < includedPoints.size(); firstPointOnLineId++) {
      for (size_t secondPointOnLineId = firstPointOnLineId + 1; secondPointOnLineId < includedPoints.size(); secondPointOnLineId++) {
        schemeGraph.addEdge(
            shapesBank->getPointPositionInVector(includedPoints[firstPointOnLineId]),
            shapesBank->getPointPositionInVector(includedPoints[secondPointOnLineId]));
      }
    }
  }
}

void HeuristicsBank::initializeVariablesGraph() {
  const auto& variables = dependenciesBank->getVariables();
  for (auto variable = variables.begin(); variable != variables.end(); ++variable) {
    variablesGraph.addVertex(variable->first);
  }

  const auto& expressions = dependenciesBank->getExpressionModels();
  for (auto expressionModel = expressions.begin(); expressionModel != expressions.end(); ++expressionModel) {
    std::set<std::string> variablesSet{};
    (*expressionModel)->getIncludedVariables(variablesSet);

    const std::vector<std::string> variablesVector(variablesSet.begin(), variablesSet.end());
    variablesGraph.addFormula(variablesVector);
  }
}

void HeuristicsBank::clearNewShapesFlags() {
  newPointHasBeenAdded = false;
  newLineHasBeenAdded = false;
  newCircleHasBeenAdded = false;
}

void HeuristicsBank::clearNewDependenciesFlags() {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    newDependencyHasBeenAdded.at(*iter) = false;
  }
}

void HeuristicsBank::clearNewEvaluationsFlag() {
  newEvaluationHasBeenAdded = true;
}

void HeuristicsBank::clearAllFlags() {
  clearNewShapesFlags();
  clearNewDependenciesFlags();
  clearNewEvaluationsFlag();
}
} // namespace expertBackground