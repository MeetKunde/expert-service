#include "../Expert.h"

namespace expert {
unsigned int Expert::findSpecificPolygonDependencies(const expertBackground::PolygonModel& polygon, PolygonType type,
                                                     unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;
  std::vector<unsigned int> vertices = polygon.getVerticesIds();
  const size_t verticesNumber = vertices.size();

  switch (type) {
    case PolygonType::ISOSCELES_ACUTE_TRIANGLE:
      if (vertices.size() != 3) {
        throw std::invalid_argument("Given polygon model does not represent a triangle!");
      }

      newDependenciesNumber +=
          setEqualSides(vertices[0], vertices[2], vertices[1], vertices[2], IDependency::Reason::ISOSCELES_ACUTE_TRIANGLE,
                        {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualAngles(vertices[2], vertices[0], vertices[1], vertices[2], vertices[1], vertices[0], AngleType::CONVEX,
                         IDependency::Reason::ISOSCELES_ACUTE_TRIANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      return newDependenciesNumber;
    case PolygonType::EQUILATERAL_TRIANGLE:
      if (vertices.size() != 3) {
        throw std::invalid_argument("Given polygon model does not represent a triangle!");
      }

      for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
        newDependenciesNumber +=
            setEqualSides(vertices[vertexId], vertices[(vertexId + 1) % verticesNumber],
                          vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
                          IDependency::Reason::EQUILATERAL_TRIANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
        newDependenciesNumber += dependenciesBank.addConvexAngle(
            vertices[vertexId], vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
            Integer(MathHelper::EQUILATERAL_TRIANGLE_INTERNAL_ANGLE_VALUE), IDependency::Reason::EQUILATERAL_TRIANGLE,
            {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::SCALENE_RIGHT_TRIANGLE:
      if (vertices.size() != 3) {
        throw std::invalid_argument("Given polygon model does not represent a triangle!");
      }

      newDependenciesNumber += dependenciesBank.addConvexAngle(
          vertices[1], vertices[0], vertices[2], Integer(MathHelper::RIGHT_ANGLE_VALUE),
          IDependency::Reason::SCALENE_RIGHT_TRIANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      return newDependenciesNumber;
    case PolygonType::ISOSCELES_RIGHT_TRIANGLE:
      if (vertices.size() != 3) {
        throw std::invalid_argument("Given polygon model does not represent a triangle!");
      }

      newDependenciesNumber +=
          setEqualSides(vertices[0], vertices[1], vertices[0], vertices[2], IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE,
                        {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualAngles(vertices[0], vertices[1], vertices[2], vertices[0], vertices[2], vertices[1], AngleType::CONVEX,
                         IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber += dependenciesBank.addConvexAngle(
          vertices[1], vertices[0], vertices[2], Integer(MathHelper::RIGHT_ANGLE_VALUE),
          IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      return newDependenciesNumber;
    case PolygonType::OBTUSE_ISOSCELES_TRIANGLE:
      if (vertices.size() != 3) {
        throw std::invalid_argument("Given polygon model does not represent a triangle!");
      }

      newDependenciesNumber +=
          setEqualSides(vertices[0], vertices[2], vertices[1], vertices[2], IDependency::Reason::OBTUSE_ISOSCELES_TRIANGLE,
                        {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualAngles(vertices[2], vertices[0], vertices[1], vertices[2], vertices[1], vertices[0], AngleType::CONVEX,
                         IDependency::Reason::OBTUSE_ISOSCELES_TRIANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      return newDependenciesNumber;
    case PolygonType::SQUARE:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a square!");
      }

      for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
        newDependenciesNumber +=
            setEqualSides(vertices[vertexId], vertices[(vertexId + 1) % verticesNumber],
                          vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
                          IDependency::Reason::SQUARE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
        newDependenciesNumber += dependenciesBank.addConvexAngle(
            vertices[vertexId], vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
            Integer(MathHelper::SQUARE_INTERNAL_ANGLE_VALUE), IDependency::Reason::SQUARE, {dependencyId},
            IDependency::UsefulnessLevel::HIGH);
      }

      for (unsigned int diff = 0; diff < 2; diff++) {
        newDependenciesNumber +=
            setSidesParallelism(vertices[diff], vertices[(diff + 1) % 4], vertices[(diff + 2) % 4], vertices[(diff + 3) % 4],
                                IDependency::Reason::SQUARE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      for (unsigned int diff = 0; diff < 4; diff++) {
        newDependenciesNumber +=
            setSidesPerpendicularity(vertices[diff], vertices[(diff + 1) % 4], vertices[(diff + 1) % 4], vertices[(diff + 2) % 4],
                                     IDependency::Reason::SQUARE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      if (schemeGraph.checkIfEdgeExist(vertices[0], vertices[2]) && schemeGraph.checkIfEdgeExist(vertices[1], vertices[3])) {
        // both diagonal exist

        newDependenciesNumber +=
            setEqualSides(vertices[0], vertices[2], vertices[1], vertices[3], IDependency::Reason::SQUARE_DIAGONAL,
                          {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        const unsigned int diagonal1 = shapesBank.getLineIdThrowTwoPoints(vertices[0], vertices[2]);
        const unsigned int diagonal2 = shapesBank.getLineIdThrowTwoPoints(vertices[1], vertices[3]);

        newDependenciesNumber += dependenciesBank.addLinesDependency(diagonal1, diagonal2, IDependency::Type::PERPENDICULAR_LINES,
                                                                     IDependency::Reason::SQUARE_DIAGONAL, {dependencyId},
                                                                     IDependency::UsefulnessLevel::HIGH);

        const unsigned diagonalsIntersectionPoint = intersectionPointsOfLines[diagonal1][diagonal2][0];

        newDependenciesNumber +=
            setEqualSides(vertices[0], diagonalsIntersectionPoint, vertices[2], diagonalsIntersectionPoint,
                          IDependency::Reason::SQUARE_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        newDependenciesNumber +=
            setEqualSides(vertices[1], diagonalsIntersectionPoint, vertices[3], diagonalsIntersectionPoint,
                          IDependency::Reason::SQUARE_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::RECTANGLE:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a rectangle!");
      }

      newDependenciesNumber += setEqualSides(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::RECTANGLE,
                                             {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber += setEqualSides(vertices[1], vertices[2], vertices[3], vertices[0], IDependency::Reason::RECTANGLE,
                                             {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
        newDependenciesNumber += dependenciesBank.addConvexAngle(
            vertices[vertexId], vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
            Integer(MathHelper::RECTANGLE_INTERNAL_ANGLE_VALUE), IDependency::Reason::RECTANGLE, {dependencyId},
            IDependency::UsefulnessLevel::HIGH);
      }

      for (unsigned int diff = 0; diff < 2; diff++) {
        newDependenciesNumber +=
            setSidesParallelism(vertices[diff], vertices[(diff + 1) % 4], vertices[(diff + 2) % 4], vertices[(diff + 3) % 4],
                                IDependency::Reason::RECTANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      for (unsigned int diff = 0; diff < 4; diff++) {
        newDependenciesNumber +=
            setSidesPerpendicularity(vertices[diff], vertices[(diff + 1) % 4], vertices[(diff + 1) % 4], vertices[(diff + 2) % 4],
                                     IDependency::Reason::RECTANGLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      if (schemeGraph.checkIfEdgeExist(vertices[0], vertices[2]) && schemeGraph.checkIfEdgeExist(vertices[1], vertices[3])) {
        // both diagonal exist

        newDependenciesNumber +=
            setEqualSides(vertices[0], vertices[2], vertices[1], vertices[3], IDependency::Reason::RECTANGLE_DIAGONAL,
                          {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        const unsigned int diagonal1 = shapesBank.getLineIdThrowTwoPoints(vertices[0], vertices[2]);
        const unsigned int diagonal2 = shapesBank.getLineIdThrowTwoPoints(vertices[1], vertices[3]);

        const unsigned diagonalsIntersectionPoint = intersectionPointsOfLines[diagonal1][diagonal2][0];

        newDependenciesNumber +=
            setEqualSides(vertices[0], diagonalsIntersectionPoint, vertices[2], diagonalsIntersectionPoint,
                          IDependency::Reason::RECTANGLE_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        newDependenciesNumber +=
            setEqualSides(vertices[1], diagonalsIntersectionPoint, vertices[3], diagonalsIntersectionPoint,
                          IDependency::Reason::RECTANGLE_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::REGULAR_POLYGON:
      if (vertices.size() < 3) {
        throw std::invalid_argument("Given polygon model does not represent a regular polygon!");
      }
      else if (vertices.size() == 3) {
        return findSpecificPolygonDependencies(polygon, PolygonType::EQUILATERAL_TRIANGLE, dependencyId);
      }
      else if (vertices.size() == 4) {
        return findSpecificPolygonDependencies(polygon, PolygonType::SQUARE, dependencyId);
      }
      else {
        for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
          newDependenciesNumber +=
              setEqualSides(vertices[vertexId], vertices[(vertexId + 1) % verticesNumber],
                            vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
                            IDependency::Reason::REGULAR_POLYGON, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
        }

        for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
          newDependenciesNumber += dependenciesBank.addConvexAngle(
              vertices[vertexId], vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
              Integer((static_cast<int>(verticesNumber) - 2) * MathHelper::STRAIGHT_ANGLE_VALUE) /
                  Integer(static_cast<int>(verticesNumber)),
              IDependency::Reason::REGULAR_POLYGON, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
        }
      }
      return newDependenciesNumber;
    case PolygonType::PARALLELOGRAM:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a parallelogram!");
      }

      newDependenciesNumber +=
          setEqualSides(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::PARALLELOGRAM, {dependencyId},
                        IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setEqualSides(vertices[1], vertices[2], vertices[3], vertices[0], IDependency::Reason::PARALLELOGRAM, {dependencyId},
                        IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setSidesParallelism(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::PARALLELOGRAM,
                              {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setSidesParallelism(vertices[1], vertices[2], vertices[3], vertices[0], IDependency::Reason::PARALLELOGRAM,
                              {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setEqualAngles(vertices[0], vertices[1], vertices[2], vertices[2], vertices[3], vertices[0], AngleType::CONVEX,
                         IDependency::Reason::PARALLELOGRAM, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setEqualAngles(vertices[1], vertices[2], vertices[3], vertices[3], vertices[0], vertices[1], AngleType::CONVEX,
                         IDependency::Reason::PARALLELOGRAM, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      if (schemeGraph.checkIfEdgeExist(vertices[0], vertices[2]) && schemeGraph.checkIfEdgeExist(vertices[1], vertices[3])) {
        // both diagonal exist
        const unsigned int diagonal1 = shapesBank.getLineIdThrowTwoPoints(vertices[0], vertices[2]);
        const unsigned int diagonal2 = shapesBank.getLineIdThrowTwoPoints(vertices[1], vertices[3]);

        const unsigned diagonalsIntersectionPoint = intersectionPointsOfLines[diagonal1][diagonal2][0];

        newDependenciesNumber +=
            setEqualSides(vertices[0], diagonalsIntersectionPoint, vertices[2], diagonalsIntersectionPoint,
                          IDependency::Reason::PARALLELOGRAM_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        newDependenciesNumber +=
            setEqualSides(vertices[1], diagonalsIntersectionPoint, vertices[3], diagonalsIntersectionPoint,
                          IDependency::Reason::PARALLELOGRAM_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::KITE:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a kite!");
      }

      newDependenciesNumber += setEqualSides(vertices[0], vertices[1], vertices[3], vertices[0], IDependency::Reason::KITE,
                                             {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber += setEqualSides(vertices[1], vertices[2], vertices[2], vertices[3], IDependency::Reason::KITE,
                                             {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualAngles(vertices[0], vertices[1], vertices[2], vertices[2], vertices[3], vertices[0], AngleType::CONVEX,
                         IDependency::Reason::KITE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      if (schemeGraph.checkIfEdgeExist(vertices[0], vertices[2]) && schemeGraph.checkIfEdgeExist(vertices[1], vertices[3])) {
        // both diagonal exist
        const unsigned int diagonal1 = shapesBank.getLineIdThrowTwoPoints(vertices[0], vertices[2]);
        const unsigned int diagonal2 = shapesBank.getLineIdThrowTwoPoints(vertices[1], vertices[3]);

        const unsigned diagonalsIntersectionPoint = intersectionPointsOfLines[diagonal1][diagonal2][0];

        newDependenciesNumber +=
            setEqualSides(vertices[1], diagonalsIntersectionPoint, vertices[3], diagonalsIntersectionPoint,
                          IDependency::Reason::KITE_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        newDependenciesNumber +=
            setSidesPerpendicularity(vertices[1], vertices[3], vertices[0], vertices[2], IDependency::Reason::KITE_DIAGONAL,
                                     {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::RHOMBUS:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a rhombus!");
      }

      for (size_t vertexId = 0; vertexId < verticesNumber; vertexId++) {
        newDependenciesNumber +=
            setEqualSides(vertices[vertexId], vertices[(vertexId + 1) % verticesNumber],
                          vertices[(vertexId + 1) % verticesNumber], vertices[(vertexId + 2) % verticesNumber],
                          IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      newDependenciesNumber +=
          setSidesParallelism(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::RHOMBUS, {dependencyId},
                              IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setSidesParallelism(vertices[1], vertices[2], vertices[3], vertices[0], IDependency::Reason::RHOMBUS, {dependencyId},
                              IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setEqualAngles(vertices[0], vertices[1], vertices[2], vertices[2], vertices[3], vertices[0], AngleType::CONVEX,
                         IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      newDependenciesNumber +=
          setEqualAngles(vertices[1], vertices[2], vertices[3], vertices[3], vertices[0], vertices[1], AngleType::CONVEX,
                         IDependency::Reason::RHOMBUS, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      if (schemeGraph.checkIfEdgeExist(vertices[0], vertices[2]) && schemeGraph.checkIfEdgeExist(vertices[1], vertices[3])) {
        // both diagonal exist

        const unsigned int diagonal1 = shapesBank.getLineIdThrowTwoPoints(vertices[0], vertices[2]);
        const unsigned int diagonal2 = shapesBank.getLineIdThrowTwoPoints(vertices[1], vertices[3]);

        newDependenciesNumber += dependenciesBank.addLinesDependency(diagonal1, diagonal2, IDependency::Type::PERPENDICULAR_LINES,
                                                                     IDependency::Reason::RHOMBUS_DIAGONAL, {dependencyId},
                                                                     IDependency::UsefulnessLevel::HIGH);

        const unsigned diagonalsIntersectionPoint = intersectionPointsOfLines[diagonal1][diagonal2][0];

        newDependenciesNumber +=
            setEqualSides(vertices[0], diagonalsIntersectionPoint, vertices[2], diagonalsIntersectionPoint,
                          IDependency::Reason::RHOMBUS_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

        newDependenciesNumber +=
            setEqualSides(vertices[1], diagonalsIntersectionPoint, vertices[3], diagonalsIntersectionPoint,
                          IDependency::Reason::RHOMBUS_DIAGONAL, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::SCALENE_TRAPEZOID:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a trapezoid!");
      }

      newDependenciesNumber +=
          setSidesParallelism(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::SCALENE_TRAPEZOID,
                              {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      return newDependenciesNumber;
    case PolygonType::ISOSCELES_TRAPEZOID:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a trapezoid!");
      }

      newDependenciesNumber +=
          setSidesParallelism(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::ISOSCELES_TRAPEZOID,
                              {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualSides(vertices[1], vertices[2], vertices[3], vertices[0], IDependency::Reason::ISOSCELES_TRAPEZOID,
                        {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualAngles(vertices[0], vertices[1], vertices[2], vertices[3], vertices[0], vertices[1], AngleType::CONVEX,
                         IDependency::Reason::ISOSCELES_TRAPEZOID, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber +=
          setEqualAngles(vertices[1], vertices[2], vertices[3], vertices[2], vertices[3], vertices[0], AngleType::CONVEX,
                         IDependency::Reason::ISOSCELES_TRAPEZOID, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      if (schemeGraph.checkIfEdgeExist(vertices[0], vertices[2]) && schemeGraph.checkIfEdgeExist(vertices[1], vertices[3])) {
        newDependenciesNumber +=
            setEqualSides(vertices[0], vertices[2], vertices[1], vertices[3], IDependency::Reason::ISOSCELES_TRAPEZOID_DIAGONAL,
                          {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      }

      return newDependenciesNumber;
    case PolygonType::RIGHT_TRAPEZOID:
      if (vertices.size() != 4) {
        throw std::invalid_argument("Given polygon model does not represent a trapezoid!");
      }

      newDependenciesNumber +=
          setSidesParallelism(vertices[0], vertices[1], vertices[2], vertices[3], IDependency::Reason::RIGHT_TRAPEZOID,
                              {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber += dependenciesBank.addConcaveAngle(
          vertices[2], vertices[3], vertices[0], Integer(MathHelper::RIGHT_ANGLE_VALUE), IDependency::Reason::RIGHT_TRAPEZOID,
          {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      newDependenciesNumber += dependenciesBank.addConcaveAngle(
          vertices[3], vertices[0], vertices[1], Integer(MathHelper::RIGHT_ANGLE_VALUE), IDependency::Reason::RIGHT_TRAPEZOID,
          {dependencyId}, IDependency::UsefulnessLevel::HIGH);

      return newDependenciesNumber;
    default:
      return 0;
  }
}

unsigned int Expert::findMidPerpendicularDependencies(const expertBackground::PointsPairModel& segment, unsigned int lineId,
                                                      unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;
  const unsigned int baseId = shapesBank.getLineIdThrowTwoPoints(segment.getPoint1Id(), segment.getPoint2Id());
  const unsigned int intersectionPoint = intersectionPointsOfLines[lineId][baseId][0];

  newDependenciesNumber +=
      setEqualSides(segment.getPoint1Id(), intersectionPoint, intersectionPoint, segment.getPoint2Id(),
                    IDependency::Reason::MID_PERPENDICULAR, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

  newDependenciesNumber += dependenciesBank.addLinesDependency(baseId, lineId, IDependency::Type::PERPENDICULAR_LINES,
                                                               IDependency::Reason::MID_PERPENDICULAR, {dependencyId},
                                                               IDependency::UsefulnessLevel::HIGH);

  return newDependenciesNumber;
}

unsigned int Expert::findBisectorDependencies(const expertBackground::AngleModel& angle, unsigned int lineId,
                                              unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;

  std::vector<unsigned int> linePoints = shapesBank.getLine(lineId).getIncludedPoints();
  for (unsigned int i = 0; i < linePoints.size(); i++) {
    if (linePoints[i] != angle.getVertexId()) {
      newDependenciesNumber += setEqualAngles(angle.getPoint1Id(), angle.getVertexId(), linePoints[i], angle.getPoint2Id(),
                                              angle.getVertexId(), linePoints[i], AngleType::CONVEX,
                                              IDependency::Reason::BISECTOR, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
      break;
    }
  }

  return newDependenciesNumber;
}

unsigned int Expert::findTangentLineDependencies(unsigned int lineId, unsigned int circleId, unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;

  const unsigned int commonPointId = intersectionPointsOfLinesAndCircles[lineId][circleId][0];
  const unsigned int circleCenterId = shapesBank.getCircle(circleId).getCenterId();
  if (schemeGraph.checkIfEdgeExist(commonPointId, circleCenterId)) {
    const unsigned int radiusLineId = shapesBank.getLineIdThrowTwoPoints(commonPointId, circleCenterId);
    newDependenciesNumber += dependenciesBank.addLinesDependency(lineId, radiusLineId, IDependency::Type::PERPENDICULAR_LINES,
                                                                 IDependency::Reason::LINE_TANGENT_TO_CIRCLE, {dependencyId},
                                                                 IDependency::UsefulnessLevel::HIGH);
  }

  return newDependenciesNumber;
}

unsigned int Expert::findMedianDependencies(const expertBackground::PointsPairModel& base,
                                            const expertBackground::PointsPairModel& median, unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;
  const expertBackground::LineModel baseLine =
      shapesBank.getLine(shapesBank.getLineIdThrowTwoPoints(base.getPoint1Id(), base.getPoint2Id()));
  std::vector<unsigned int> basePoints = baseLine.getIncludedPoints();

  const unsigned int pointOnBase = std::find(basePoints.begin(), basePoints.end(), median.getPoint1Id()) != basePoints.end()
                                 ? median.getPoint1Id()
                                 : median.getPoint2Id();

  newDependenciesNumber += setEqualSides(base.getPoint1Id(), pointOnBase, pointOnBase, base.getPoint2Id(),
                                         IDependency::Reason::MEDIAN, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

  return newDependenciesNumber;
}

unsigned int Expert::findAltitudeDependencies(unsigned int baseLineId, const expertBackground::PointsPairModel& altitude,
                                              unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;

  const unsigned int altitudeLineId = shapesBank.getLineIdThrowTwoPoints(altitude.getPoint1Id(), altitude.getPoint2Id());

  newDependenciesNumber +=
      dependenciesBank.addLinesDependency(baseLineId, altitudeLineId, IDependency::Type::PERPENDICULAR_LINES,
                                          IDependency::Reason::ALTITUDE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

  return newDependenciesNumber;
}

unsigned int Expert::findMidSegmentDependencies(const expertBackground::PointsPairModel& arm1,
                                                const expertBackground::PointsPairModel& arm2,
                                                const expertBackground::PointsPairModel& midSegment, unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;
  const unsigned int arm1End1 = arm1.getPoint1Id();
  const unsigned int arm1End2 = arm1.getPoint2Id();
  const unsigned int arm2End1 = arm2.getPoint1Id();
  const unsigned int arm2End2 = arm2.getPoint2Id();

  if (arm1End2 == arm2End2 && schemeGraph.checkIfEdgeExist(arm1End1, arm2End1)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End1, arm2End1,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        expertBackground::DependenciesBank::getLengthVariable(arm1End1, arm2End1) / Integer(2), IDependency::Reason::MID_SEGMENT,
        {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (arm1End2 == arm2End1 && schemeGraph.checkIfEdgeExist(arm1End1, arm2End2)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End1, arm2End2,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        expertBackground::DependenciesBank::getLengthVariable(arm1End1, arm2End2) / Integer(2), IDependency::Reason::MID_SEGMENT,
        {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (arm1End1 == arm2End2 && schemeGraph.checkIfEdgeExist(arm1End2, arm2End1)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End2, arm2End1,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        expertBackground::DependenciesBank::getLengthVariable(arm1End2, arm2End1) / Integer(2), IDependency::Reason::MID_SEGMENT,
        {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (arm1End1 == arm2End1 && schemeGraph.checkIfEdgeExist(arm1End2, arm2End2)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End2, arm2End2,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        expertBackground::DependenciesBank::getLengthVariable(arm1End2, arm2End2) / Integer(2), IDependency::Reason::MID_SEGMENT,
        {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (schemeGraph.checkIfEdgeExist(arm1End2, arm2End2) && schemeGraph.checkIfEdgeExist(arm1End1, arm2End1)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End2, arm2End2,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End1, arm2End1,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        (expertBackground::DependenciesBank::getLengthVariable(arm1End2, arm2End2) +
         expertBackground::DependenciesBank::getLengthVariable(arm1End1, arm2End1)) /
            Integer(2),
        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (schemeGraph.checkIfEdgeExist(arm1End2, arm2End1) && schemeGraph.checkIfEdgeExist(arm1End1, arm2End2)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End2, arm2End1,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End1, arm2End2,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        (expertBackground::DependenciesBank::getLengthVariable(arm1End2, arm2End1) +
         expertBackground::DependenciesBank::getLengthVariable(arm1End1, arm2End2)) /
            Integer(2),
        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (schemeGraph.checkIfEdgeExist(arm1End1, arm2End2) && schemeGraph.checkIfEdgeExist(arm1End2, arm2End1)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End1, arm2End2,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End2, arm2End1,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        (expertBackground::DependenciesBank::getLengthVariable(arm1End1, arm2End2) +
         expertBackground::DependenciesBank::getLengthVariable(arm1End2, arm2End1)) /
            Integer(2),
        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else if (schemeGraph.checkIfEdgeExist(arm1End1, arm2End1) && schemeGraph.checkIfEdgeExist(arm1End2, arm2End2)) {
    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End1, arm2End1,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber +=
        setSidesParallelism(midSegment.getPoint1Id(), midSegment.getPoint2Id(), arm1End2, arm2End2,
                            IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += dependenciesBank.addEquation(
        expertBackground::DependenciesBank::getLengthVariable(midSegment.getPoint1Id(), midSegment.getPoint2Id()),
        (expertBackground::DependenciesBank::getLengthVariable(arm1End1, arm2End1) +
         expertBackground::DependenciesBank::getLengthVariable(arm1End2, arm2End2)) /
            Integer(2),
        IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }

  std::vector<unsigned int> arm1Points =
      shapesBank.getLine(shapesBank.getLineIdThrowTwoPoints(arm1End1, arm1End2)).getIncludedPoints();

  if (std::find(arm1Points.begin(), arm1Points.end(), midSegment.getPoint1Id()) != arm1Points.end()) {
    newDependenciesNumber += setEqualSides(arm1End1, midSegment.getPoint1Id(), midSegment.getPoint1Id(), arm1End2,
                                           IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += setEqualSides(arm2End1, midSegment.getPoint2Id(), midSegment.getPoint2Id(), arm2End2,
                                           IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }
  else {
    newDependenciesNumber += setEqualSides(arm1End1, midSegment.getPoint2Id(), midSegment.getPoint2Id(), arm1End2,
                                           IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

    newDependenciesNumber += setEqualSides(arm2End1, midSegment.getPoint1Id(), midSegment.getPoint1Id(), arm2End2,
                                           IDependency::Reason::MID_SEGMENT, {dependencyId}, IDependency::UsefulnessLevel::HIGH);
  }

  return newDependenciesNumber;
}

unsigned int Expert::findInscribedCircleDependencies(const expertBackground::PolygonModel& polygon, unsigned int circleId,
                                                     unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;
  const std::vector<unsigned int>& vertices = polygon.getVerticesIds();
  const unsigned int verticesNumber = vertices.size();

  for (unsigned int vertexId = 0; vertexId < verticesNumber; vertexId++) {
    newDependenciesNumber += dependenciesBank.addLineCircleDependency(
        shapesBank.getLineIdThrowTwoPoints(vertices[vertexId], vertices[(vertexId + 1) % verticesNumber]), circleId,
        IDependency::Type::TANGENT_LINE_TO_CIRCLE, IDependency::Reason::INSCRIBED_CIRCLE, {dependencyId},
        IDependency::UsefulnessLevel::HIGH);
  }

  return newDependenciesNumber;
}

unsigned int Expert::findEscribedCircleDependencies(const expertBackground::PolygonModel& polygon, unsigned int circleId,
                                                    unsigned int dependencyId) {
  unsigned int newDependenciesNumber = 0;
  const std::vector<unsigned int> vertices = polygon.getVerticesIds();

  newDependenciesNumber += dependenciesBank.addLineCircleDependency(
      shapesBank.getLineIdThrowTwoPoints(vertices[1], vertices[2]), circleId, IDependency::Type::TANGENT_LINE_TO_CIRCLE,
      IDependency::Reason::ESCRIBED_CIRCLE, {dependencyId}, IDependency::UsefulnessLevel::HIGH);

  return newDependenciesNumber;
}

unsigned int Expert::findSpecificShapesDependencies() {
  unsigned int sumOfNewDependencies = 0;

  std::vector<std::shared_ptr<PolygonTypeDependency>> definedPolygonTypes = dependenciesBank.getPolygonTypeDependencies();

  for (unsigned int i = 0; i < definedPolygonTypes.size(); i++) {
    sumOfNewDependencies += findSpecificPolygonDependencies(
        definedPolygonTypes[i]->getSecondElement(), static_cast<PolygonType>(definedPolygonTypes[i]->getFirstElement().getId()),
        definedPolygonTypes[i]->getId());
  }

  std::vector<std::shared_ptr<CirclePolygonDependency>> definedInscribedCircles =
      dependenciesBank.getCirclePolygonDependencies(CirclePolygonDependencies::INSCRIBED_CIRCLE);

  for (unsigned int i = 0; i < definedInscribedCircles.size(); i++) {
    sumOfNewDependencies += findInscribedCircleDependencies(definedInscribedCircles[i]->getSecondElement(),
                                                            definedInscribedCircles[i]->getFirstElement().getId(),
                                                            definedInscribedCircles[i]->getId());
  }

  std::vector<std::shared_ptr<CirclePolygonDependency>> definedEscribedCircles =
      dependenciesBank.getCirclePolygonDependencies(CirclePolygonDependencies::ESCRIBED_CIRCLE);

  for (unsigned int i = 0; i < definedEscribedCircles.size(); i++) {
    sumOfNewDependencies +=
        findEscribedCircleDependencies(definedEscribedCircles[i]->getSecondElement(),
                                       definedEscribedCircles[i]->getFirstElement().getId(), definedEscribedCircles[i]->getId());
  }

  std::vector<std::shared_ptr<PointsPairsDependency>> definedMedians =
      dependenciesBank.getPointPairsDependencies(PointsPairsDependencies::MEDIAN);

  for (unsigned int i = 0; i < definedMedians.size(); i++) {
    sumOfNewDependencies += findMedianDependencies(definedMedians[i]->getFirstElement(), definedMedians[i]->getSecondElement(),
                                                   definedMedians[i]->getId());
  }

  std::vector<std::shared_ptr<LinePointPairDependency>> definedAltitudes =
      dependenciesBank.getLinePointPairDependencies(LinePointPairDependencies::ALTITUDE);

  for (unsigned int i = 0; i < definedAltitudes.size(); i++) {
    sumOfNewDependencies += findAltitudeDependencies(definedAltitudes[i]->getFirstElement().getId(),
                                                     definedAltitudes[i]->getSecondElement(), definedAltitudes[i]->getId());
  }

  std::vector<std::shared_ptr<PointsPairsPairPointsPairDependency>> definedMidSegments =
      dependenciesBank.getPointsPairsPairPointsPairDependencies(PointsPairsPairPointsPairDependencies::MID_SEGMENT);

  for (unsigned int i = 0; i < definedMidSegments.size(); i++) {
    sumOfNewDependencies += findMidSegmentDependencies(definedMidSegments[i]->getFirstElement().getFirstElement(),
                                                       definedMidSegments[i]->getFirstElement().getSecondElement(),
                                                       definedMidSegments[i]->getSecondElement(), definedMidSegments[i]->getId());
  }

  std::vector<std::shared_ptr<LinePointPairDependency>> definedMidPerpendiculars =
      dependenciesBank.getLinePointPairDependencies(LinePointPairDependencies::MID_PERPENDICULAR_LINE);

  for (unsigned int i = 0; i < definedMidPerpendiculars.size(); i++) {
    sumOfNewDependencies += findMidPerpendicularDependencies(definedMidPerpendiculars[i]->getSecondElement(),
                                                             definedMidPerpendiculars[i]->getFirstElement().getId(),
                                                             definedMidPerpendiculars[i]->getId());
  }

  std::vector<std::shared_ptr<LineAngleDependency>> definedBisectors =
      dependenciesBank.getLineAngleDependencies(LineAngleDependencies::BISECTOR_LINE);

  for (unsigned int i = 0; i < definedBisectors.size(); i++) {
    sumOfNewDependencies += findBisectorDependencies(
        definedBisectors[i]->getSecondElement(), definedBisectors[i]->getFirstElement().getId(), definedBisectors[i]->getId());
  }

  std::vector<std::shared_ptr<LineCircleDependency>> definedTangentLinesToCircles =
      dependenciesBank.getLineCircleDependencies(LineCircleDependencies::TANGENT_LINE_TO_CIRCLE);

  for (unsigned int i = 0; i < definedTangentLinesToCircles.size(); i++) {
    sumOfNewDependencies += findTangentLineDependencies(definedTangentLinesToCircles[i]->getFirstElement().getId(),
                                                        definedTangentLinesToCircles[i]->getSecondElement().getId(),
                                                        definedTangentLinesToCircles[i]->getId());
  }

  return sumOfNewDependencies;
}
}  // namespace expert
