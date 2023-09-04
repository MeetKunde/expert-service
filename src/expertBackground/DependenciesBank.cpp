#include "DependenciesBank.h"

namespace expertBackground {
DependenciesBank::DependenciesBank()
    : dependencyIdCounter{0},
      shapesBank{nullptr} {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    dependenciesMap.insert({(*iter), {}});
  }
}

DependenciesBank::DependenciesBank(const ShapesBank* generatedShapesBank)
    : dependencyIdCounter(0),
      shapesBank(generatedShapesBank) {
  std::vector<IDependency::Type>::const_iterator iter;
  for (iter = IDependency::dependencyTypes.begin(); iter != IDependency::dependencyTypes.end(); ++iter) {
    dependenciesMap.insert({ (*iter), { } });
  }

  const std::vector<PointModel> points = shapesBank->getPointsVector();

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos <= point1Pos; point2Pos++) {
      const std::string name = getLengthName(points.at(point1Pos), points.at(point2Pos));
      variableToIndexes.insert({ name, { point1Pos, point2Pos } });
      formulasIncludingVariable.insert({ name, { } });
      extractedEquations.insert({ name, { } });
    }
  }

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < shapesBank->getPointsNumber(); point2Pos++) {
      for (size_t point3Pos = 0; point3Pos <= point1Pos; point3Pos++) {
        const std::string convexName = getAngleName(points.at(point1Pos), points.at(point2Pos), points.at(point3Pos), true);
        variableToIndexes.insert({ convexName, {point1Pos, point2Pos, point3Pos, static_cast<size_t>(true)} });
        formulasIncludingVariable.insert({ convexName, { } });
        extractedEquations.insert( {convexName, { } });

        const std::string concaveName = getAngleName(points.at(point1Pos), points.at(point2Pos), points.at(point3Pos), false);
        variableToIndexes.insert({ convexName, {point1Pos, point2Pos, point3Pos, static_cast<size_t>(false)} });
        formulasIncludingVariable.insert({ convexName, { } });
        extractedEquations.insert( {convexName, { } });
      }
    }
  }

  for (size_t pointPos = 0; pointPos < shapesBank->getPointsNumber(); pointPos++) {
    addLength(points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ZERO_ANGLE_VALUE),
              IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::UsefulnessLevel::LOW);

    addConvexAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ZERO_ANGLE_VALUE),
                   IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::UsefulnessLevel::LOW);
    addConcaveAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ROUND_ANGLE_VALUE),
                    IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::UsefulnessLevel::LOW);
  }

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < shapesBank->getPointsNumber(); point2Pos++) {
      if (point1Pos != point2Pos) {
        addConvexAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                       Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME, {}, IDependency::UsefulnessLevel::LOW);
        addConcaveAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                        Int(MathHelper::ROUND_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME, {}, IDependency::UsefulnessLevel::LOW);
      }
    }
  }
}

unsigned int DependenciesBank::addLength(std::string point1Id, std::string point2Id, const symbolicAlgebra::Expression& length,
                                         IDependency::Reason reason, std::vector<size_t> basedOn,
                                         IDependency::UsefulnessLevel usefulness) {
  const std::string name = getLengthName(shapesBank->getPoint(point1Id), shapesBank->getPoint(point2Id));

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

unsigned int DependenciesBank::addConvexAngle(std::string point1Id, std::string vertexId, std::string point2Id,
                                              const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                                              std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(point1Id, vertexId, point2Id);
  const std::string name =
      getAngleName(shapesBank->getPoint(point1Id), shapesBank->getPoint(vertexId), shapesBank->getPoint(point2Id), true);

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

unsigned int DependenciesBank::addConcaveAngle(std::string point1Id, std::string vertexId, std::string point2Id,
                                               const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                                               std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(point1Id, vertexId, point2Id);
  const std::string name =
      getAngleName(shapesBank->getPoint(point1Id), shapesBank->getPoint(vertexId), shapesBank->getPoint(point2Id), true);

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

unsigned int DependenciesBank::addLinesDependency(std::string id1, std::string id2, IDependency::Type type,
                                                  IDependency::Reason reason, std::vector<size_t> basedOn,
                                                  IDependency::UsefulnessLevel usefulness) {
  std::string index1;
  std::string index2;
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

unsigned int DependenciesBank::addCirclesDependency(std::string id1, std::string id2, IDependency::Type type,
                                                    IDependency::Reason reason, std::vector<size_t> basedOn,
                                                    IDependency::UsefulnessLevel usefulness) {
  std::string index1;
  std::string index2;
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

unsigned int DependenciesBank::addPointPairsDependency(std::string pair1End1, std::string pair1End2,
                                                       std::string pair2End1, std::string pair2End2,
                                                       IDependency::Type type, IDependency::Reason reason,
                                                       std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness) {
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

unsigned int DependenciesBank::addAnglesDependency(std::string angle1Point1Id, std::string angle1VertexId,
                                                   std::string angle1Point2Id, AngleModel::AngleType angle1Type,
                                                   std::string angle2Point1Id, std::string angle2VertexId,
                                                   std::string angle2Point2Id, AngleModel::AngleType angle2Type,
                                                   IDependency::Type type, IDependency::Reason reason,
                                                   std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(angle1Point1Id, angle1VertexId, angle1Point2Id);
  changeAngleEnds(angle2Point1Id, angle2VertexId, angle2Point2Id);

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

unsigned int DependenciesBank::addPolygonsDependency(std::vector<std::string> polygon1VerticesIds,
                                                     std::vector<std::string> polygon2VerticesIds, IDependency::Type type,
                                                     IDependency::Reason reason, std::vector<size_t> basedOn,
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

unsigned int DependenciesBank::addLineCircleDependency(std::string lineId, std::string circleId, IDependency::Type type,
                                                       IDependency::Reason reason, std::vector<size_t> basedOn,
                                                       IDependency::UsefulnessLevel usefulness) {
  const std::shared_ptr<IdentifiersDependency> dependency = std::make_shared<IdentifiersDependency>(
      IdentifierModel(std::move(lineId)), IdentifierModel(std::move(circleId)), dependencyIdCounter, IDependency::Category::OF_LINE_AND_CIRCLE, type,
      reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinePointPairDependency(std::string lineId, std::string pairEnd1Id, std::string pairEnd2Id,
                                                          IDependency::Type type, IDependency::Reason reason,
                                                          std::vector<size_t> basedOn,
                                                          IDependency::UsefulnessLevel usefulness) {

  const std::shared_ptr<IdentifierAndPointsPairDependency> dependency = std::make_shared<IdentifierAndPointsPairDependency>(
      IdentifierModel(std::move(lineId)), PointsPairModel(std::move(pairEnd1Id), std::move(pairEnd2Id)), dependencyIdCounter,
      IDependency::Category::OF_LINE_AND_POINTS_PAIR, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineAngleDependency(std::string lineId, std::string anglePoint1Id, std::string angleVertexId,
                                                      std::string anglePoint2Id, AngleModel::AngleType angleType,
                                                      IDependency::Type type, IDependency::Reason reason,
                                                      std::vector<size_t> basedOn,
                                                      IDependency::UsefulnessLevel usefulness) {
  changeAngleEnds(anglePoint1Id, angleVertexId, anglePoint2Id);

  const std::shared_ptr<IdentifierAndAngleDependency> dependency = std::make_shared<IdentifierAndAngleDependency>(
      IdentifierModel(std::move(lineId)),
      AngleModel(std::move(angleVertexId), std::move(anglePoint1Id), std::move(anglePoint2Id), angleType),
      dependencyIdCounter, IDependency::Category::OF_LINE_AND_ANGLE, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclePolygonDependency(std::string circleId, std::vector<std::string> polygonVerticesIds,
                                                          IDependency::Type type, IDependency::Reason reason,
                                                          std::vector<size_t> basedOn,
                                                          IDependency::UsefulnessLevel usefulness) {

  const std::shared_ptr<IdentifierAndPolygonDependency> dependency = std::make_shared<IdentifierAndPolygonDependency>(
      IdentifierModel(std::move(circleId)), PolygonModel(std::move(polygonVerticesIds)), dependencyIdCounter,
      IDependency::Category::OF_CIRCLE_AND_POLYGON, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonTypeDependency(std::vector<std::string> polygonVerticesIds,
                                                        PolygonModel::PolygonType polygonType, IDependency::Type type,
                                                        IDependency::Reason reason, std::vector<size_t> basedOn,
                                                        IDependency::UsefulnessLevel usefulness) {
  const std::shared_ptr<IdentifierAndPolygonDependency> dependency = std::make_shared<IdentifierAndPolygonDependency>(
      IdentifierModel(std::to_string(static_cast<size_t>(polygonType))), PolygonModel(std::move(polygonVerticesIds)), dependencyIdCounter,
      IDependency::Category::POLYGON_TYPE, type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointsPairsPairPointsPairDependency(std::string arm1End1, std::string arm1End2,
                                                                      std::string arm2End1, std::string arm2End2,
                                                                      std::string segmentEnd1, std::string segmentEnd2,
                                                                      IDependency::Type type, IDependency::Reason reason,
                                                                      std::vector<size_t> basedOn,
                                                                      IDependency::UsefulnessLevel usefulness) {

  const std::shared_ptr<PairOfPointsPairPointsPairDependency> dependency = std::make_shared<PairOfPointsPairPointsPairDependency>(
      ModelsPairModel<PointsPairModel>(PointsPairModel(std::move(arm1End1), std::move(arm1End2)), PointsPairModel(std::move(arm2End1), std::move(arm2End2))),
      PointsPairModel(std::move(segmentEnd1), std::move(segmentEnd2)), dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR,
      type, reason, std::move(basedOn), usefulness);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addEquation(const symbolicAlgebra::Expression& leftSide,
                                           const symbolicAlgebra::Expression& rightSide, IDependency::Reason reason,
                                           std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness) {
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
  for (std::vector<size_t>::const_iterator iter = formulas.cbegin(); iter != formulas.cend(); ++iter) {
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

  std::map<IDependency::Type, std::vector<size_t>>::iterator mIt;
  std::vector<size_t>::iterator vIt;
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

  std::map<std::string, std::vector<size_t>>::iterator iter;
  for (iter = variableToIndexes.begin(); iter != variableToIndexes.end(); ++iter) {
    result.push_back({
        {"name", iter->first},
        {"ids", json(iter->second)},
    });
  }

  return result;
}

std::string DependenciesBank::getLengthName(const PointModel& point1, const PointModel& point2) {
  const std::string& point1Name = point1.getName();
  const std::string& point2Name = point2.getName();

  if(point1Name.length() < point2Name.length()) {
    return "|" + point1Name + point2Name + "|";
  }

  if(point1Name.length() > point2Name.length()) {
    return "|" + point2Name + point1Name + "|";
  }

  if(point1Name < point2Name) {
    return "|" + point1Name + point2Name + "|";
  }

  return "|" + point2Name + point1Name + "|";
}

std::string DependenciesBank::getAngleName(const PointModel& point1, const PointModel& point2, const PointModel& point3, bool angleIsConvex) {
  const std::string& point1Name = point1.getName();
  const std::string& point2Name = point2.getName();
  const std::string& point3Name = point3.getName();

  if(point1Name.length() < point3Name.length()) {
    if(angleIsConvex) {
      return "|>" + point1Name + point2Name + point3Name+ "|";
    }

    return "|<" + point1Name + point2Name + point3Name+ "|";
  }

  if(point1Name.length() > point3Name.length()) {
    if(angleIsConvex) {
      return "|>" + point3Name + point2Name + point1Name+ "|";
    }

    return "|<" + point3Name + point2Name + point1Name+ "|";
  }

  if(point1Name < point3Name) {
    if(angleIsConvex) {
      return "|>" + point1Name + point2Name + point3Name+ "|";
    }

    return "|<" + point1Name + point2Name + point3Name+ "|";
  }

  if(angleIsConvex) {
    return "|>" + point3Name + point2Name + point1Name+ "|";
  }

  return "|<" + point3Name + point2Name + point1Name+ "|";
}

void DependenciesBank::changeAngleEnds(std::string& point1Id, std::string& vertexId, std::string& point2Id) {
  try {
    const std::string arm1Id = shapesBank->getLineIdThrowTwoPoints(point1Id, vertexId);
    const std::vector<std::string>& arm1Points = shapesBank->getLine(arm1Id).getIncludedPoints();

    const unsigned int indexOfPoint2OnArm1 =
        std::distance(arm1Points.begin(), std::find(arm1Points.begin(), arm1Points.end(), vertexId));
    const unsigned int indexOfId1 = std::distance(arm1Points.begin(), std::find(arm1Points.begin(), arm1Points.end(), point1Id));

    if (indexOfId1 > indexOfPoint2OnArm1) {
      point1Id = arm1Points.back();
    }
    else {
      point1Id = arm1Points.front();
    }
  } catch (std::invalid_argument const& exception) {}

  try {
    const std::string arm2Id = shapesBank->getLineIdThrowTwoPoints(vertexId, point2Id);
    const std::vector<std::string>& arm2Points =shapesBank->getLine(arm2Id).getIncludedPoints();
    const unsigned int indexOfVertexOnArm2 =
        std::distance(arm2Points.begin(), std::find(arm2Points.begin(), arm2Points.end(), vertexId));
    const unsigned int indexOfId2 = std::distance(arm2Points.begin(), std::find(arm2Points.begin(), arm2Points.end(), point2Id));

    if (indexOfId2 > indexOfVertexOnArm2) {
      point2Id = arm2Points.back();
    }
    else {
      point2Id = arm2Points.front();
    }
  } catch (std::invalid_argument const& exception) {}
}

void DependenciesBank::add(std::shared_ptr<IDependency> dependencyModel) {
  dependenciesMap.at(dependencyModel->getType()).emplace_back(dependencyIdCounter);
  dependenciesVector.emplace_back(std::move(dependencyModel));
  dependencyIdCounter++;
}
}  // namespace expertBackground