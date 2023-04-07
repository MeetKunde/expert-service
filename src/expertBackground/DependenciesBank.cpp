#include "DependenciesBank.h"

namespace expertBackground {
const std::string DependenciesBank::capitalLetters = "ABCDEFGHIJKLMNOPQRSTUWVXYZ";

DependenciesBank::DependenciesBank()
    : dependencyIdCounter(0),
      valuesBankDimension(0),
      shapesBank(nullptr),
      dependenciesVector({}),
      dependenciesMap({}),
      variableToIndexes({}),
      formulas({}),
      formulasIncludingVariable({}),
      extractedEquations({}) {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    dependenciesMap.insert({(*iter), {}});
  }
}

DependenciesBank::DependenciesBank(const ShapesBank* generatedShapesBank)
    : dependencyIdCounter(0),
      valuesBankDimension(generatedShapesBank->getPointsNumber()),
      shapesBank(generatedShapesBank),
      dependenciesVector({}),
      dependenciesMap({}),
      formulas({}),
      formulasIncludingVariable({}),
      extractedEquations({}) {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    dependenciesMap.insert({(*iter), {}});
  }

  std::string name;

  for (unsigned int point1Id = 0; point1Id < valuesBankDimension; point1Id++) {
    for (unsigned int point2Id = 0; point2Id <= point1Id; point2Id++) {
      name = getLengthName(point1Id, point2Id);
      variableToIndexes.insert({name, {point1Id, point2Id}});
      formulasIncludingVariable.insert({name, {}});
      extractedEquations.insert({name, {}});
    }
  }

  for (unsigned int point1Id = 0; point1Id < valuesBankDimension; point1Id++) {
    for (unsigned int point2Id = 0; point2Id < valuesBankDimension; point2Id++) {
      for (unsigned int point3Id = 0; point3Id <= point2Id; point3Id++) {
        name = getAngleName(point3Id, point1Id, point2Id, true);
        variableToIndexes.insert({name, {point3Id, point1Id, point2Id, 0}});
        formulasIncludingVariable.insert({name, {}});
        extractedEquations.insert({name, {}});

        name = getAngleName(point3Id, point1Id, point2Id, false);
        variableToIndexes.insert({name, {point3Id, point1Id, point2Id, 1}});
        formulasIncludingVariable.insert({name, {}});
        extractedEquations.insert({name, {}});
      }
    }
  }

  for (unsigned int pointId = 0; pointId < valuesBankDimension; pointId++) {
    addLength(pointId, pointId, Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::POINTS_ARE_THE_SAME, {},
              IDependency::UsefulnessLevel::LOW);

    addConvexAngle(pointId, pointId, pointId, Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::POINTS_ARE_THE_SAME, {},
                   IDependency::UsefulnessLevel::LOW);
    addConcaveAngle(pointId, pointId, pointId, Int(MathHelper::ROUND_ANGLE_VALUE), IDependency::Reason::POINTS_ARE_THE_SAME, {},
                    IDependency::UsefulnessLevel::LOW);
  }

  for (unsigned int point1Id = 0; point1Id < valuesBankDimension; point1Id++) {
    for (unsigned int point2Id = 0; point2Id < valuesBankDimension; point2Id++) {
      if (point1Id != point2Id) {
        addConvexAngle(point2Id, point1Id, point2Id, Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME,
                       {}, IDependency::UsefulnessLevel::LOW);
        addConcaveAngle(point2Id, point1Id, point2Id, Int(MathHelper::ROUND_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME,
                        {}, IDependency::UsefulnessLevel::LOW);
      }
    }
  }
}

unsigned int DependenciesBank::addLength(unsigned int id1, unsigned int id2, const symbolicAlgebra::Expression& length,
                                         IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                         IDependency::UsefulnessLevel usefulness) {
  const std::string name = getLengthName(id1, id2);

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(Var(name)), ExpressionModel(length), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::SEGMENT_LENGTH, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  names.insert(name);
  length.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
      extractedEquations.insert({*iter, {}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  add(dependency);
  return 1;
}

Var DependenciesBank::getLengthVariable(unsigned int id1, unsigned int id2) {
  return Var(getLengthName(id1, id2));
}

unsigned int DependenciesBank::addConvexAngle(unsigned int id1, unsigned int vertexId, unsigned int id2,
                                              const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                                              std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(&id1, &vertexId, &id2);
  const std::string name = getAngleName(id1, vertexId, id2, true);

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(Var(name)), ExpressionModel(value), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::ANGLE_VALUE, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  names.insert(name);
  value.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
      extractedEquations.insert({*iter, {}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  add(dependency);
  return 1;
}

Var DependenciesBank::getConvexAngleVariable(unsigned int id1, unsigned int vertexId, unsigned int id2) {
  changeAngleEnds(&id1, &vertexId, &id2);

  return Var(getAngleName(id1, vertexId, id2, true));
}

unsigned int DependenciesBank::addConcaveAngle(unsigned int id1, unsigned int vertexId, unsigned int id2,
                                               const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                                               std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(&id1, &vertexId, &id2);
  const std::string name = getAngleName(id1, vertexId, id2, false);

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(Var(name)), ExpressionModel(value), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::ANGLE_VALUE, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  names.insert(name);
  value.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
      extractedEquations.insert({*iter, {}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  add(dependency);
  return 1;
}

Var DependenciesBank::getConcaveAngleVariable(unsigned int id1, unsigned int vertexId, unsigned int id2) {
  changeAngleEnds(&id1, &vertexId, &id2);

  return Var(getAngleName(id1, vertexId, id2, false));
}

unsigned int DependenciesBank::addLinesDependency(unsigned int id1, unsigned int id2, IDependency::Type type,
                                                  IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                                  IDependency::UsefulnessLevel usefulness) {
  unsigned int index1;
  unsigned int index2;
  if (id1 > id2) {
    index1 = id1;
    index2 = id2;
  }
  else {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<LinesDependency> dependency =
      std::make_shared<LinesDependency>(IdentifierModel(index1), IdentifierModel(index2), dependencyIdCounter,
                                        IDependency::Category::OF_LINES, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclesDependency(unsigned int id1, unsigned int id2, IDependency::Type type,
                                                    IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                                    IDependency::UsefulnessLevel usefulness) {
  unsigned int index1;
  unsigned int index2;
  if (id1 > id2) {
    index1 = id1;
    index2 = id2;
  }
  else {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<CirclesDependency> dependency =
      std::make_shared<CirclesDependency>(IdentifierModel(index1), IdentifierModel(index2), dependencyIdCounter,
                                          IDependency::Category::OF_CIRCLES, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointPairsDependency(unsigned int pair1End1, unsigned int pair1End2, unsigned int pair2End1,
                                                       unsigned int pair2End2, IDependency::Type type, IDependency::Reason reason,
                                                       std::vector<unsigned int> basedOn,
                                                       IDependency::UsefulnessLevel usefulness) {
  const PointsPairModel pair1(pair1End1, pair1End2);
  const PointsPairModel pair2(pair2End1, pair2End2);

  const std::shared_ptr<PointPairsDependency> dependency = std::make_shared<PointsPairsDependency>(
      pair1, pair2, dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addAnglesDependency(unsigned int angle1Point1Id, unsigned int angle1VertexId,
                                                   unsigned int angle1Point2Id, AngleModel::AngleType angle1Type,
                                                   unsigned int angle2Point1Id, unsigned int angle2VertexId,
                                                   unsigned int angle2Point2Id, AngleModel::AngleType angle2Type,
                                                   IDependency::Type type, IDependency::Reason reason,
                                                   std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(&angle1Point1Id, &angle1VertexId, &angle1Point2Id);
  changeAngleEnds(&angle2Point1Id, &angle2VertexId, &angle2Point2Id);

  const AngleModel angle1(angle1VertexId, angle1Point1Id, angle1Point2Id, angle1Type);
  const AngleModel angle2(angle2VertexId, angle2Point1Id, angle2Point2Id, angle2Type);

  const std::shared_ptr<AnglesDependency> dependency = std::make_shared<AnglesDependency>(
      angle1, angle2, dependencyIdCounter, IDependency::Category::OF_ANGLES, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonsDependency(std::vector<unsigned int> polygon1VerticesIds,
                                                     std::vector<unsigned int> polygon2VerticesIds, IDependency::Type type,
                                                     IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                                     IDependency::UsefulnessLevel usefulness) {
  const std::shared_ptr<PolygonsDependency> dependency = std::make_shared<PolygonsDependency>(
      PolygonModel(std::move(polygon1VerticesIds)), PolygonModel(std::move(polygon2VerticesIds)), dependencyIdCounter,
      IDependency::Category::OF_POLYGONS, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineCircleDependency(unsigned int lineId, unsigned int circleId, IDependency::Type type,
                                                       IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                                       IDependency::UsefulnessLevel usefulness) {
  const std::shared_ptr<IdentifiersDependency> dependency = std::make_shared<IdentifiersDependency>(
      IdentifierModel(lineId), IdentifierModel(circleId), dependencyIdCounter, IDependency::Category::OF_LINE_AND_CIRCLE, type,
      reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinePointPairDependency(unsigned int lineId, unsigned int pairEnd1Id, unsigned pairEnd2Id,
                                                          IDependency::Type type, IDependency::Reason reason,
                                                          std::vector<unsigned int> basedOn,
                                                          IDependency::UsefulnessLevel usefulness) {

  const std::shared_ptr<IdentifierAndPointsPairDependency> dependency = std::make_shared<IdentifierAndPointsPairDependency>(
      IdentifierModel(lineId), PointsPairModel(pairEnd1Id, pairEnd2Id), dependencyIdCounter,
      IDependency::Category::OF_LINE_AND_POINTS_PAIR, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineAngleDependency(unsigned int lineId, unsigned int anglePoint1Id, unsigned int angleVertexId,
                                                      unsigned int anglePoint2Id, AngleModel::AngleType angleType,
                                                      IDependency::Type type, IDependency::Reason reason,
                                                      std::vector<unsigned int> basedOn,
                                                      IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(&anglePoint1Id, &angleVertexId, &anglePoint2Id);

  const std::shared_ptr<IdentifierAndAngleDependency> dependency = std::make_shared<IdentifierAndAngleDependency>(
      IdentifierModel(lineId), AngleModel(angleVertexId, anglePoint1Id, anglePoint2Id, angleType), dependencyIdCounter,
      IDependency::Category::OF_LINE_AND_ANGLE, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclePolygonDependency(unsigned int circleId, std::vector<unsigned int> polygonVerticesIds,
                                                          IDependency::Type type, IDependency::Reason reason,
                                                          std::vector<unsigned int> basedOn,
                                                          IDependency::UsefulnessLevel usefulness) {

  const std::shared_ptr<IdentifierAndPolygonDependency> dependency = std::make_shared<IdentifierAndPolygonDependency>(
      IdentifierModel(circleId), PolygonModel(std::move(polygonVerticesIds)), dependencyIdCounter,
      IDependency::Category::OF_CIRCLE_AND_POLYGON, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonTypeDependency(std::vector<unsigned int> polygonVerticesIds,
                                                        PolygonModel::PolygonType polygonType, IDependency::Type type,
                                                        IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                                        IDependency::UsefulnessLevel usefulness) {
  const std::shared_ptr<IdentifierAndPolygonDependency> dependency = std::make_shared<IdentifierAndPolygonDependency>(
      IdentifierModel(static_cast<unsigned int>(polygonType)), PolygonModel(std::move(polygonVerticesIds)), dependencyIdCounter,
      IDependency::Category::POLYGON_TYPE, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointsPairsPairPointsPairDependency(unsigned int arm1End1, unsigned int arm1End2,
                                                                      unsigned int arm2End1, unsigned int arm2End2,
                                                                      unsigned int segmentEnd1, unsigned int segmentEnd2,
                                                                      IDependency::Type type, IDependency::Reason reason,
                                                                      std::vector<unsigned int> basedOn,
                                                                      IDependency::UsefulnessLevel usefulness) {

  const std::shared_ptr<PairOfPointsPairPointsPairDependency> dependency = std::make_shared<PairOfPointsPairPointsPairDependency>(
      ModelsPairModel<PointsPairModel>(PointsPairModel(arm1End1, arm1End2), PointsPairModel(arm2End1, arm2End2)),
      PointsPairModel(segmentEnd1, segmentEnd2), dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR,
      type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addEquation(const symbolicAlgebra::Expression& leftSide,
                                           const symbolicAlgebra::Expression& rightSide, IDependency::Reason reason,
                                           std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness) {
  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(leftSide), ExpressionModel(rightSide), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::EQUATION, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  leftSide.getIncludedVariables(names);
  rightSide.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
      extractedEquations.insert({*iter, {}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  add(dependency);
  return 1;
}

Equ DependenciesBank::getEquation(unsigned int dependencyId) const {
  const std::shared_ptr<EquationDependency> dependency = std::static_pointer_cast<EquationDependency>(dependenciesVector.at(dependencyId));
  return Equ(dependency->getFirstElement(), dependency->getSecondElement());
}

void DependenciesBank::extractEquations() {
  std::vector<symbolicAlgebra::Equation> equations;
  for (std::vector<unsigned int>::const_iterator iter = formulas.cbegin(); iter != formulas.cend(); ++iter) {
    equations.push_back(getEquation(*iter));
  }

  const std::vector<symbolicAlgebra::Equation> solution = symbolicAlgebra::Solve::systemOfEquations(equations);
  for (std::vector<symbolicAlgebra::Equation>::const_iterator iter = solution.cbegin(); iter != solution.cend(); ++iter) {
    extractedEquations.at(iter->lhs.getString()).push_back(ExpressionModel(iter->rhs));
    addEquation(iter->lhs, iter->rhs, IDependency::Reason::EQUATION_EXTRACTION, {}, IDependency::UsefulnessLevel::LOW);
  }
}

std::pair<DependenciesBank::EvaluationResult, std::vector<unsigned int>> DependenciesBank::evaluateEquation(
    const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide) {
  std::set<std::string> variables;

  leftSide.getIncludedVariables(variables);
  rightSide.getIncludedVariables(variables);

  return evaluateEquation(leftSide, rightSide, variables);
}

std::pair<DependenciesBank::EvaluationResult, std::vector<unsigned int>> DependenciesBank::evaluateEquation(
    const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide, std::set<std::string>& variables) {
  if (variables.empty()) {
    std::set<std::string> vars;
    leftSide.getIncludedVariables(vars);
    rightSide.getIncludedVariables(vars);

    if (symbolicAlgebra::Solve::compare(leftSide, rightSide)) {
      return {DependenciesBank::EvaluationResult::SUCCESS, {}};
    }

    if (vars.empty()) {
      return {DependenciesBank::EvaluationResult::FAILURE, {}};
    }

    return {DependenciesBank::EvaluationResult::UNDEFINED, {}};
  }

  const std::string var = *variables.begin();
  variables.erase(variables.begin());

  for (std::vector<ExpressionModel>::iterator equ = extractedEquations.at(var).begin(); equ != extractedEquations.at(var).end();
       ++equ) {
    symbolicAlgebra::Expression newLeft = symbolicAlgebra::Expression(leftSide);
    symbolicAlgebra::Expression newRight = symbolicAlgebra::Expression(rightSide);

    newLeft.substitute(var, *equ);
    newRight.substitute(var, *equ);

    std::pair<EvaluationResult, std::vector<unsigned int>> res = evaluateEquation(newLeft, newRight, variables);
    if (res.first == DependenciesBank::EvaluationResult::SUCCESS) {
      return res;
    }
  }

  return {DependenciesBank::EvaluationResult::UNDEFINED, {}};
}

json DependenciesBank::getDependenciesAsJsonObjects() {
  json result;

  std::map<IDependency::Type, std::vector<unsigned int>>::iterator mIt;
  std::vector<unsigned int>::iterator vIt;
  for (mIt = dependenciesMap.begin(); mIt != dependenciesMap.end(); ++mIt) {
    std::vector<json> dependencies;
    for (vIt = mIt->second.begin(); vIt != mIt->second.end(); ++vIt) {
      dependencies.push_back(dependenciesVector.at((*vIt))->getObjectAsJson());
    }

    result.push_back({
        {"type", mIt->first},
        {"dependencies", json(dependencies)},
    });
  }

  return result;
}

json DependenciesBank::getVariablesIndexesAsJsonObject() {
  json result;

  std::map<std::string, std::vector<unsigned int>>::iterator iter;
  for (iter = variableToIndexes.begin(); iter != variableToIndexes.end(); ++iter) {
    result.push_back({
        {"name", iter->first},
        {"ids", json(iter->second)},
    });
  }

  return result;
}

std::string DependenciesBank::getPointName(unsigned int identifier) {
  std::string name;
  name += DependenciesBank::capitalLetters[identifier % DependenciesBank::capitalLetters.size()];

  // if there are more points than letters of the alphabet, add apostrophes
  while (identifier > (DependenciesBank::capitalLetters.size() - 1)) {
    name += "'";
    identifier -= DependenciesBank::capitalLetters.size();
  }

  return name;
}

std::string DependenciesBank::getLengthName(unsigned int end1Id, unsigned int end2Id) {
  if (end1Id < end2Id) {
    return "|" + getPointName(end1Id) + getPointName(end2Id) + "|";
  }

  return "|" + getPointName(end2Id) + getPointName(end1Id) + "|";
}

std::string DependenciesBank::getAngleName(unsigned int end1Id, unsigned int vertexId, unsigned int end2Id, bool isConvex) {
  if (end1Id < end2Id) {
    if (isConvex) {
      return "|>" + getPointName(end1Id) + getPointName(vertexId) + getPointName(end2Id) + "|";
    }

    return "|<" + getPointName(end1Id) + getPointName(vertexId) + getPointName(end2Id) + "|";
  }

  if (isConvex) {
    return "|>" + getPointName(end2Id) + getPointName(vertexId) + getPointName(end1Id) + "|";
  }

  return "|<" + getPointName(end2Id) + getPointName(vertexId) + getPointName(end1Id) + "|";
}

void DependenciesBank::changeAngleEnds(unsigned int* id1, unsigned int* vertexId, unsigned int* id2) {
  try {
    const std::vector<unsigned int>& arm1Points =
        shapesBank->getLine(shapesBank->getLineIdThrowTwoPoints(*id1, *vertexId)).getIncludedPoints();

    const unsigned int indexOfVertexOnArm1 =
        std::distance(arm1Points.begin(), std::find(arm1Points.begin(), arm1Points.end(), *vertexId));
    const unsigned int indexOfId1 = std::distance(arm1Points.begin(), std::find(arm1Points.begin(), arm1Points.end(), *id1));

    if (indexOfId1 > indexOfVertexOnArm1) {
      *id1 = arm1Points.back();
    }
    else {
      *id1 = arm1Points.front();
    }
  } catch (std::invalid_argument const& exception) {}

  try {
    const std::vector<unsigned int>& arm2Points =
        shapesBank->getLine(shapesBank->getLineIdThrowTwoPoints(*id2, *vertexId)).getIncludedPoints();
    const unsigned int indexOfVertexOnArm2 =
        std::distance(arm2Points.begin(), std::find(arm2Points.begin(), arm2Points.end(), *vertexId));
    const unsigned int indexOfId2 = std::distance(arm2Points.begin(), std::find(arm2Points.begin(), arm2Points.end(), *id2));

    if (indexOfId2 > indexOfVertexOnArm2) {
      *id2 = arm2Points.back();
    }
    else {
      *id2 = arm2Points.front();
    }
  } catch (std::invalid_argument const& exception) {}
}

void DependenciesBank::add(std::shared_ptr<IDependency> dependencyModel) {
  dependenciesMap.at(dependencyModel->getType()).emplace_back(dependencyIdCounter);
  dependenciesVector.emplace_back(std::move(dependencyModel));
  dependencyIdCounter++;
}
}  // namespace expertBackground