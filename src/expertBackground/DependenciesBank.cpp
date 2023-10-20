#include "DependenciesBank.h"

namespace expertBackground {
DependenciesBank::DependenciesBank()
    : dependencyIdCounter{0},
      shapesBank{nullptr},
      heuristicsBank{nullptr} {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    dependenciesMap.insert({(*iter), {}});
  }

  const std::shared_ptr<IDependency> rootDependency{std::make_shared<ExerciseDescriptionDependency>(dependencyIdCounter)};
  addNewDependency(rootDependency);
}

DependenciesBank::DependenciesBank(const ShapesBank* generatedShapesBank)
    : dependencyIdCounter(0),
      shapesBank(generatedShapesBank),
      heuristicsBank{nullptr} {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    dependenciesMap.insert({ (*iter), { } });
  }

  const std::shared_ptr<IDependency> rootDependency{std::make_shared<ExerciseDescriptionDependency>(dependencyIdCounter)};
  addNewDependency(rootDependency);
}

void DependenciesBank::initializeBaseVariables() {
  const std::vector<PointModel>& points = shapesBank->getPointsVector();

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos <= point1Pos; point2Pos++) {
      const std::pair<std::string, std::vector<std::string>> lengthName =
          getSegmentName(points.at(point1Pos), points.at(point2Pos));

      variableToIndexes.insert({ lengthName.first,
                                { shapesBank->getPointPositionInVector(lengthName.second[0]),shapesBank->getPointPositionInVector(lengthName.second[1]) } });
      formulasIncludingVariable.insert({ lengthName.first, { } });
    }
  }

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < shapesBank->getPointsNumber(); point2Pos++) {
      for (size_t point3Pos = 0; point3Pos <= point1Pos; point3Pos++) {
        const std::pair<std::string, std::vector<std::string>> convexMeasureName =
            getAngleName(points.at(point1Pos), points.at(point2Pos), points.at(point3Pos), true);

        variableToIndexes.insert({ convexMeasureName.first,
                                  { shapesBank->getPointPositionInVector(convexMeasureName.second[0]),
                                   shapesBank->getPointPositionInVector(convexMeasureName.second[1]),
                                   shapesBank->getPointPositionInVector(convexMeasureName.second[2]),
                                   static_cast<size_t>(true)} });
        formulasIncludingVariable.insert({ convexMeasureName.first, { } });

        const std::pair<std::string, std::vector<std::string>> concaveMeasureName =
            getAngleName(points.at(point1Pos), points.at(point2Pos), points.at(point3Pos), false);

        variableToIndexes.insert({ concaveMeasureName.first,
                                  { shapesBank->getPointPositionInVector(concaveMeasureName.second[0]),
                                   shapesBank->getPointPositionInVector(concaveMeasureName.second[1]),
                                   shapesBank->getPointPositionInVector(concaveMeasureName.second[2]),
                                   static_cast<size_t>(false)} });
        formulasIncludingVariable.insert({ concaveMeasureName.first, { } });
      }
    }
  }
}

unsigned int DependenciesBank::addEquation(const symbolicAlgebra::Expression& leftSide,
                                           const symbolicAlgebra::Expression& rightSide,
                                           IDependency::Reason reason,
                                           const std::vector<size_t>& dependentDependencies,
                                           IDependency::ImportanceLevel importanceLevel) {

  return  addEquationUtil(leftSide, rightSide,EquationDependencies::EQUATION,
                         reason, dependentDependencies, importanceLevel);
}

unsigned int DependenciesBank::addLength(const std::string& point1Id, const std::string& point2Id,
                                         const symbolicAlgebra::Expression& length,
                                         IDependency::Reason reason, const std::vector<size_t>& dependentDependencies,
                                         IDependency::ImportanceLevel importanceLevel) {
  const std::pair<std::string, std::vector<std::string>> name =
      getSegmentName(shapesBank->getPoint(point1Id), shapesBank->getPoint(point2Id));

  return  addEquationUtil(ExpressionModel(Var(name.first)), ExpressionModel(length), EquationDependencies::SEGMENT_LENGTH,
                         reason, dependentDependencies, importanceLevel);
}

unsigned int DependenciesBank::addConvexAngle(const std::string& point1Id, const std::string& vertexId,
                                              const std::string& point2Id,const symbolicAlgebra::Expression& value,
                                              IDependency::Reason reason,const std::vector<size_t>& dependentDependencies,
                                              IDependency::ImportanceLevel importanceLevel) {

  const std::vector<std::string> angle = changeAngleEnds(point1Id, vertexId, point2Id);
  const std::pair<std::string, std::vector<std::string>> name =
      getAngleName(shapesBank->getPoint(angle[0]), shapesBank->getPoint(angle[1]), shapesBank->getPoint(angle[2]), true);

  return  addEquationUtil(ExpressionModel(Var(name.first)), ExpressionModel(value), EquationDependencies::ANGLE_MEASURE,
                         reason, dependentDependencies, importanceLevel);
}

unsigned int DependenciesBank::addConcaveAngle(const std::string& point1Id, const std::string& vertexId,
                                               const std::string& point2Id, const symbolicAlgebra::Expression& value,
                                               IDependency::Reason reason, const std::vector<size_t>& dependentDependencies,
                                               IDependency::ImportanceLevel importanceLevel) {

  const std::vector<std::string> angle = changeAngleEnds(point1Id, vertexId, point2Id);
  const std::pair<std::string, std::vector<std::string>> name =
      getAngleName(shapesBank->getPoint(angle[0]), shapesBank->getPoint(angle[1]), shapesBank->getPoint(angle[2]), false);

  return  addEquationUtil(ExpressionModel(Var(name.first)), ExpressionModel(value), EquationDependencies::ANGLE_MEASURE,
                         reason, dependentDependencies, importanceLevel);
}

unsigned int DependenciesBank::addLinesDependency(const std::string& id1, const std::string& id2,
                                                  LinesDependencies type, IDependency::Reason reason,
                                                  const std::vector<size_t>& dependentDependencies,
                                                  IDependency::ImportanceLevel importanceLevel) {

  std::string index1{id1};
  std::string index2{id2};
  if (id1 > id2) {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<LinesDependency> dependency =
      std::make_shared<LinesDependency>(IdentifierModel(index1), IdentifierModel(index2), true, dependencyIdCounter,
                                        IDependency::Category::OF_LINES, static_cast<IDependency::Type>(type),
                                        reason, dependentDependencies, importanceLevel);

  const std::pair<bool, LinesDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclesDependency(const std::string& id1, const std::string& id2,
                                                    CirclesDependencies type, IDependency::Reason reason,
                                                    const std::vector<size_t>& dependentDependencies,
                                                    IDependency::ImportanceLevel importanceLevel) {

  std::string index1{id1};
  std::string index2{id2};
  if (id1 > id2) {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<CirclesDependency> dependency =
      std::make_shared<CirclesDependency>(IdentifierModel(index1), IdentifierModel(index2), true, dependencyIdCounter,
                                          IDependency::Category::OF_CIRCLES, static_cast<IDependency::Type>(type),
                                          reason, dependentDependencies, importanceLevel);

  const std::pair<bool, CirclesDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointsPairsDependency(const std::string& pair1End1, const std::string& pair1End2,
                                                       const std::string& pair2End1, const std::string& pair2End2,
                                                       PointsPairsDependencies type, IDependency::Reason reason,
                                                       const std::vector<size_t>& dependentDependencies,
                                                       IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PointsPairsDependency> dependency = std::make_shared<PointsPairsDependency>(
      PointsPairModel{pair1End1, pair1End2}, PointsPairModel{pair2End1, pair2End2}, true,
      dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS, static_cast<IDependency::Type>(type),
      reason, dependentDependencies, importanceLevel);

  const std::pair<bool, PointsPairsDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addAnglesDependency(const std::string& angle1Point1Id, const std::string& angle1VertexId,
                                                   const std::string& angle1Point2Id, AngleModel::AngleType angle1Type,
                                                   const std::string& angle2Point1Id, const std::string& angle2VertexId,
                                                   const std::string& angle2Point2Id, AngleModel::AngleType angle2Type,
                                                   AnglesDependencies type, IDependency::Reason reason,
                                                   const std::vector<size_t>& dependentDependencies,
                                                   IDependency::ImportanceLevel importanceLevel) {

  const std::vector<std::string> angle1 = changeAngleEnds(angle1Point1Id, angle1VertexId, angle1Point2Id);
  const std::vector<std::string> angle2 = changeAngleEnds(angle2Point1Id, angle2VertexId, angle2Point2Id);

  const std::shared_ptr<AnglesDependency> dependency = std::make_shared<AnglesDependency>(
      AngleModel{angle1[1], angle1[0], angle1[2], angle1Type},
      AngleModel{angle2[1], angle2[0], angle2[2], angle2Type}, true,
      dependencyIdCounter, IDependency::Category::OF_ANGLES, static_cast<IDependency::Type>(type),
      reason, dependentDependencies, importanceLevel);

  const std::pair<bool, AnglesDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonsDependency(const std::vector<std::string>& polygon1VerticesIds,
                                                     const std::vector<std::string>& polygon2VerticesIds,
                                                     bool fixedPointsOrder,
                                                     PolygonsDependencies type, IDependency::Reason reason,
                                                     const std::vector<size_t>& dependentDependencies,
                                                     IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PolygonsDependency> dependency = std::make_shared<PolygonsDependency>(
      PolygonModel{polygon1VerticesIds, fixedPointsOrder}, PolygonModel{polygon2VerticesIds, fixedPointsOrder},
      true, dependencyIdCounter, IDependency::Category::OF_POLYGONS, static_cast<IDependency::Type>(type),
      reason, dependentDependencies, importanceLevel);

  const std::pair<bool, PolygonsDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineCircleDependency(const std::string& lineId, const std::string& circleId,
                                                       LineCircleDependencies type, IDependency::Reason reason,
                                                       const std::vector<size_t>& dependentDependencies,
                                                       IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<LineCircleDependency> dependency = std::make_shared<LineCircleDependency>(
      IdentifierModel(lineId), IdentifierModel(circleId), false, dependencyIdCounter,
      IDependency::Category::OF_LINE_AND_CIRCLE, static_cast<IDependency::Type>(type),
      reason, dependentDependencies, importanceLevel);

  const std::pair<bool, LineCircleDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinePointsPairDependency(const std::string& lineId,
                                                          const std::string& pairEnd1Id, const std::string& pairEnd2Id,
                                                          LinePointsPairDependencies type, IDependency::Reason reason,
                                                          const std::vector<size_t>& dependentDependencies,
                                                          IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<LinePointsPairDependency> dependency = std::make_shared<LinePointsPairDependency>(
      IdentifierModel{lineId}, PointsPairModel{pairEnd1Id, pairEnd2Id}, false, dependencyIdCounter,
      IDependency::Category::OF_LINE_AND_POINTS_PAIR, static_cast<IDependency::Type>(type), reason,
      dependentDependencies, importanceLevel);

  const std::pair<bool, LinePointsPairDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineAngleDependency(const std::string& lineId, const std::string& anglePoint1Id,
                                                      const std::string& angleVertexId,const std::string& anglePoint2Id,
                                                      AngleModel::AngleType angleType,
                                                      LineAngleDependencies type, IDependency::Reason reason,
                                                      const std::vector<size_t>& dependentDependencies,
                                                      IDependency::ImportanceLevel importanceLevel) {

  const std::vector<std::string> angle = changeAngleEnds(anglePoint1Id, angleVertexId, anglePoint2Id);

  const std::shared_ptr<LineAngleDependency> dependency = std::make_shared<LineAngleDependency>(
      IdentifierModel(lineId),
      AngleModel{angle[1], angle[0], angle[2], angleType}, false,
      dependencyIdCounter, IDependency::Category::OF_LINE_AND_ANGLE, static_cast<IDependency::Type>(type),
      reason, dependentDependencies, importanceLevel);

  const std::pair<bool, LineAngleDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclePolygonDependency(const std::string& circleId,
                                                          const std::vector<std::string>& polygonVerticesIds,
                                                          CirclePolygonDependencies type, IDependency::Reason reason,
                                                          const std::vector<size_t>& dependentDependencies,
                                                          IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<CirclePolygonDependency> dependency = std::make_shared<CirclePolygonDependency>(
      IdentifierModel{circleId}, PolygonModel{polygonVerticesIds, false}, false, dependencyIdCounter,
      IDependency::Category::OF_CIRCLE_AND_POLYGON, static_cast<IDependency::Type>(type), reason,
      dependentDependencies, importanceLevel);

  const std::pair<bool, CirclePolygonDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonTypeDependency(const std::vector<std::string>& polygonVerticesIds,
                                                        PolygonModel::PolygonType polygonType,
                                                        PolygonTypeDependencies type, IDependency::Reason reason,
                                                        const std::vector<size_t>& dependentDependencies,
                                                        IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PolygonTypeDependency> dependency = std::make_shared<PolygonTypeDependency>(
      IdentifierModel{std::to_string(static_cast<size_t>(polygonType))}, PolygonModel{polygonVerticesIds, false}, false,
      dependencyIdCounter,IDependency::Category::POLYGON_TYPE, static_cast<IDependency::Type>(type), reason,
      dependentDependencies, importanceLevel);

  const std::pair<bool, PolygonTypeDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointsPairPairPointsPairDependency(const std::string& arm1End1,
                                                                     const std::string& arm1End2,
                                                                     const std::string& arm2End1,
                                                                     const std::string& arm2End2,
                                                                     const std::string& segmentEnd1,
                                                                     const std::string& segmentEnd2,
                                                                     PointsPairPairPointsPairDependencies type,
                                                                     IDependency::Reason reason,
                                                                     const std::vector<size_t>& dependentDependencies,
                                                                     IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PointsPairPairPointsPairDependency> dependency =
      std::make_shared<PointsPairPairPointsPairDependency>(ModelsPairModel{PointsPairModel{arm1End1, arm1End2},
                                                                           PointsPairModel{arm2End1, arm2End2}},
                                                           PointsPairModel{segmentEnd1, segmentEnd2}, false,
       dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR,
      static_cast<IDependency::Type>(type), reason, dependentDependencies, importanceLevel);

  const std::pair<bool, PointsPairPairPointsPairDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonPointsPairDependency(const std::vector<std::string>& polygonVerticesIds,
                                              bool fixedPointsOrder, const std::string& pairEnd1, const std::string& pairEnd2,
                                              PolygonPointsPairDependencies type, IDependency::Reason reason,
                                              const std::vector<size_t>& dependentDependencies, IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PolygonPointsPairDependency> dependency =
      std::make_shared<PolygonPointsPairDependency>(PolygonModel{polygonVerticesIds, fixedPointsOrder}, PointsPairModel{pairEnd1, pairEnd2},
       false, dependencyIdCounter, IDependency::Category::OF_POLYGON_AND_POINTS_PAIRS,
      static_cast<IDependency::Type>(type), reason, dependentDependencies, importanceLevel);

  const std::pair<bool, PolygonPointsPairDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonExpressionDependency(const std::vector<std::string>& polygonVerticesIds,
                                            bool fixedPointsOrder, const symbolicAlgebra::Expression& expression,
                                            PolygonExpressionDependencies type, IDependency::Reason reason,
                                            const std::vector<size_t>& dependentDependencies, IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PolygonExpressionDependency> dependency =
      std::make_shared<PolygonExpressionDependency>(PolygonModel{polygonVerticesIds, fixedPointsOrder}, ExpressionModel{expression},
       false, dependencyIdCounter, IDependency::Category::OF_POLYGON_AND_EXPRESSION,
      static_cast<IDependency::Type>(type), reason, dependentDependencies, importanceLevel);

  const std::pair<bool, PolygonExpressionDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  addNewDependency(dependency);
  expressionModels.push_back(&dependency->getSecondObject());
  return 1;
}

unsigned int DependenciesBank::extractVariables() {
  unsigned int sumOfNewDependencies{0};

  for (auto variableDescriptors = formulasIncludingVariable.begin(); variableDescriptors != formulasIncludingVariable.end(); ++variableDescriptors) {
    if (variableDescriptors->second.size()  < 2) {
      continue;
    }

    std::vector<symbolicAlgebra::Equation> equations{};
    std::vector<size_t> equationIds{};
    for (auto inputEquationId = variableDescriptors->second.begin(); inputEquationId != variableDescriptors->second.end(); ++inputEquationId) {
      const auto inputEquation = std::dynamic_pointer_cast<EquationDependency>(dependenciesVector.at(*inputEquationId));
      equations.emplace_back(inputEquation->getFirstObject(), inputEquation->getSecondObject());
      equationIds.emplace_back(*inputEquationId);
    }

    const std::vector<symbolicAlgebra::Equation> partSol = symbolicAlgebra::Solve::systemOfEquations(equations);

    for(auto pSol = partSol.begin(); pSol != partSol.end(); ++pSol) {
      bool identity = false;
      for(auto equ = equations.begin(); equ != equations.end(); ++equ) {
        auto maybeZero1 = equ->lhs - equ->rhs - pSol->lhs + pSol->rhs;
        auto maybeZero2 = equ->lhs - equ->rhs + pSol->lhs - pSol->rhs;
        maybeZero1.simplify();
        maybeZero2.simplify();
        if(maybeZero1.canBeFullEvaluated() && fabs(maybeZero1.evaluate()) < MathHelper::COMPARISON_EPSILON ||
           maybeZero2.canBeFullEvaluated() && fabs(maybeZero2.evaluate()) < MathHelper::COMPARISON_EPSILON) {
          identity = true;
        }
      }

      if(!identity) {
        auto equalSides = pSol->lhs - pSol->rhs;
        equalSides.simplify();

       // if(equalSides.canBeFullEvaluated() && fabs(equalSides.evaluate()) < MathHelper::COMPARISON_EPSILON) {
       //   continue;
       // }

        std::set<std::string> vars;
        pSol->lhs.getIncludedVariables(vars);
        pSol->rhs.getIncludedVariables(vars);
       // if(vars.size() > 2) {
        //  continue;
        //}

        sumOfNewDependencies += addEquationUtil(pSol->lhs, pSol->rhs, EquationDependencies::EQUATION,
                                                IDependency::Reason::EQUATION_EXTRACTION, equationIds,
                                                IDependency::ImportanceLevel::LOW);
      }
    }
  }

  if(sumOfNewDependencies > 0) {
    heuristicsBank->markNewEvaluationsFlag();
  }

  return sumOfNewDependencies;
}

json DependenciesBank::getDependenciesAsJsonObjects() const {
  json result;

  std::map<IDependency::Type, std::vector<size_t>>::const_iterator mIt;
  std::vector<size_t>::const_iterator vIt;
  for (mIt = dependenciesMap.begin(); mIt != dependenciesMap.end(); ++mIt) {
    for (vIt = mIt->second.begin(); vIt != mIt->second.end(); ++vIt) {
      result.push_back(dependenciesVector.at((*vIt))->getObjectAsJson());
    }
  }

  return result;
}

json DependenciesBank::getVariablesIndexesAsJsonObject() const {
  json result;

  std::map<std::string, std::vector<size_t>>::const_iterator iter;
  for (iter = variableToIndexes.begin(); iter != variableToIndexes.end(); ++iter) {
    result.push_back({
        {"name", iter->first},
        {"ids", json(iter->second)},
    });
  }

  return result;
}

std::pair<bool, std::vector<size_t>> DependenciesBank::evaluateEquation(const ExpressionModel& leftSide, const ExpressionModel& rightSide) const {
  std::set<std::string> variables{};
  leftSide.getIncludedVariables(variables);
  rightSide.getIncludedVariables(variables);

  if (!heuristicsBank->equationsAreDeterminable(variables)) {
    return {false, {}};
  }

  for (const EquationDependencies type: { EquationDependencies::EQUATION, EquationDependencies::SEGMENT_LENGTH, EquationDependencies::ANGLE_MEASURE }) {
    const auto equations = getEquationDependencies(type);

    for(auto equation = equations.begin(); equation != equations.end(); ++equation) {
      const bool cond1 = (*equation)->getFirstObject() == rightSide && (*equation)->getSecondObject() == leftSide;
      const bool cond2 = (*equation)->getFirstObject() == leftSide && (*equation)->getSecondObject() == rightSide;
      if (cond1 || cond2) {
        return {true, {(*equation)->getId()}};
      }
    }
  }

  return {false, {}};
}

std::pair<bool, std::vector<size_t>> DependenciesBank::evaluateEquation(const symbolicAlgebra::Variable& leftSide, const symbolicAlgebra::Variable& rightSide) const {
  std::set<std::string> variables{};
  leftSide.getIncludedVariables(variables);
  rightSide.getIncludedVariables(variables);

  if (!heuristicsBank->equationsAreDeterminable(variables)) {
    return {false, {}};
  }

  if(leftSide == rightSide) {
    return {true, {}};
  }

  for (const EquationDependencies type: { EquationDependencies::EQUATION, EquationDependencies::SEGMENT_LENGTH, EquationDependencies::ANGLE_MEASURE }) {
    const auto equations = getEquationDependencies(type);

    for(auto equation = equations.begin(); equation != equations.end(); ++equation) {
      const bool cond1 = (*equation)->getFirstObject() == rightSide && (*equation)->getSecondObject() == leftSide;
      const bool cond2 = (*equation)->getFirstObject() == leftSide && (*equation)->getSecondObject() == rightSide;
      if (cond1 || cond2) {
        return {true, {(*equation)->getId()}};
      }
    }
  }

  return {false, {}};
}

unsigned int DependenciesBank::addEquationUtil(const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide,
                             EquationDependencies type, IDependency::Reason reason, const std::vector<size_t>& dependentDependencies,
                             IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(rightSide), ExpressionModel(leftSide), true, dependencyIdCounter, IDependency::Category::FORMULA,
      static_cast<IDependency::Type>(type), reason, dependentDependencies, importanceLevel);

  const std::pair<bool, EquationDependency*> dependencySearch = checkIfDependencyExist(dependency);
  if (dependencySearch.first) {
    if(checkIfDependencyIsPredecessor(dependency)) {
      return 0;
    }

    const bool reasonExist = std::find(dependencySearch.second->getReasons().begin(), dependencySearch.second->getReasons().end(), reason)
                             != dependencySearch.second->getReasons().end();
    if(!reasonExist) {
      dependencySearch.second->addNewPredecessor(reason, dependentDependencies, importanceLevel);
    }
    return 0;
  }

  std::set<std::string> names;
  leftSide.getIncludedVariables(names);
  rightSide.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  const std::vector<std::string> namesVector(names.begin(), names.end());
  heuristicsBank->addNewFormula(namesVector);

  formulas.push_back(dependencyIdCounter);
  addNewDependency(dependency);
  expressionModels.push_back(&dependency->getFirstObject());
  expressionModels.push_back(&dependency->getSecondObject());

  return 1;
}

void DependenciesBank::addNewDependency(std::shared_ptr<IDependency> dependencyModel) {
  if(heuristicsBank != nullptr) {
    heuristicsBank->markNewDependencyFlag(dependencyModel->getType());
  }

  dependenciesMap.at(dependencyModel->getType()).emplace_back(dependencyIdCounter);
  dependenciesVector.emplace_back(std::move(dependencyModel));
  dependencyIdCounter++;
}

std::pair<std::string, std::vector<std::string>> DependenciesBank::getSegmentName(const PointModel& point1, const PointModel& point2) {
  const std::string& point1Name = point1.getName();
  const std::string& point2Name = point2.getName();

  if(point1Name.length() < point2Name.length()) {
    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId()};
    return std::make_pair("|" + point1Name + point2Name + "|", pointsOrder);
  }

  if(point1Name.length() > point2Name.length()) {
    const std::vector<std::string> pointsOrder = {point2.getId(), point1.getId()};
    return std::make_pair("|" + point2Name + point1Name + "|", pointsOrder);
  }

  if(point1Name < point2Name) {
    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId()};
    return std::make_pair("|" + point1Name + point2Name + "|", pointsOrder);
  }

  const std::vector<std::string> pointsOrder = {point2.getId(), point1.getId()};
  return std::make_pair("|" + point2Name + point1Name + "|", pointsOrder);
}

std::pair<std::string, std::vector<std::string>> DependenciesBank::getAngleName(const PointModel& point1, const PointModel& point2, const PointModel& point3, bool angleIsConvex) {
  const std::vector<std::string> properAnglePoints = changeAngleEnds(point1.getId(), point2.getId(), point3.getId());

  const std::string& point1Name = shapesBank->getPoint(properAnglePoints[0]).getName();
  const std::string& point2Name = shapesBank->getPoint(properAnglePoints[1]).getName();
  const std::string& point3Name = shapesBank->getPoint(properAnglePoints[2]).getName();

  if(point1Name.length() < point3Name.length()) {
    if(angleIsConvex) {
      const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
      return std::make_pair("|>" + point1Name + point2Name + point3Name + "|", pointsOrder);
    }

    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
    return std::make_pair("|<" + point1Name + point2Name + point3Name + "|", pointsOrder);
  }

  if(point1Name.length() > point3Name.length()) {
    if(angleIsConvex) {
      const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
      return std::make_pair("|>" + point3Name + point2Name + point1Name + "|", pointsOrder);
    }

    const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
    return std::make_pair("|<" + point3Name + point2Name + point1Name + "|", pointsOrder);
  }

  if(point1Name < point3Name) {
    if(angleIsConvex) {
      const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
      return std::make_pair("|>" + point1Name + point2Name + point3Name + "|", pointsOrder);
    }

    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
    return std::make_pair("|<" + point1Name + point2Name + point3Name + "|", pointsOrder);
  }

  if(angleIsConvex) {
    const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
    return std::make_pair("|>" + point3Name + point2Name + point1Name + "|", pointsOrder);
  }

  const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
  return std::make_pair("|<" + point3Name + point2Name + point1Name + "|", pointsOrder);
}

std::vector<std::string> DependenciesBank::changeAngleEnds(const std::string& point1Id, const std::string& vertexId, const std::string& point2Id) {
  if(shapesBank == nullptr) {
    return std::vector<std::string>{point1Id, vertexId, point2Id};
  }

  std::string point1IdTmp{point1Id};
  std::string point2IdTmp{point2Id};

  if(point1IdTmp != vertexId) {
    try {
      const std::string arm1Id = shapesBank->getLineIdThrowTwoPoints(point1IdTmp, vertexId);
      const std::vector<std::string>& arm1Points = shapesBank->getLine(arm1Id).getIncludedPoints();

      const auto iterOfVertexOnArm1 = std::find(arm1Points.begin(), arm1Points.end(), vertexId) - arm1Points.begin();
      const auto iterOfPoint1OnArm1 = std::find(arm1Points.begin(), arm1Points.end(), point1IdTmp) - arm1Points.begin();

      if (iterOfPoint1OnArm1 > iterOfVertexOnArm1) {
        point1IdTmp = arm1Points.back();
      }
      else {
        point1IdTmp = arm1Points.front();
      }
    } catch (std::invalid_argument const& exception) {}
  }

  if(point2IdTmp != vertexId) {
    try {
      const std::string arm2Id = shapesBank->getLineIdThrowTwoPoints(vertexId, point2IdTmp);
      const std::vector<std::string>& arm2Points = shapesBank->getLine(arm2Id).getIncludedPoints();

      const auto iterOfVertexOnArm2 = std::find(arm2Points.begin(), arm2Points.end(), vertexId) - arm2Points.begin();
      const auto iterOfPoint2OnArm2 = std::find(arm2Points.begin(), arm2Points.end(), point2IdTmp) - arm2Points.begin();

      if (iterOfPoint2OnArm2 > iterOfVertexOnArm2) {
        point2IdTmp = arm2Points.back();
      }
      else {
        point2IdTmp = arm2Points.front();
      }
    } catch (std::invalid_argument const& exception) {}
  }

  return std::vector<std::string>{point1IdTmp, vertexId, point2IdTmp};
}
}  // namespace expertBackground