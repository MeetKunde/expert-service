#include "../Expert.h"

namespace expert {
unsigned int Expert::explorePolygonTypeBasedDependencies() {
  unsigned int sumOfNewDependencies{0};

  if (dependenciesBank.lastChange(IDependency::Type::POLYGON_TYPE)) {
    const std::vector<std::shared_ptr<PolygonTypeDependency>> definedPolygonTypes =
        dependenciesBank.getPolygonTypeDependencies(PolygonTypeDependencies::POLYGON_TYPE);

    for (size_t index = 0; index < definedPolygonTypes.size(); index++) {
      const PolygonType polygonType{static_cast<PolygonType>(std::stoi(definedPolygonTypes[index]->getFirstObject().getId()))};

      switch (polygonType) {
        case PolygonType::UNKNOWN:
          break;
        case PolygonType::ISOSCELES_ACUTE_TRIANGLE:
          sumOfNewDependencies += setIsoscelesAcuteTriangleDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::EQUILATERAL_TRIANGLE:
          sumOfNewDependencies += setEquilateralTriangleDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::SCALENE_RIGHT_TRIANGLE:
          sumOfNewDependencies += setScaleneRightTriangleDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::ISOSCELES_RIGHT_TRIANGLE:
          sumOfNewDependencies += setIsoscelesRightTriangleDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::OBTUSE_ISOSCELES_TRIANGLE:
          sumOfNewDependencies += setObtuseIsoscelesTriangleDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::SQUARE:
          sumOfNewDependencies += setSquareDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::RECTANGLE:
          sumOfNewDependencies += setRectangleDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::REGULAR_POLYGON:
          sumOfNewDependencies += setRegularPolygonDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::PARALLELOGRAM:
          sumOfNewDependencies += setParallelogramDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::KITE:
          sumOfNewDependencies += setKiteDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::RHOMBUS:
          sumOfNewDependencies += setRhombusDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::SCALENE_TRAPEZOID:
          sumOfNewDependencies += setScaleneTrapezoidDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::ISOSCELES_TRAPEZOID:
          sumOfNewDependencies += setIsoscelesTrapezoidDependencies(definedPolygonTypes[index]);
          break;
        case PolygonType::RIGHT_TRAPEZOID:
          sumOfNewDependencies += setRightTrapezoidDependencies(definedPolygonTypes[index]);
          break;
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setIsoscelesAcuteTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 3) {
    sumOfNewDependencies +=
        setEqualSides(polygonVertices[0], polygonVertices[2], polygonVertices[1], polygonVertices[2],
                      IDependency::Reason::ISOSCELES_ACUTE_TRIANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[2], polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[1],
                       polygonVertices[0], AngleType::CONVEX, IDependency::Reason::ISOSCELES_ACUTE_TRIANGLE,
                       {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setEquilateralTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 3) {
    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies +=
          setEqualSides(polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                        polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                        polygonVertices[(vertexId + 2) % polygonVerticesNumber],
                        IDependency::Reason::EQUILATERAL_TRIANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies += dependenciesBank.addConvexAngle(
          polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
          polygonVertices[(vertexId + 2) % polygonVerticesNumber],
          Integer(MathHelper::EQUILATERAL_TRIANGLE_INTERNAL_ANGLE_VALUE), IDependency::Reason::EQUILATERAL_TRIANGLE,
          {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setScaleneRightTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 3) {
    sumOfNewDependencies += dependenciesBank.addConvexAngle(
        polygonVertices[1], polygonVertices[0], polygonVertices[2], Integer(MathHelper::RIGHT_ANGLE_VALUE),
        IDependency::Reason::SCALENE_RIGHT_TRIANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setIsoscelesRightTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 3) {
    sumOfNewDependencies +=
        setEqualSides(polygonVertices[0], polygonVertices[1], polygonVertices[0], polygonVertices[2],
                      IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[0], polygonVertices[2],
                       polygonVertices[1], AngleType::CONVEX, IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE,
                       {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies += dependenciesBank.addConvexAngle(
        polygonVertices[1], polygonVertices[0], polygonVertices[2], Integer(MathHelper::RIGHT_ANGLE_VALUE),
        IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setObtuseIsoscelesTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 3) {
    sumOfNewDependencies +=
        setEqualSides(polygonVertices[0], polygonVertices[2], polygonVertices[1], polygonVertices[2],
                      IDependency::Reason::OBTUSE_ISOSCELES_TRIANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[2], polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[1],
                       polygonVertices[0], AngleType::CONVEX, IDependency::Reason::OBTUSE_ISOSCELES_TRIANGLE,
                       {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setSquareDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies +=
          setEqualSides(polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                        polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                        polygonVertices[(vertexId + 2) % polygonVerticesNumber], IDependency::Reason::SQUARE,
                        {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    /*
    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies += dependenciesBank.addConvexAngle(
          polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
          polygonVertices[(vertexId + 2) % polygonVerticesNumber], Integer(MathHelper::SQUARE_INTERNAL_ANGLE_VALUE),
          IDependency::Reason::SQUARE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
    */
    for (size_t diff = 0; diff < 2; diff++) {
      sumOfNewDependencies +=
          setSidesParallelism(polygonVertices[diff], polygonVertices[(diff + 1) % 4], polygonVertices[(diff + 2) % 4],
                              polygonVertices[(diff + 3) % 4], IDependency::Reason::SQUARE, {dependencyId},
                              IDependency::ImportanceLevel::HIGH);
    }

    for (size_t diff = 0; diff < 4; diff++) {
      sumOfNewDependencies +=
          setSidesPerpendicularity(polygonVertices[diff], polygonVertices[(diff + 1) % 4],
                                   polygonVertices[(diff + 1) % 4], polygonVertices[(diff + 2) % 4],
                                   IDependency::Reason::SQUARE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[0]),
                                     shapesBank.getPointPositionInVector(polygonVertices[2])) &&
        schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[1]),
                                     shapesBank.getPointPositionInVector(polygonVertices[3]))) {
      // both diagonal exist

      sumOfNewDependencies +=
          setEqualSides(polygonVertices[0], polygonVertices[2], polygonVertices[1], polygonVertices[3],
                        IDependency::Reason::SQUARE_DIAGONAL, {dependencyId}, IDependency::ImportanceLevel::HIGH);

      const std::string& diagonal1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[2]);
      const std::string& diagonal2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[3]);

      sumOfNewDependencies += dependenciesBank.addLinesDependency(
          diagonal1, diagonal2, LinesDependencies::PERPENDICULAR_LINES, IDependency::Reason::SQUARE_DIAGONAL,
          {dependencyId}, IDependency::ImportanceLevel::HIGH);

      const size_t diagonalsIntersectionPoint =
          intersectionPointsOfLines[shapesBank.getLinePositionInVector(diagonal1)]
                                   [shapesBank.getLinePositionInVector(diagonal2)][0];
      const std::string& diagonalsIntersectionPointId =
          shapesBank.getPointsVector().at(diagonalsIntersectionPoint).getId();

      sumOfNewDependencies += setEqualSides(polygonVertices[0], diagonalsIntersectionPointId, polygonVertices[2],
                                            diagonalsIntersectionPointId, IDependency::Reason::SQUARE_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);

      sumOfNewDependencies += setEqualSides(polygonVertices[1], diagonalsIntersectionPointId, polygonVertices[3],
                                            diagonalsIntersectionPointId, IDependency::Reason::SQUARE_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setRectangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    sumOfNewDependencies +=
        setEqualSides(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                      IDependency::Reason::RECTANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualSides(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[0],
                      IDependency::Reason::RECTANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies += dependenciesBank.addConvexAngle(
          polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
          polygonVertices[(vertexId + 2) % polygonVerticesNumber], Integer(MathHelper::RECTANGLE_INTERNAL_ANGLE_VALUE),
          IDependency::Reason::RECTANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    for (unsigned int diff = 0; diff < 2; diff++) {
      sumOfNewDependencies +=
          setSidesParallelism(polygonVertices[diff], polygonVertices[(diff + 1) % 4], polygonVertices[(diff + 2) % 4],
                              polygonVertices[(diff + 3) % 4], IDependency::Reason::RECTANGLE, {dependencyId},
                              IDependency::ImportanceLevel::HIGH);
    }

    for (unsigned int diff = 0; diff < 4; diff++) {
      sumOfNewDependencies +=
          setSidesPerpendicularity(polygonVertices[diff], polygonVertices[(diff + 1) % 4],
                                   polygonVertices[(diff + 1) % 4], polygonVertices[(diff + 2) % 4],
                                   IDependency::Reason::RECTANGLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[0]),
                                     shapesBank.getPointPositionInVector(polygonVertices[2])) &&
        schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[1]),
                                     shapesBank.getPointPositionInVector(polygonVertices[3]))) {
      // both diagonal exist

      sumOfNewDependencies +=
          setEqualSides(polygonVertices[0], polygonVertices[2], polygonVertices[1], polygonVertices[3],
                        IDependency::Reason::RECTANGLE_DIAGONAL, {dependencyId}, IDependency::ImportanceLevel::HIGH);

      const std::string& diagonal1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[2]);
      const std::string& diagonal2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[3]);

      const unsigned diagonalsIntersectionPoint =
          intersectionPointsOfLines[shapesBank.getLinePositionInVector(diagonal1)]
                                   [shapesBank.getPointPositionInVector(diagonal2)][0];
      const std::string& diagonalsIntersectionPointId =
          shapesBank.getPointsVector().at(diagonalsIntersectionPoint).getId();

      sumOfNewDependencies += setEqualSides(polygonVertices[0], diagonalsIntersectionPointId, polygonVertices[2],
                                            diagonalsIntersectionPointId, IDependency::Reason::RECTANGLE_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);

      sumOfNewDependencies += setEqualSides(polygonVertices[1], diagonalsIntersectionPointId, polygonVertices[3],
                                            diagonalsIntersectionPointId, IDependency::Reason::RECTANGLE_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setRegularPolygonDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 3) {
    sumOfNewDependencies += setEquilateralTriangleDependencies(dependency);
  }
  else if (polygonVerticesNumber == 4) {
    sumOfNewDependencies += setSquareDependencies(dependency);
  }
  else {
    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies +=
          setEqualSides(polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                        polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                        polygonVertices[(vertexId + 2) % polygonVerticesNumber], IDependency::Reason::REGULAR_POLYGON,
                        {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies += dependenciesBank.addConvexAngle(
          polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
          polygonVertices[(vertexId + 2) % polygonVerticesNumber],
          Integer((static_cast<int>(polygonVerticesNumber) - 2) * MathHelper::STRAIGHT_ANGLE_VALUE) /
              Integer(static_cast<int>(polygonVerticesNumber)),
          IDependency::Reason::REGULAR_POLYGON, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setParallelogramDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    sumOfNewDependencies +=
        setEqualSides(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                      IDependency::Reason::PARALLELOGRAM, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies +=
        setEqualSides(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[0],
                      IDependency::Reason::PARALLELOGRAM, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies +=
        setSidesParallelism(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                            IDependency::Reason::PARALLELOGRAM, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies +=
        setSidesParallelism(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[0],
                            IDependency::Reason::PARALLELOGRAM, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[2], polygonVertices[3],
                       polygonVertices[0], AngleType::CONVEX, IDependency::Reason::PARALLELOGRAM, {dependencyId},
                       IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[3], polygonVertices[0],
                       polygonVertices[1], AngleType::CONVEX, IDependency::Reason::PARALLELOGRAM, {dependencyId},
                       IDependency::ImportanceLevel::HIGH);

    if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[0]),
                                     shapesBank.getPointPositionInVector(polygonVertices[2])) &&
        schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[1]),
                                     shapesBank.getPointPositionInVector(polygonVertices[3]))) {
      // both diagonal exist
      const std::string& diagonal1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[2]);
      const std::string& diagonal2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[3]);

      const size_t diagonalsIntersectionPoint =
          intersectionPointsOfLines[shapesBank.getLinePositionInVector(diagonal1)]
                                   [shapesBank.getLinePositionInVector(diagonal2)][0];
      const std::string& diagonalsIntersectionPointId =
          shapesBank.getPointsVector().at(diagonalsIntersectionPoint).getId();

      sumOfNewDependencies += setEqualSides(polygonVertices[0], diagonalsIntersectionPointId, polygonVertices[2],
                                            diagonalsIntersectionPointId, IDependency::Reason::PARALLELOGRAM_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);

      sumOfNewDependencies += setEqualSides(polygonVertices[1], diagonalsIntersectionPointId, polygonVertices[3],
                                            diagonalsIntersectionPointId, IDependency::Reason::PARALLELOGRAM_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setKiteDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    sumOfNewDependencies += setEqualSides(polygonVertices[0], polygonVertices[1], polygonVertices[3], polygonVertices[0],
                                          IDependency::Reason::KITE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies += setEqualSides(polygonVertices[1], polygonVertices[2], polygonVertices[2], polygonVertices[3],
                                          IDependency::Reason::KITE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies += setEqualAngles(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[2],
                                           polygonVertices[3], polygonVertices[0], AngleType::CONVEX,
                                           IDependency::Reason::KITE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[0]),
                                     shapesBank.getPointPositionInVector(polygonVertices[2])) &&
        schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[1]),
                                     shapesBank.getPointPositionInVector(polygonVertices[3]))) {
      // both diagonal exist
      const std::string& diagonal1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[2]);
      const std::string& diagonal2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[3]);

      const size_t diagonalsIntersectionPoint =
          intersectionPointsOfLines[shapesBank.getLinePositionInVector(diagonal1)]
                                   [shapesBank.getLinePositionInVector(diagonal2)][0];
      const std::string& diagonalsIntersectionPointId =
          shapesBank.getPointsVector().at(diagonalsIntersectionPoint).getId();

      sumOfNewDependencies += setEqualSides(polygonVertices[1], diagonalsIntersectionPointId, polygonVertices[3],
                                            diagonalsIntersectionPointId, IDependency::Reason::KITE_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);

      sumOfNewDependencies += setSidesPerpendicularity(polygonVertices[1], polygonVertices[3], polygonVertices[0],
                                                       polygonVertices[2], IDependency::Reason::KITE_DIAGONAL,
                                                       {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setRhombusDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
      sumOfNewDependencies += setEqualSides(polygonVertices[vertexId], polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                                            polygonVertices[(vertexId + 1) % polygonVerticesNumber],
                                            polygonVertices[(vertexId + 2) % polygonVerticesNumber], IDependency::Reason::RHOMBUS,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }

    sumOfNewDependencies += setSidesParallelism(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                                                IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies += setSidesParallelism(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[0],
                                                IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies += setEqualAngles(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[2],
                                           polygonVertices[3], polygonVertices[0], AngleType::CONVEX,
                                           IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    sumOfNewDependencies += setEqualAngles(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[3],
                                           polygonVertices[0], polygonVertices[1], AngleType::CONVEX,
                                           IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[0]),
                                     shapesBank.getPointPositionInVector(polygonVertices[2])) &&
        schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[1]),
                                     shapesBank.getPointPositionInVector(polygonVertices[3]))) {
      // both diagonal exist

      const std::string& diagonal1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[2]);
      const std::string& diagonal2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[3]);

      sumOfNewDependencies += dependenciesBank.addLinesDependency(diagonal1, diagonal2, LinesDependencies::PERPENDICULAR_LINES,
                                                                  IDependency::Reason::RHOMBUS_DIAGONAL, {dependencyId},
                                                                  IDependency::ImportanceLevel::HIGH);

      const size_t diagonalsIntersectionPoint = intersectionPointsOfLines[shapesBank.getLinePositionInVector(diagonal1)]
                                                                         [shapesBank.getLinePositionInVector(diagonal2)][0];
      const std::string& diagonalsIntersectionPointId = shapesBank.getPointsVector().at(diagonalsIntersectionPoint).getId();

      sumOfNewDependencies +=
          setEqualSides(polygonVertices[0], diagonalsIntersectionPointId, polygonVertices[2], diagonalsIntersectionPointId,
                        IDependency::Reason::RHOMBUS_DIAGONAL, {dependencyId}, IDependency::ImportanceLevel::HIGH);

      sumOfNewDependencies +=
          setEqualSides(polygonVertices[1], diagonalsIntersectionPointId, polygonVertices[3], diagonalsIntersectionPointId,
                        IDependency::Reason::RHOMBUS_DIAGONAL, {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }
  return sumOfNewDependencies;
}

unsigned int Expert::setScaleneTrapezoidDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    sumOfNewDependencies +=
        setSidesParallelism(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                            IDependency::Reason::SCALENE_TRAPEZOID, {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setIsoscelesTrapezoidDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    sumOfNewDependencies +=
        setSidesParallelism(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                            IDependency::Reason::ISOSCELES_TRAPEZOID, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualSides(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[0],
                      IDependency::Reason::ISOSCELES_TRAPEZOID, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[0],
                       polygonVertices[1], AngleType::CONVEX, IDependency::Reason::ISOSCELES_TRAPEZOID, {dependencyId},
                       IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies +=
        setEqualAngles(polygonVertices[1], polygonVertices[2], polygonVertices[3], polygonVertices[2], polygonVertices[3],
                       polygonVertices[0], AngleType::CONVEX, IDependency::Reason::ISOSCELES_TRAPEZOID, {dependencyId},
                       IDependency::ImportanceLevel::HIGH);

    if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[0]),
                                     shapesBank.getPointPositionInVector(polygonVertices[2])) &&
        schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(polygonVertices[1]),
                                     shapesBank.getPointPositionInVector(polygonVertices[3]))) {
      // both diagonal exist

      sumOfNewDependencies += setEqualSides(polygonVertices[0], polygonVertices[2], polygonVertices[1],
                                            polygonVertices[3], IDependency::Reason::ISOSCELES_TRAPEZOID_DIAGONAL,
                                            {dependencyId}, IDependency::ImportanceLevel::HIGH);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setRightTrapezoidDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  if (polygonVerticesNumber == 4) {
    sumOfNewDependencies +=
        setSidesParallelism(polygonVertices[0], polygonVertices[1], polygonVertices[2], polygonVertices[3],
                            IDependency::Reason::RIGHT_TRAPEZOID, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies += dependenciesBank.addConcaveAngle(
        polygonVertices[2], polygonVertices[3], polygonVertices[0], Integer(MathHelper::RIGHT_ANGLE_VALUE),
        IDependency::Reason::RIGHT_TRAPEZOID, {dependencyId}, IDependency::ImportanceLevel::HIGH);

    sumOfNewDependencies += dependenciesBank.addConcaveAngle(
        polygonVertices[3], polygonVertices[0], polygonVertices[1], Integer(MathHelper::RIGHT_ANGLE_VALUE),
        IDependency::Reason::RIGHT_TRAPEZOID, {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::exploreSpecificSegmentsBasedDependencies() {
  unsigned int sumOfNewDependencies{0};

  if(dependenciesBank.lastChange(IDependency::Type::MEDIAN)) {
    const std::vector<std::shared_ptr<PolygonPointsPairDependency>> medians =
        dependenciesBank.getPolygonPointsPairDependencies(PolygonPointsPairDependencies::MEDIAN);

    for (size_t index = 0; index < medians.size(); index++) {
      sumOfNewDependencies += setMedianDependencies(medians[index]);
    }
  }

  if(dependenciesBank.lastChange(IDependency::Type::ALTITUDE)) {
    const std::vector<std::shared_ptr<PolygonPointsPairDependency>> altitudes =
        dependenciesBank.getPolygonPointsPairDependencies(PolygonPointsPairDependencies::ALTITUDE);

    for (size_t index = 0; index < altitudes.size(); index++) {
      sumOfNewDependencies += setAltitudeDependencies(altitudes[index]);
    }
  }

  if(dependenciesBank.lastChange(IDependency::Type::MID_SEGMENT)) {
    const std::vector<std::shared_ptr<PolygonPointsPairDependency>> midSegments =
        dependenciesBank.getPolygonPointsPairDependencies(PolygonPointsPairDependencies::MID_SEGMENT);

    for (size_t index = 0; index < midSegments.size(); index++) {
      sumOfNewDependencies += setMidSegmentDependencies(midSegments[index]);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setMedianDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getFirstObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const std::string& segmentEnd1{dependency->getSecondObject().getPoint1Id()};
  const std::string& segmentEnd2{dependency->getSecondObject().getPoint2Id()};
  const size_t dependencyId{dependency->getId()};

  if(polygonVerticesNumber == 3) {
    const std::string& baseLine = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[1]);
    const std::vector<std::string>& baseLinePoints = shapesBank.getLine(baseLine).getIncludedPoints();

    const std::string& pointOnBase =
        std::find(baseLinePoints.begin(), baseLinePoints.end(), segmentEnd1) != baseLinePoints.end() ? segmentEnd1 : segmentEnd2;


    sumOfNewDependencies += setEqualSides(polygonVertices[1], pointOnBase,
                                          pointOnBase, polygonVertices[1],
                                          IDependency::Reason::MEDIAN, {dependencyId},
                                          IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setAltitudeDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getFirstObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const std::string& segmentEnd1{dependency->getSecondObject().getPoint1Id()};
  const std::string& segmentEnd2{dependency->getSecondObject().getPoint2Id()};
  const size_t dependencyId{dependency->getId()};

  if(polygonVerticesNumber == 3) {
    const std::string& altitudeLine = shapesBank.getLineIdThrowTwoPoints(segmentEnd1, segmentEnd2);
    const std::string& baseLine = shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[1]);

    sumOfNewDependencies +=
        dependenciesBank.addLinesDependency(baseLine, altitudeLine, LinesDependencies::PERPENDICULAR_LINES,
                                            IDependency::Reason::ALTITUDE, {dependencyId}, IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setMidSegmentDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getFirstObject().getVerticesIds()};

  if(polygonVertices.size() == 3) {
    sumOfNewDependencies += setMidSegmentInTriangleDependencies(dependency);
  }
  else if(polygonVertices.size() == 4) {
    sumOfNewDependencies += setMidSegmentInTrapezoidDependencies(dependency);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setMidSegmentInTriangleDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getFirstObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const std::string& segmentEnd1{dependency->getSecondObject().getPoint1Id()};
  const std::string& segmentEnd2{dependency->getSecondObject().getPoint2Id()};
  const size_t dependencyId{dependency->getId()};

  const std::string& arm1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[1]);
  const std::string& arm2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[2]);

  const std::vector<std::string>& arm1Points{shapesBank.getLine(arm1).getIncludedPoints()};
  const std::vector<std::string>& arm2Points{shapesBank.getLine(arm2).getIncludedPoints()};

  const std::string& pointOnArm1 = std::find(arm1Points.begin(), arm1Points.end(), segmentEnd1) != arm1Points.end() ? segmentEnd1 : segmentEnd2;
  const std::string& pointOnArm2 = std::find(arm2Points.begin(), arm2Points.end(), segmentEnd1) != arm2Points.end() ? segmentEnd1 : segmentEnd2;

  sumOfNewDependencies += setEqualSides(polygonVertices[0], pointOnArm1, pointOnArm1, polygonVertices[1],
                                        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  sumOfNewDependencies += setEqualSides(polygonVertices[0], pointOnArm2, pointOnArm2, polygonVertices[2],
                                        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  sumOfNewDependencies += setSidesParallelism(polygonVertices[1], polygonVertices[2], segmentEnd1, segmentEnd2,
                                              IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  const PointModel& segmentEnd1Point = shapesBank.getPoint(segmentEnd1);
  const PointModel& segmentEnd2Point = shapesBank.getPoint(segmentEnd2);
  const PointModel& baseEnd1Point = shapesBank.getPoint(polygonVertices[0]);
  const PointModel& baseEnd2Point = shapesBank.getPoint(polygonVertices[2]);

  sumOfNewDependencies += dependenciesBank.addEquation(
      expertBackground::DependenciesBank::getSegmentLengthVariable(segmentEnd1Point, segmentEnd2Point),
      expertBackground::DependenciesBank::getSegmentLengthVariable(baseEnd1Point, baseEnd2Point) /
          Integer(2),
      IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  return sumOfNewDependencies;
}

unsigned int Expert::setMidSegmentInTrapezoidDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::string> polygonVertices{dependency->getFirstObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const std::string& segmentEnd1{dependency->getSecondObject().getPoint1Id()};
  const std::string& segmentEnd2{dependency->getSecondObject().getPoint2Id()};
  const size_t dependencyId{dependency->getId()};

  const std::string& arm1 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[0], polygonVertices[1]);
  const std::string& arm2 = shapesBank.getLineIdThrowTwoPoints(polygonVertices[2], polygonVertices[3]);

  const std::vector<std::string>& arm1Points{shapesBank.getLine(arm1).getIncludedPoints()};
  const std::vector<std::string>& arm2Points{shapesBank.getLine(arm2).getIncludedPoints()};

  const std::string& pointOnArm1 = std::find(arm1Points.begin(), arm1Points.end(), segmentEnd1) != arm1Points.end() ? segmentEnd1 : segmentEnd2;
  const std::string& pointOnArm2 = std::find(arm2Points.begin(), arm2Points.end(), segmentEnd1) != arm2Points.end() ? segmentEnd1 : segmentEnd2;

  sumOfNewDependencies += setEqualSides(polygonVertices[0], pointOnArm1, pointOnArm1, polygonVertices[1],
                                        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  sumOfNewDependencies += setEqualSides(polygonVertices[2], pointOnArm2, pointOnArm2, polygonVertices[3],
                                        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  sumOfNewDependencies += setSidesParallelism(polygonVertices[0], polygonVertices[2], segmentEnd1, segmentEnd2,
                                              IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  sumOfNewDependencies += setSidesParallelism(polygonVertices[1], polygonVertices[3], segmentEnd1, segmentEnd2,
                                              IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  const PointModel& segmentEnd1Point = shapesBank.getPoint(segmentEnd1);
  const PointModel& segmentEnd2Point = shapesBank.getPoint(segmentEnd2);
  const PointModel& topBaseEnd1Point = shapesBank.getPoint(polygonVertices[0]);
  const PointModel& topBaseEnd2Point = shapesBank.getPoint(polygonVertices[2]);
  const PointModel& bottomBaseEnd1Point = shapesBank.getPoint(polygonVertices[1]);
  const PointModel& bottomBaseEnd2Point = shapesBank.getPoint(polygonVertices[3]);

  sumOfNewDependencies += dependenciesBank.addEquation(
      expertBackground::DependenciesBank::getSegmentLengthVariable(segmentEnd1Point, segmentEnd2Point),
      (expertBackground::DependenciesBank::getSegmentLengthVariable(topBaseEnd1Point, topBaseEnd2Point) +
       expertBackground::DependenciesBank::getSegmentLengthVariable(bottomBaseEnd1Point, bottomBaseEnd2Point)) /
          Integer(2),
      IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  return sumOfNewDependencies;
}

unsigned int Expert::exploreTangentLineAndCircleBasedDependencies() {
  unsigned int sumOfNewDependencies{0};

  if(dependenciesBank.lastChange(IDependency::Type::TANGENT_LINE_TO_CIRCLE)) {
    const std::vector<std::shared_ptr<LineCircleDependency>> tangentLineCircle =
        dependenciesBank.getLineCircleDependencies(LineCircleDependencies::TANGENT_LINE_TO_CIRCLE);

    for (size_t index = 0; index < tangentLineCircle.size(); index++) {
      sumOfNewDependencies += setTangentLineCircleDependencies(tangentLineCircle[index]);
    }
  }

  if(dependenciesBank.lastChange(IDependency::Type::TANGENT_CIRCLE_TO_CIRCLE)) {
    const std::vector<std::shared_ptr<CirclesDependency>> tangentCircleCircle =
        dependenciesBank.getCirclesDependencies(CirclesDependencies::TANGENT_CIRCLE_TO_CIRCLE);

    for (size_t index = 0; index < tangentCircleCircle.size(); index++) {
      sumOfNewDependencies += setTangentCircleCircleDependencies(tangentCircleCircle[index]);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setTangentLineCircleDependencies(const std::shared_ptr<LineCircleDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::string& line{dependency->getFirstObject().getId()};
  const std::string& circle{dependency->getSecondObject().getId()};

  const size_t commonPoint =
      intersectionPointsOfLinesAndCircles[shapesBank.getLinePositionInVector(line)][shapesBank.getCirclePositionInVector(circle)][0];
  const std::string& commonPointId{shapesBank.getPointsVector().at(commonPoint).getId()};
  const std::string& circleCenterId{shapesBank.getCircle(circle).getCenterId()};

  const size_t dependencyId{dependency->getId()};

  if (schemeGraph.checkIfEdgeExist(shapesBank.getPointPositionInVector(circleCenterId),
                                   shapesBank.getPointPositionInVector(commonPointId))) {
    const std::string& radiusLine = shapesBank.getLineIdThrowTwoPoints(commonPointId, circleCenterId);

    sumOfNewDependencies += dependenciesBank.addLinesDependency(line, radiusLine, LinesDependencies::PERPENDICULAR_LINES,
                                                                IDependency::Reason::LINE_TANGENT_TO_CIRCLE, {dependencyId},
                                                                IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setTangentCircleCircleDependencies(const std::shared_ptr<CirclesDependency>& /*dependency*/) {
  const unsigned int sumOfNewDependencies{0};

  return sumOfNewDependencies;
}

unsigned int Expert::explorePolygonCircleBasedDependencies() {
  unsigned int sumOfNewDependencies{0};

  if(dependenciesBank.lastChange(IDependency::Type::INSCRIBED_CIRCLE)) {
    const std::vector<std::shared_ptr<CirclePolygonDependency>> inscribedCircles =
        dependenciesBank.getCirclePolygonDependencies(CirclePolygonDependencies::INSCRIBED_CIRCLE);

    for (size_t index = 0; index < inscribedCircles.size(); index++) {
      sumOfNewDependencies += setInscribedCircleDependencies(inscribedCircles[index]);
    }
  }

  if(dependenciesBank.lastChange(IDependency::Type::CIRCUMSCRIBED_CIRCLE)) {
    const std::vector<std::shared_ptr<CirclePolygonDependency>> circumscribedCircles =
        dependenciesBank.getCirclePolygonDependencies(CirclePolygonDependencies::CIRCUMSCRIBED_CIRCLE);

    for (size_t index = 0; index < circumscribedCircles.size(); index++) {
      sumOfNewDependencies += setCircumscribedCircleDependencies(circumscribedCircles[index]);
    }
  }

  if(dependenciesBank.lastChange(IDependency::Type::ESCRIBED_CIRCLE)) {
    const std::vector<std::shared_ptr<CirclePolygonDependency>> escribedCircles =
        dependenciesBank.getCirclePolygonDependencies(CirclePolygonDependencies::ESCRIBED_CIRCLE);

    for (size_t index = 0; index < escribedCircles.size(); index++) {
      sumOfNewDependencies += setEscribedCircleDependencies(escribedCircles[index]);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setInscribedCircleDependencies(const std::shared_ptr<CirclePolygonDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::string& circle{dependency->getFirstObject().getId()};
  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  for (size_t vertexId = 0; vertexId < polygonVerticesNumber; vertexId++) {
    sumOfNewDependencies += dependenciesBank.addLineCircleDependency(
        shapesBank.getLineIdThrowTwoPoints(
            polygonVertices[vertexId],
            polygonVertices[(vertexId + 1) % polygonVerticesNumber]),
        circle,
        LineCircleDependencies::TANGENT_LINE_TO_CIRCLE, IDependency::Reason::INSCRIBED_CIRCLE, {dependencyId},
        IDependency::ImportanceLevel::HIGH);
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setCircumscribedCircleDependencies(const std::shared_ptr<CirclePolygonDependency>& /*dependency*/) {
  const unsigned int sumOfNewDependencies{0};

  return sumOfNewDependencies;
}

unsigned int Expert::setEscribedCircleDependencies(const std::shared_ptr<CirclePolygonDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::string& circle{dependency->getFirstObject().getId()};
  const std::vector<std::string> polygonVertices{dependency->getSecondObject().getVerticesIds()};
  const size_t polygonVerticesNumber{polygonVertices.size()};
  const size_t dependencyId{dependency->getId()};

  sumOfNewDependencies += dependenciesBank.addLineCircleDependency(
      shapesBank.getLineIdThrowTwoPoints(polygonVertices[1], polygonVertices[2]), circle,
      LineCircleDependencies::TANGENT_LINE_TO_CIRCLE,
      IDependency::Reason::ESCRIBED_CIRCLE, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  return sumOfNewDependencies;
}

unsigned int Expert::exploreSpecificLineBasedDependencies() {
  unsigned int sumOfNewDependencies{0};

  if(dependenciesBank.lastChange(IDependency::Type::MID_PERPENDICULAR_LINE)) {
    const std::vector<std::shared_ptr<LinePointsPairDependency>> midPerpendicularLines =
        dependenciesBank.getLinePointsPairDependencies(LinePointsPairDependencies::MID_PERPENDICULAR_LINE);

    for (size_t index = 0; index < midPerpendicularLines.size(); index++) {
      sumOfNewDependencies += setMidPerpendicularLineDependencies(midPerpendicularLines[index]);
    }
  }

  if(dependenciesBank.lastChange(IDependency::Type::BISECTOR_LINE)) {
    const std::vector<std::shared_ptr<LineAngleDependency>> bisectorLines =
        dependenciesBank.getLineAngleDependencies(LineAngleDependencies::BISECTOR_LINE);

    for (size_t index = 0; index < bisectorLines.size(); index++) {
      sumOfNewDependencies += setBisectorLineDependencies(bisectorLines[index]);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setMidPerpendicularLineDependencies(const std::shared_ptr<LinePointsPairDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::string& midPerpendicularLine = dependency->getFirstObject().getId();
  const std::string& segmentEnd1{dependency->getSecondObject().getPoint1Id()};
  const std::string& segmentEnd2{dependency->getSecondObject().getPoint2Id()};
  const size_t dependencyId{dependency->getId()};

  const std::string& baseLine = shapesBank.getLineIdThrowTwoPoints(segmentEnd1, segmentEnd2);
  const size_t intersectionPoint =
      intersectionPointsOfLines[shapesBank.getLinePositionInVector(baseLine)][shapesBank.getLinePositionInVector(midPerpendicularLine)][0];
  const std::string& intersectionPointId = shapesBank.getPointsVector().at(intersectionPoint).getId();

  sumOfNewDependencies +=
      setEqualSides(segmentEnd1, intersectionPointId, intersectionPointId, segmentEnd2,
                    IDependency::Reason::MID_PERPENDICULAR, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  sumOfNewDependencies += dependenciesBank.addLinesDependency(baseLine, midPerpendicularLine,
                                                              LinesDependencies::PERPENDICULAR_LINES,
                                                              IDependency::Reason::MID_PERPENDICULAR, {dependencyId},
                                                              IDependency::ImportanceLevel::HIGH);

  return sumOfNewDependencies;
}

unsigned int Expert::setBisectorLineDependencies(const std::shared_ptr<LineAngleDependency>& dependency) {
  unsigned int sumOfNewDependencies{0};

  const std::string& bisectorLine = dependency->getFirstObject().getId();
  const std::string& angleEnd1{dependency->getSecondObject().getPoint1Id()};
  const std::string& angleVertex{dependency->getSecondObject().getVertexId()};
  const std::string& angleEnd2{dependency->getSecondObject().getPoint2Id()};
  const AngleType angleType{dependency->getSecondObject().getAngleType()};
  const size_t dependencyId{dependency->getId()};

  const std::vector<std::string>& bisectorLinePoints = shapesBank.getLine(bisectorLine).getIncludedPoints();
  const std::string& pointOnBisectorLine =
      angleVertex == bisectorLinePoints.front() ? bisectorLinePoints.back() : bisectorLinePoints.front();

  sumOfNewDependencies += setEqualAngles(angleEnd1, angleVertex, pointOnBisectorLine,
                                         angleEnd2, angleVertex, pointOnBisectorLine,
                                         angleType,
                                         IDependency::Reason::BISECTOR, {dependencyId}, IDependency::ImportanceLevel::HIGH);

  return sumOfNewDependencies;
}

}  // namespace expert
