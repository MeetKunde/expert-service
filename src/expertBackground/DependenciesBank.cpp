#include "DependenciesBank.h"

namespace expertBackground {
DependenciesBank::DependenciesBank() : dependencyIdCounter{0}, shapesBank{nullptr} {
  for (auto iter = IDependency::TypeBegin; iter != IDependency::TypeEnd; ++iter) {
    dependenciesMap.insert({iter, {}});
  }
}

DependenciesBank::DependenciesBank(ShapesBank* shapesBank) : dependencyIdCounter{0}, shapesBank{shapesBank} {
  for (auto iter = IDependency::TypeBegin; iter != IDependency::TypeEnd; ++iter) {
    dependenciesMap.insert({iter, {}});
  }

  const std::vector<PointModel> points = shapesBank->getPointsVector();

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos <= point1Pos; point2Pos++) {
      const std::pair<std::string, std::vector<std::string>> lengthName =
          getLengthName(points.at(point1Pos), points.at(point2Pos));

      variableToIndexes.insert({lengthName.first,
                                {shapesBank->getPointPositionInVector(lengthName.second[0]),
                                 shapesBank->getPointPositionInVector(lengthName.second[1])}});
      formulasIncludingVariable.insert({lengthName.first, {}});
    }
  }

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < shapesBank->getPointsNumber(); point2Pos++) {
      for (size_t point3Pos = 0; point3Pos <= point1Pos; point3Pos++) {
        const std::pair<std::string, std::vector<std::string>> convexMeasureName =
            getAngleName(points.at(point1Pos), points.at(point2Pos), points.at(point3Pos), true);

        variableToIndexes.insert(
            {convexMeasureName.first,
             {shapesBank->getPointPositionInVector(convexMeasureName.second[0]),
              shapesBank->getPointPositionInVector(convexMeasureName.second[1]),
              shapesBank->getPointPositionInVector(convexMeasureName.second[2]), static_cast<size_t>(true)}});
        formulasIncludingVariable.insert({convexMeasureName.first, {}});

        const std::pair<std::string, std::vector<std::string>> concaveMeasureName =
            getAngleName(points.at(point1Pos), points.at(point2Pos), points.at(point3Pos), false);

        variableToIndexes.insert(
            {concaveMeasureName.first,
             {shapesBank->getPointPositionInVector(concaveMeasureName.second[0]),
              shapesBank->getPointPositionInVector(concaveMeasureName.second[1]),
              shapesBank->getPointPositionInVector(concaveMeasureName.second[2]), static_cast<size_t>(false)}});
        formulasIncludingVariable.insert({concaveMeasureName.first, {}});
      }
    }
  }

  for (size_t pointPos = 0; pointPos < shapesBank->getPointsNumber(); pointPos++) {
    addLength(points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ZERO_ANGLE_VALUE),
              IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::Importance::LOW);

    addConvexAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(),
                   Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::Importance::LOW);
    addConcaveAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(),
                    Int(MathHelper::ROUND_ANGLE_VALUE), IDependency::Reason::POINTS_ARE_THE_SAME, {},
                    IDependency::Importance::LOW);
  }

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < shapesBank->getPointsNumber(); point2Pos++) {
      if (point1Pos != point2Pos) {
        addConvexAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                       Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME, {},
                       IDependency::Importance::LOW);
        addConcaveAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                        Int(MathHelper::ROUND_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME, {},
                        IDependency::Importance::LOW);
      }
    }
  }
}

unsigned int DependenciesBank::addEquation(const symbolicAlgebra::Expression& leftSide,
                                           const symbolicAlgebra::Expression& rightSide, IDependency::Reason reason,
                                           std::vector<size_t> basedOn, IDependency::Importance importance) {

  auto lhsExpression = ExpressionModel{leftSide};
  auto rhsExpression = ExpressionModel{rightSide};
  if (rhsExpression < lhsExpression) {
    std::swap(lhsExpression, rhsExpression);
  }

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      dependencyIdCounter, IDependency::Category::FORMULA, IDependency::Type::EQUATION, reason, std::move(basedOn), importance,
      std::move(lhsExpression), std::move(rhsExpression));

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
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLength(const std::string& point1Id, const std::string& point2Id,
                                         const symbolicAlgebra::Expression& length, IDependency::Reason reason,
                                         std::vector<size_t> basedOn, IDependency::Importance importance) {
  const std::pair<std::string, std::vector<std::string>> name =
      getLengthName(shapesBank->getPoint(point1Id), shapesBank->getPoint(point2Id));

  auto lhsExpression = ExpressionModel{Var(name.first)};
  auto rhsExpression = ExpressionModel{length};
  if (rhsExpression < lhsExpression) {
    std::swap(lhsExpression, rhsExpression);
  }

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      dependencyIdCounter, IDependency::Category::FORMULA, IDependency::Type::SEGMENT_LENGTH, reason, std::move(basedOn),
      importance, std::move(lhsExpression), std::move(rhsExpression));

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  names.insert(name.first);
  length.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addConvexAngle(const std::string& point1Id, const std::string& vertexId,
                                              const std::string& point2Id, const symbolicAlgebra::Expression& value,
                                              IDependency::Reason reason, std::vector<size_t> basedOn,
                                              IDependency::Importance importance) {

  const std::vector<std::string> angle = changeAngleEnds(point1Id, vertexId, point2Id);
  const std::pair<std::string, std::vector<std::string>> name =
      getAngleName(shapesBank->getPoint(angle[0]), shapesBank->getPoint(angle[1]), shapesBank->getPoint(angle[2]), true);

  auto lhsExpression = ExpressionModel{Var(name.first)};
  auto rhsExpression = ExpressionModel{value};
  if (rhsExpression < lhsExpression) {
    std::swap(lhsExpression, rhsExpression);
  }

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      dependencyIdCounter, IDependency::Category::FORMULA, IDependency::Type::ANGLE_MEASURE, reason, std::move(basedOn),
      importance, std::move(lhsExpression), std::move(rhsExpression));

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  names.insert(name.first);
  value.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addConcaveAngle(const std::string& point1Id, const std::string& vertexId,
                                               const std::string& point2Id, const symbolicAlgebra::Expression& value,
                                               IDependency::Reason reason, std::vector<size_t> basedOn,
                                               IDependency::Importance importance) {

  const std::vector<std::string> angle = changeAngleEnds(point1Id, vertexId, point2Id);
  const std::pair<std::string, std::vector<std::string>> name =
      getAngleName(shapesBank->getPoint(angle[0]), shapesBank->getPoint(angle[1]), shapesBank->getPoint(angle[2]), false);

  auto lhsExpression = ExpressionModel{Var(name.first)};
  auto rhsExpression = ExpressionModel{value};
  if (rhsExpression < lhsExpression) {
    std::swap(lhsExpression, rhsExpression);
  }

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      dependencyIdCounter, IDependency::Category::FORMULA, IDependency::Type::ANGLE_MEASURE, reason, std::move(basedOn),
      importance, std::move(lhsExpression), std::move(rhsExpression));

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  std::set<std::string> names;
  names.insert(name.first);
  value.getIncludedVariables(names);

  std::set<std::string>::iterator iter;
  for (iter = names.begin(); iter != names.end(); ++iter) {
    if (formulasIncludingVariable.find(*iter) == formulasIncludingVariable.end()) {
      formulasIncludingVariable.insert({*iter, {dependencyIdCounter}});
    }
    else {
      formulasIncludingVariable.at(*iter).insert(dependencyIdCounter);
    }
  }

  formulas.push_back(dependencyIdCounter);
  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinesDependency(const std::string& id1, const std::string& id2, bool commutative,
                                                  LinesDependencies type, IDependency::Reason reason, std::vector<size_t> basedOn,
                                                  IDependency::Importance importance) {

  std::string index1{id1};
  std::string index2{id2};
  if (id1 > id2 && commutative) {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<LinesDependency> dependency = std::make_shared<LinesDependency>(
      dependencyIdCounter, IDependency::Category::OF_LINES, static_cast<IDependency::Type>(type), reason, std::move(basedOn),
      importance, index1, index2);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclesDependency(const std::string& id1, const std::string& id2, bool commutative,
                                                    CirclesDependencies type, IDependency::Reason reason,
                                                    std::vector<size_t> basedOn, IDependency::Importance importance) {

  std::string index1{id1};
  std::string index2{id2};
  if (id1 > id2 && commutative) {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<CirclesDependency> dependency = std::make_shared<CirclesDependency>(
      dependencyIdCounter, IDependency::Category::OF_CIRCLES, static_cast<IDependency::Type>(type), reason, std::move(basedOn),
      importance, index1, index2);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointsPairsDependency(const std::string& pair1End1, const std::string& pair1End2,
                                                        const std::string& pair2End1, const std::string& pair2End2,
                                                        bool commutative, PointsPairsDependencies type,
                                                        IDependency::Reason reason, std::vector<size_t> basedOn,
                                                        IDependency::Importance importance) {
  auto pair1 = PointsPairModel{pair1End1, pair1End2};
  auto pair2 = PointsPairModel{pair2End1, pair2End2};
  if (pair1 > pair2 && commutative) {
    std::swap(pair1, pair2);
  }

  const std::shared_ptr<PointsPairsDependency> dependency = std::make_shared<PointsPairsDependency>(
      dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, pair1, pair2);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addAnglesDependency(const std::string& angle1Point1Id, const std::string& angle1VertexId,
                                                   const std::string& angle1Point2Id, AngleModel::Type angle1Type,
                                                   const std::string& angle2Point1Id, const std::string& angle2VertexId,
                                                   const std::string& angle2Point2Id, AngleModel::Type angle2Type,
                                                   bool commutative, AnglesDependencies type, IDependency::Reason reason,
                                                   std::vector<size_t> basedOn, IDependency::Importance importance) {

  const std::vector<std::string> angle1 = changeAngleEnds(angle1Point1Id, angle1VertexId, angle1Point2Id);
  const std::vector<std::string> angle2 = changeAngleEnds(angle2Point1Id, angle2VertexId, angle2Point2Id);

  auto angleModel1 = AngleModel{angle1[1], angle1[0], angle1[2], angle1Type};
  auto angleModel2 = AngleModel{angle2[1], angle2[0], angle2[2], angle2Type};
  if (angleModel1 > angleModel2 && commutative) {
    std::swap(angleModel1, angleModel2);
  }

  const std::shared_ptr<AnglesDependency> dependency = std::make_shared<AnglesDependency>(
      dependencyIdCounter, IDependency::Category::OF_ANGLES, static_cast<IDependency::Type>(type), reason, std::move(basedOn),
      importance, angleModel1, angleModel2);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addAnglesDependency(const std::string& angle1Arm1Point1Id, const std::string& angle1Arm1Point2Id,
                                                   const std::string& angle1Arm2Point1Id, const std::string& angle1Arm2Point2Id,
                                                   AngleModel::Type angle1Type, const std::string& angle2Arm1Point1Id,
                                                   const std::string& angle2Arm1Point2Id, const std::string& angle2Arm2Point1Id,
                                                   const std::string& angle2Arm2Point2Id, AngleModel::Type angle2Type,
                                                   bool commutative, AnglesDependencies type, IDependency::Reason reason,
                                                   std::vector<size_t> basedOn, IDependency::Importance importance) {

  auto angleModel1 = AngleModel{angle1Arm1Point1Id, angle1Arm1Point2Id, angle1Arm2Point1Id, angle1Arm2Point2Id, angle1Type};
  auto angleModel2 = AngleModel{angle2Arm1Point1Id, angle2Arm1Point2Id, angle2Arm2Point1Id, angle2Arm2Point2Id, angle2Type};
  if (angleModel1 > angleModel2 && commutative) {
    std::swap(angleModel1, angleModel2);
  }

  const std::shared_ptr<AnglesDependency> dependency = std::make_shared<AnglesDependency>(
      dependencyIdCounter, IDependency::Category::OF_ANGLES, static_cast<IDependency::Type>(type), reason, std::move(basedOn),
      importance, angleModel1, angleModel2);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonsDependency(const std::vector<std::string>& polygon1VerticesIds, bool fixedPoints1Order,
                                                     const std::vector<std::string>& polygon2VerticesIds, bool fixedPoints2Order,
                                                     bool commutative, PolygonsDependencies type, IDependency::Reason reason,
                                                     std::vector<size_t> basedOn, IDependency::Importance importance) {

  auto polygon1 = PolygonModel{PolygonModel::Type::UNKNOWN, fixedPoints1Order, polygon1VerticesIds};
  auto polygon2 = PolygonModel{PolygonModel::Type::UNKNOWN, fixedPoints2Order, polygon2VerticesIds};
  if (polygon1 > polygon2 && commutative) {
    std::swap(polygon1, polygon2);
  }

  const std::shared_ptr<PolygonsDependency> dependency = std::make_shared<PolygonsDependency>(
      dependencyIdCounter, IDependency::Category::OF_POLYGONS, static_cast<IDependency::Type>(type), reason, std::move(basedOn),
      importance, std::move(polygon1), std::move(polygon2));

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineCircleDependency(const std::string& lineId, const std::string& circleId,
                                                       LineCircleDependencies type, IDependency::Reason reason,
                                                       std::vector<size_t> basedOn, IDependency::Importance importance) {

  const std::shared_ptr<LineCircleDependency> dependency = std::make_shared<LineCircleDependency>(
      dependencyIdCounter, IDependency::Category::OF_LINE_AND_CIRCLE, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, lineId, circleId);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinePointsPairDependency(const std::string& lineId, const std::string& pairEnd1Id,
                                                           const std::string& pairEnd2Id, LinePointsPairDependencies type,
                                                           IDependency::Reason reason, std::vector<size_t> basedOn,
                                                           IDependency::Importance importance) {

  const std::shared_ptr<LinePointsPairDependency> dependency = std::make_shared<LinePointsPairDependency>(
      dependencyIdCounter, IDependency::Category::OF_LINE_AND_POINTS_PAIR, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, lineId, PointsPairModel{pairEnd1Id, pairEnd2Id});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineAngleDependency(const std::string& lineId, const std::string& anglePoint1Id,
                                                      const std::string& angleVertexId, const std::string& anglePoint2Id,
                                                      AngleModel::Type angleType, LineAngleDependencies type,
                                                      IDependency::Reason reason, std::vector<size_t> basedOn,
                                                      IDependency::Importance importance) {

  const std::vector<std::string> angle = changeAngleEnds(anglePoint1Id, angleVertexId, anglePoint2Id);

  const std::shared_ptr<LineAngleDependency> dependency = std::make_shared<LineAngleDependency>(
      dependencyIdCounter, IDependency::Category::OF_LINE_AND_ANGLE, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, lineId, AngleModel{angle[1], angle[0], angle[2], angleType});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclePolygonDependency(const std::string& circleId,
                                                          const std::vector<std::string>& polygonVerticesIds,
                                                          CirclePolygonDependencies type, IDependency::Reason reason,
                                                          std::vector<size_t> basedOn, IDependency::Importance importance) {

  const std::shared_ptr<CirclePolygonDependency> dependency = std::make_shared<CirclePolygonDependency>(
      dependencyIdCounter, IDependency::Category::OF_CIRCLE_AND_POLYGON, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, circleId, PolygonModel{PolygonModel::Type::UNKNOWN, false, polygonVerticesIds});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonTypeDependency(const std::vector<std::string>& polygonVerticesIds,
                                                        PolygonModel::Type polygonType, PolygonTypeDependencies type,
                                                        IDependency::Reason reason, std::vector<size_t> basedOn,
                                                        IDependency::Importance importance) {

  const std::shared_ptr<PolygonTypeDependency> dependency = std::make_shared<PolygonTypeDependency>(
      dependencyIdCounter, IDependency::Category::POLYGON_TYPE, static_cast<IDependency::Type>(type), reason, std::move(basedOn),
      importance, PolygonModel{polygonType, false, polygonVerticesIds});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addTriplePointsPairDependency(const std::string& arm1End1, const std::string& arm1End2,
                                                             const std::string& arm2End1, const std::string& arm2End2,
                                                             const std::string& segmentEnd1, const std::string& segmentEnd2,
                                                             TriplePointsPairDependencies type, IDependency::Reason reason,
                                                             std::vector<size_t> basedOn, IDependency::Importance importance) {

  const std::shared_ptr<TriplePointsPairDependency> dependency = std::make_shared<TriplePointsPairDependency>(
      dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR, static_cast<IDependency::Type>(type),
      reason, std::move(basedOn), importance, PointsPairModel{arm1End1, arm1End2}, PointsPairModel{arm2End1, arm2End2},
      PointsPairModel{segmentEnd1, segmentEnd2});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonPointsPairDependency(const std::vector<std::string>& polygonVerticesIds,
                                                              bool fixedPointsOrder, const std::string& pairEnd1,
                                                              const std::string& pairEnd2, PolygonPointsPairDependencies type,
                                                              IDependency::Reason reason, std::vector<size_t> basedOn,
                                                              IDependency::Importance importance) {

  const std::shared_ptr<PolygonPointsPairDependency> dependency = std::make_shared<PolygonPointsPairDependency>(
      dependencyIdCounter, IDependency::Category::OF_POLYGON_AND_POINTS_PAIRS, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, PolygonModel{PolygonModel::Type::UNKNOWN, fixedPointsOrder, polygonVerticesIds},
      PointsPairModel{pairEnd1, pairEnd2});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonExpressionDependency(const std::vector<std::string>& polygonVerticesIds,
                                                              bool fixedPointsOrder,
                                                              const symbolicAlgebra::Expression& expression,
                                                              PolygonExpressionDependencies type, IDependency::Reason reason,
                                                              std::vector<size_t> basedOn, IDependency::Importance importance) {

  const std::shared_ptr<PolygonExpressionDependency> dependency = std::make_shared<PolygonExpressionDependency>(
      dependencyIdCounter, IDependency::Category::OF_POLYGON_AND_EXPRESSION, static_cast<IDependency::Type>(type), reason,
      std::move(basedOn), importance, PolygonModel{PolygonModel::Type::UNKNOWN, fixedPointsOrder, polygonVerticesIds},
      ExpressionModel{expression});

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  addDependency(dependency);
  return 1;
}

json DependenciesBank::getDependenciesJson() const {
  json result;

  std::map<IDependency::Type, std::vector<size_t>>::const_iterator mIt;
  std::vector<size_t>::const_iterator vIt;
  for (mIt = dependenciesMap.begin(); mIt != dependenciesMap.end(); ++mIt) {
    std::vector<json> dependencies;
    for (vIt = mIt->second.begin(); vIt != mIt->second.end(); ++vIt) {
      dependencies.push_back(dependenciesVector.at((*vIt))->getJson());
    }

    result.push_back({
        {"type", mIt->first},
        {"dependencies", json(dependencies)},
    });
  }

  return result;
}

json DependenciesBank::getVariablesIndexesJson() const {
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

std::pair<std::string, std::vector<std::string>> DependenciesBank::getLengthName(const PointModel& point1,
                                                                                 const PointModel& point2) {
  const std::string& point1Name = point1.getName();
  const std::string& point2Name = point2.getName();

  if (point1Name.length() < point2Name.length()) {
    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId()};
    return std::make_pair("|" + point1Name + point2Name + "|", pointsOrder);
  }

  if (point1Name.length() > point2Name.length()) {
    const std::vector<std::string> pointsOrder = {point2.getId(), point1.getId()};
    return std::make_pair("|" + point2Name + point1Name + "|", pointsOrder);
  }

  if (point1Name < point2Name) {
    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId()};
    return std::make_pair("|" + point1Name + point2Name + "|", pointsOrder);
  }

  const std::vector<std::string> pointsOrder = {point2.getId(), point1.getId()};
  return std::make_pair("|" + point2Name + point1Name + "|", pointsOrder);
}

std::pair<std::string, std::vector<std::string>> DependenciesBank::getAngleName(const PointModel& point1,
                                                                                const PointModel& point2,
                                                                                const PointModel& point3, bool angleIsConvex) {
  const std::string& point1Name = point1.getName();
  const std::string& point2Name = point2.getName();
  const std::string& point3Name = point3.getName();

  if (point1Name.length() < point3Name.length()) {
    if (angleIsConvex) {
      const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
      return std::make_pair("|>" + point1Name + point2Name + point3Name + "|", pointsOrder);
    }

    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
    return std::make_pair("|<" + point1Name + point2Name + point3Name + "|", pointsOrder);
  }

  if (point1Name.length() > point3Name.length()) {
    if (angleIsConvex) {
      const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
      return std::make_pair("|>" + point3Name + point2Name + point1Name + "|", pointsOrder);
    }

    const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
    return std::make_pair("|<" + point3Name + point2Name + point1Name + "|", pointsOrder);
  }

  if (point1Name < point3Name) {
    if (angleIsConvex) {
      const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
      return std::make_pair("|>" + point1Name + point2Name + point3Name + "|", pointsOrder);
    }

    const std::vector<std::string> pointsOrder = {point1.getId(), point2.getId(), point3.getId()};
    return std::make_pair("|<" + point1Name + point2Name + point3Name + "|", pointsOrder);
  }

  if (angleIsConvex) {
    const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
    return std::make_pair("|>" + point3Name + point2Name + point1Name + "|", pointsOrder);
  }

  const std::vector<std::string> pointsOrder = {point3.getId(), point2.getId(), point1.getId()};
  return std::make_pair("|<" + point3Name + point2Name + point1Name + "|", pointsOrder);
}

std::vector<std::string> DependenciesBank::changeAngleEnds(const std::string& point1Id, const std::string& vertexId,
                                                           const std::string& point2Id) {
  if (shapesBank == nullptr) {
    return std::vector<std::string>{point1Id, vertexId, point2Id};
  }

  std::string point1IdTmp{point1Id};
  std::string point2IdTmp{point2Id};

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

  return std::vector<std::string>{point1IdTmp, vertexId, point2IdTmp};
}
}  // namespace expertBackground