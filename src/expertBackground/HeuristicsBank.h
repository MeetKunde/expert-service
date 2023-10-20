#ifndef EXPERT_SERVICE_HEURISTICS_BANK_H
#define EXPERT_SERVICE_HEURISTICS_BANK_H

#include "dependency/IDependency.h"
#include "utilities/SchemeGraph.h"
#include "utilities/VariablesGraph.h"

namespace expertBackground {
class ShapesBank;
class DependenciesBank;

class HeuristicsBank {
 private:
    ShapesBank* shapesBank;
    DependenciesBank* dependenciesBank;

    bool newPointHasBeenAdded;
    bool newLineHasBeenAdded;
    bool newCircleHasBeenAdded;
    std::map<IDependency::Type, bool> newDependencyHasBeenAdded;
    bool newEvaluationHasBeenAdded;

    SchemeGraph schemeGraph;
    VariablesGraph variablesGraph;

   public:
    HeuristicsBank();
    HeuristicsBank(ShapesBank* shapesBankPointer, DependenciesBank* dependenciesBankPointer);

    void initializeSchemeGraph();
    void initializeVariablesGraph();

    inline void markNewPointsFlag() { newPointHasBeenAdded = true; }
    inline bool getNewPointsFlag() const { return newPointHasBeenAdded; }

    inline void markNewlinesFlag() { newLineHasBeenAdded = true; }
    inline bool getNewLinesFlag() const { return newLineHasBeenAdded; }

    inline void markNewCirclesFlag() { newCircleHasBeenAdded = true; }
    inline bool getNewCirclesFlag() const { return newCircleHasBeenAdded; }

    inline void markNewDependencyFlag(IDependency::Type type) { newDependencyHasBeenAdded.at(type) = true; }
    inline bool getNewDependencyFlag(IDependency::Type type) const { return newDependencyHasBeenAdded.at(type); }

    inline void markNewEvaluationsFlag() { newEvaluationHasBeenAdded = true; }
    inline bool getNewEvaluationFlag() const { return newEvaluationHasBeenAdded; }

    inline void addNewFormula(const std::vector<std::string>& variables) { variablesGraph.addFormula(variables); }

    inline bool schemeEdgeExist(size_t vertex1, size_t vertex2) const { return schemeGraph.checkIfEdgeExist(vertex1, vertex2); }
    inline std::vector<std::vector<size_t>>trianglesInScheme() const { return schemeGraph.getAllTriangles(); }
    inline bool equationsAreDeterminable(const std::set<std::string>& variables) const { return variablesGraph.equationsAreDeterminable(variables); }

    void clearNewShapesFlags();
    void clearNewDependenciesFlags();
    void clearNewEvaluationsFlag();
    void clearAllFlags();
};
} // namespace expertBackground

#endif  //EXPERT_SERVICE_HEURISTICS_BANK_H
