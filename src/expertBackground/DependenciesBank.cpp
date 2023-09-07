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
      const std::pair<std::string, std::vector<std::string>> lengthName =
          getLengthName(points.at(point1Pos), points.at(point2Pos));

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

  for (size_t pointPos = 0; pointPos < shapesBank->getPointsNumber(); pointPos++) {
    addLength(points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ZERO_ANGLE_VALUE),
              IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::ImportanceLevel::LOW);

    addConvexAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ZERO_ANGLE_VALUE),
                   IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::ImportanceLevel::LOW);
    addConcaveAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(), Int(MathHelper::ROUND_ANGLE_VALUE),
                    IDependency::Reason::POINTS_ARE_THE_SAME, {}, IDependency::ImportanceLevel::LOW);
  }

  for (size_t point1Pos = 0; point1Pos < shapesBank->getPointsNumber(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < shapesBank->getPointsNumber(); point2Pos++) {
      if (point1Pos != point2Pos) {
        addConvexAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                       Int(MathHelper::ZERO_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME, {}, IDependency::ImportanceLevel::LOW);
        addConcaveAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                        Int(MathHelper::ROUND_ANGLE_VALUE), IDependency::Reason::ARMS_ARE_THE_SAME, {}, IDependency::ImportanceLevel::LOW);
      }
    }
  }
}

unsigned int DependenciesBank::addEquation(const symbolicAlgebra::Expression& leftSide,
                                           const symbolicAlgebra::Expression& rightSide, IDependency::Reason reason,
                                           std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(rightSide), ExpressionModel(leftSide), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::EQUATION, reason, std::move(basedOn), importanceLevel);

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
  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLength(const std::string& point1Id, const std::string& point2Id, const symbolicAlgebra::Expression& length,
                                         IDependency::Reason reason, std::vector<size_t> basedOn,
                                         IDependency::ImportanceLevel importanceLevel) {
  const std::pair<std::string, std::vector<std::string>> name =
      getLengthName(shapesBank->getPoint(point1Id), shapesBank->getPoint(point2Id));

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(Var(name.first)), ExpressionModel(length), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::SEGMENT_LENGTH, reason, std::move(basedOn), importanceLevel);

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
  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addConvexAngle(const std::string& point1Id, const std::string& vertexId, const std::string& point2Id,
                                              const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                                              std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel) {

  std::string point1IdTmp{point1Id};
  std::string vertexIdTmp{vertexId};
  std::string point2IdTmp{point2Id};
  changeAngleEnds(point1IdTmp, vertexIdTmp, point2IdTmp);
  const std::pair<std::string, std::vector<std::string>> name =
      getAngleName(shapesBank->getPoint(point1IdTmp), shapesBank->getPoint(vertexIdTmp), shapesBank->getPoint(point2IdTmp), true);

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(Var(name.first)), ExpressionModel(value), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::ANGLE_MEASURE, reason, std::move(basedOn), importanceLevel);

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
  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addConcaveAngle(const std::string& point1Id, const std::string& vertexId, const std::string& point2Id,
                                               const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                                               std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel) {
  std::string point1IdTmp{point1Id};
  std::string vertexIdTmp{vertexId};
  std::string point2IdTmp{point2Id};
  changeAngleEnds(point1IdTmp, vertexIdTmp, point2IdTmp);
  const std::pair<std::string, std::vector<std::string>> name =
      getAngleName(shapesBank->getPoint(point1IdTmp), shapesBank->getPoint(vertexIdTmp), shapesBank->getPoint(point2IdTmp), false);

  const std::shared_ptr<EquationDependency> dependency = std::make_shared<EquationDependency>(
      ExpressionModel(Var(name.first)), ExpressionModel(value), dependencyIdCounter, IDependency::Category::FORMULA,
      IDependency::Type::ANGLE_MEASURE, reason, std::move(basedOn), importanceLevel);

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
  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinesDependency(const std::string& id1, const std::string& id2, IDependency::Type type,
                                                  IDependency::Reason reason, std::vector<size_t> basedOn,
                                                  IDependency::ImportanceLevel importanceLevel) {

  std::string index1{id1};
  std::string index2{id2};
  if (id1 < id2) {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<LinesDependency> dependency =
      std::make_shared<LinesDependency>(IdentifierModel(index1), IdentifierModel(index2), dependencyIdCounter,
                                        IDependency::Category::OF_LINES, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclesDependency(const std::string& id1, const std::string& id2, IDependency::Type type,
                                                    IDependency::Reason reason, std::vector<size_t> basedOn,
                                                    IDependency::ImportanceLevel importanceLevel) {

  std::string index1{id1};
  std::string index2{id2};
  if (id1 < id2) {
    index1 = id2;
    index2 = id1;
  }

  const std::shared_ptr<CirclesDependency> dependency =
      std::make_shared<CirclesDependency>(IdentifierModel(index1), IdentifierModel(index2), dependencyIdCounter,
                                          IDependency::Category::OF_CIRCLES, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointPairsDependency(const std::string& pair1End1, const std::string& pair1End2,
                                                       const std::string& pair2End1, const std::string& pair2End2,
                                                       IDependency::Type type, IDependency::Reason reason,
                                                       std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PointsPairsDependency> dependency = std::make_shared<PointsPairsDependency>(
      PointsPairModel{pair1End1, pair1End2}, PointsPairModel{pair2End1, pair2End2},
      dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addAnglesDependency(const std::string& angle1Point1Id, const std::string& angle1VertexId,
                                                   const std::string& angle1Point2Id, AngleModel::AngleType angle1Type,
                                                   const std::string& angle2Point1Id, const std::string& angle2VertexId,
                                                   const std::string& angle2Point2Id, AngleModel::AngleType angle2Type,
                                                   IDependency::Type type, IDependency::Reason reason,
                                                   std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel) {

  std::string angle1Point1IdTmp{angle1Point1Id};
  std::string angle1VertexIdTmp{angle1VertexId};
  std::string angle1Point2IdTmp{angle1Point2Id};

  std::string angle2Point1IdTmp{angle2Point1Id};
  std::string angle2VertexIdTmp{angle2VertexId};
  std::string angle2Point2IdTmp{angle2Point2Id};

  changeAngleEnds(angle1Point1IdTmp, angle1VertexIdTmp, angle1Point2IdTmp);
  changeAngleEnds(angle2Point1IdTmp, angle2VertexIdTmp, angle2Point2IdTmp);

  const std::shared_ptr<AnglesDependency> dependency = std::make_shared<AnglesDependency>(
      AngleModel{angle1VertexIdTmp, angle1Point1IdTmp, angle1Point2IdTmp, angle1Type},
      AngleModel{angle2VertexIdTmp, angle2Point1IdTmp, angle2Point2IdTmp, angle2Type},
      dependencyIdCounter, IDependency::Category::OF_ANGLES, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonsDependency(const std::vector<std::string>& polygon1VerticesIds,
                                                     const std::vector<std::string>& polygon2VerticesIds, IDependency::Type type,
                                                     IDependency::Reason reason, std::vector<size_t> basedOn,
                                                     IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PolygonsDependency> dependency = std::make_shared<PolygonsDependency>(
      PolygonModel(polygon1VerticesIds), PolygonModel(polygon2VerticesIds), dependencyIdCounter,
      IDependency::Category::OF_POLYGONS, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineCircleDependency(const std::string& lineId, const std::string& circleId, IDependency::Type type,
                                                       IDependency::Reason reason, std::vector<size_t> basedOn,
                                                       IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<LineCircleDependency> dependency = std::make_shared<LineCircleDependency>(
      IdentifierModel(lineId), IdentifierModel(circleId), dependencyIdCounter, IDependency::Category::OF_LINE_AND_CIRCLE, type,
      reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLinePointPairDependency(const std::string& lineId, const std::string& pairEnd1Id, const std::string& pairEnd2Id,
                                                          IDependency::Type type, IDependency::Reason reason,
                                                          std::vector<size_t> basedOn,
                                                          IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<LinePointPairDependency> dependency = std::make_shared<LinePointPairDependency>(
      IdentifierModel{lineId}, PointsPairModel{pairEnd1Id, pairEnd2Id}, dependencyIdCounter,
      IDependency::Category::OF_LINE_AND_POINTS_PAIR, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addLineAngleDependency(const std::string& lineId, const std::string& anglePoint1Id, const std::string& angleVertexId,
                                                      const std::string& anglePoint2Id, AngleModel::AngleType angleType,
                                                      IDependency::Type type, IDependency::Reason reason,
                                                      std::vector<size_t> basedOn,
                                                      IDependency::ImportanceLevel importanceLevel) {

  std::string anglePoint1IdTmp{anglePoint1Id};
  std::string angleVertexIdTmp{angleVertexId};
  std::string anglePoint2IdTmp{anglePoint2Id};

  changeAngleEnds(anglePoint1IdTmp, angleVertexIdTmp, anglePoint2IdTmp);

  const std::shared_ptr<LineAngleDependency> dependency = std::make_shared<LineAngleDependency>(
      IdentifierModel(lineId),
      AngleModel{angleVertexIdTmp, anglePoint1IdTmp, anglePoint2IdTmp, angleType},
      dependencyIdCounter, IDependency::Category::OF_LINE_AND_ANGLE, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addCirclePolygonDependency(const std::string& circleId, const std::vector<std::string>& polygonVerticesIds,
                                                          IDependency::Type type, IDependency::Reason reason,
                                                          std::vector<size_t> basedOn,
                                                          IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<CirclePolygonDependency> dependency = std::make_shared<CirclePolygonDependency>(
      IdentifierModel{circleId}, PolygonModel{polygonVerticesIds}, dependencyIdCounter,
      IDependency::Category::OF_CIRCLE_AND_POLYGON, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPolygonTypeDependency(const std::vector<std::string>& polygonVerticesIds,
                                                        PolygonModel::PolygonType polygonType, IDependency::Type type,
                                                        IDependency::Reason reason, std::vector<size_t> basedOn,
                                                        IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<PolygonTypeDependency> dependency = std::make_shared<PolygonTypeDependency>(
      IdentifierModel{std::to_string(static_cast<size_t>(polygonType))}, PolygonModel{polygonVerticesIds}, dependencyIdCounter,
      IDependency::Category::POLYGON_TYPE, type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

unsigned int DependenciesBank::addPointsPairsPairPointsPairDependency(const std::string& arm1End1, const std::string& arm1End2,
                                                                      const std::string& arm2End1, const std::string& arm2End2,
                                                                      const std::string& segmentEnd1, const std::string& segmentEnd2,
                                                                      IDependency::Type type, IDependency::Reason reason,
                                                                      std::vector<size_t> basedOn,
                                                                      IDependency::ImportanceLevel importanceLevel) {

  const std::shared_ptr<TriplePointsPairsDependency> dependency = std::make_shared<TriplePointsPairsDependency>(
      PointsPairModel{arm1End1, arm1End2}, PointsPairModel{arm2End1, arm2End2}, PointsPairModel{segmentEnd1, segmentEnd2},
       dependencyIdCounter, IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR,
      type, reason, std::move(basedOn), importanceLevel);

  if (checkIfDependencyExist(dependency)) {
    return 0;
  }

  add(dependency);
  return 1;
}

json DependenciesBank::getDependenciesAsJsonObjects() const {
  json result;

  std::map<IDependency::Type, std::vector<size_t>>::const_iterator mIt;
  std::vector<size_t>::const_iterator vIt;
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

std::pair<std::string, std::vector<std::string>> DependenciesBank::getLengthName(const PointModel& point1, const PointModel& point2) {
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
  const std::string& point1Name = point1.getName();
  const std::string& point2Name = point2.getName();
  const std::string& point3Name = point3.getName();

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