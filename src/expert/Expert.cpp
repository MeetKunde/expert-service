#include "Expert.h"

namespace expert {
Expert::Expert() : pointsNumber(0), linesNumber(0), circlesNumber(0) {}

void Expert::importTask(json task) {
  localTimer.startMeasurement();

  shapesBank = expertBackground::ShapesBank(task["maxShapeId"], task["linesNumber"], task["maxShapeId"]);

  json::iterator iter;
  json points = task["points"];
  json circles = task["circles"];
  json lines = task["lines"];

  for (iter = points.begin(); iter != points.end(); ++iter) {
    shapesBank.addPoint((*iter)["id"], (*iter)["x"], (*iter)["y"]);
  }

  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    shapesBank.addCircle((*iter)["id"], shapesBank.getUnifiedPointId((*iter)["centerId"]), (*iter)["centerX"], (*iter)["centerY"],
                         (*iter)["radius"], shapesBank.getUnifiedPointsVector(parseUnsignedIntVector((*iter)["pointsOn"])));
  }

  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    shapesBank.addLine((*iter)["id"], parseLineType((*iter)["type"]), (*iter)["a"], (*iter)["b"],
                       shapesBank.getUnifiedPointsVector(parseUnsignedIntVector((*iter)["pointsOn"])));
  }

  pointsNumber = static_cast<unsigned int>(shapesBank.getPointsNumber());
  linesNumber = static_cast<unsigned int>(shapesBank.getLinesNumber());
  circlesNumber = static_cast<unsigned int>(shapesBank.getCirclesNumber());

  schemeGraph = expertBackground::Graph(static_cast<unsigned int>(shapesBank.getPointsNumber()));
  for (unsigned int lineId = 0; lineId < shapesBank.getLinesNumber(); lineId++) {
    std::vector<unsigned int> includedPoints = shapesBank.getLine(lineId).getIncludedPoints();
    for (unsigned firstPointOnLineId = 0; firstPointOnLineId < includedPoints.size(); firstPointOnLineId++) {
      for (unsigned int secondPointOnLineId = firstPointOnLineId + 1; secondPointOnLineId < includedPoints.size();
           secondPointOnLineId++) {
        schemeGraph.addEdge(includedPoints[firstPointOnLineId], includedPoints[secondPointOnLineId]);
      }
    }
  }

  dependenciesBank = expertBackground::DependenciesBank(&shapesBank);

  addValues(task["lengths"], task["angleValues"]);
  addLinesDependencies(task["perpendicular"], task["parallel"]);
  addEqualismDependencies(task["equalSegments"], task["equalAngles"]);
  addTangentLinesAndCircles(task["tangentLines"], task["tangentCircles"]);
  addCirclePolygonDependencies(task["inscribedCircles"], task["circumscribedCircles"], task["escribedCircles"]);
  addBisectors(task["bisectorLines"]);
  addMidPerpendiculars(task["midPerpendicularLines"]);
  addPolygonTypes(task["polygonTypes"]);
  addSpecialSegments(task["medians"], task["altitudes"], task["midSegments"]);

  intersectionPointsOfLines.resize(linesNumber, std::vector<std::vector<unsigned int>>(linesNumber));
  intersectionPointsOfLinesAndCircles.resize(linesNumber, std::vector<std::vector<unsigned int>>(circlesNumber));
  intersectionPointsOfCircles.resize(circlesNumber, std::vector<std::vector<unsigned int>>(circlesNumber));

  pointsOnLinesIntersections.resize(pointsNumber, {});
  pointsOnCirclesIntersections.resize(pointsNumber, {});
  pointsOnLineAndCircleIntersections.resize(pointsNumber, {});

  pointsOnShapes.resize(pointsNumber, {false, false});

  findIntersectionPointsOfLines();
  findIntersectionPointsOfCircles();
  findIntersectionPointsOfLinesCircles();
  checkPointsOnShapes();

  localTimer.takeMeasurement("Importing and parsing data", false, std::cout);
}

json Expert::exportSolution() {
  localTimer.startMeasurement();

  json output = {{"points", shapesBank.getPointsAsJsonObjects()},
                 {"lines", shapesBank.getLinesAsJsonObjects()},
                 {"circles", shapesBank.getCirclesAsJsonObjects()},
                 {"intersections", getIntersectionPointsAsJson()},
                 {"indexes_of_variables", dependenciesBank.getVariablesIndexesAsJsonObject()},
                 {"dependencies", dependenciesBank.getDependenciesAsJsonObjects()}};

  localTimer.takeMeasurement("Exporting data", false, std::cout);

  return output;
}

void Expert::useKnowledge() {
  unsigned int dependenciesFoundNumber = 0;

  localTimer.startMeasurement();

  dependenciesFoundNumber += findSpecificShapesDependencies();

  dependenciesFoundNumber += findVerticalAngles();
  dependenciesFoundNumber += findSupplementaryAngles();
  dependenciesFoundNumber += findAlternateAngles();
  dependenciesFoundNumber += findCorrespondingAngles();

  //dependenciesFoundNumber += setSumOfAnglesInTriangles();

  dependenciesBank.extractEquations();

  //dependenciesFoundNumber += findSimilarAndCongruentTriangles();

  dependenciesFoundNumber += setRightAnglesBasedOnPerpendicularities();
  dependenciesFoundNumber += findParallelLinesBasedOnParallelLines();
  dependenciesFoundNumber += findParallelLinesBasedOnPerpendicularLines();
  dependenciesFoundNumber += findPerpendicularLinesBasedOnLines();
  dependenciesFoundNumber += findPerpendicularLinesBasedOnRightAngles();

  localTimer.takeMeasurement("Using knowledge ended!\nFound " + std::to_string(dependenciesFoundNumber) + " new dependencies!",
                             false, std::cout);
}

void Expert::addValues(json lengths, json angleValues) {
  json::iterator iter;

  for (iter = lengths.begin(); iter != lengths.end(); ++iter) {
    dependenciesBank.addLength(shapesBank.getUnifiedPointId((*iter)["end1Id"]), shapesBank.getUnifiedPointId((*iter)["end2Id"]),
                               expertBackground::ExpressionModel(static_cast<std::string>((*iter)["value"])),
                               IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = angleValues.begin(); iter != angleValues.end(); ++iter) {
    if (parseAngleType((*iter)["type"]) == AngleType::CONCAVE) {
      dependenciesBank.addConcaveAngle(shapesBank.getUnifiedPointId((*iter)["end1Id"]),
                                       shapesBank.getUnifiedPointId((*iter)["vertexId"]),
                                       shapesBank.getUnifiedPointId((*iter)["end2Id"]),
                                       expertBackground::ExpressionModel(static_cast<std::string>((*iter)["value"])),
                                       IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
    }
    else if (parseAngleType((*iter)["type"]) == AngleType::CONVEX) {
      dependenciesBank.addConvexAngle(shapesBank.getUnifiedPointId((*iter)["end1Id"]),
                                      shapesBank.getUnifiedPointId((*iter)["vertexId"]),
                                      shapesBank.getUnifiedPointId((*iter)["end2Id"]),
                                      expertBackground::ExpressionModel(static_cast<std::string>((*iter)["value"])),
                                      IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
    }
  }
}

void Expert::addLinesDependencies(json perpendicular, json parallel) {
  json::iterator iter;

  for (iter = perpendicular.begin(); iter != perpendicular.end(); ++iter) {
    dependenciesBank.addLinesDependency(shapesBank.getUnifiedLineId((*iter)["id1"]), shapesBank.getUnifiedLineId((*iter)["id2"]),
                                        IDependency::Type::PERPENDICULAR_LINES, IDependency::Reason::USER_DEFINED, {},
                                        IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = parallel.begin(); iter != parallel.end(); ++iter) {
    dependenciesBank.addLinesDependency(shapesBank.getUnifiedLineId((*iter)["id1"]), shapesBank.getUnifiedLineId((*iter)["id2"]),
                                        IDependency::Type::PARALLEL_LINES, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addEqualismDependencies(json segments, json angles) {
  json::iterator iter;

  for (iter = segments.begin(); iter != segments.end(); ++iter) {
    dependenciesBank.addPointPairsDependency(
        shapesBank.getUnifiedPointId((*iter)["s1End1Id"]), shapesBank.getUnifiedPointId((*iter)["s1End2Id"]),
        shapesBank.getUnifiedPointId((*iter)["s2End1Id"]), shapesBank.getUnifiedPointId((*iter)["s2End2Id"]),
        IDependency::Type::EQUAL_SEGMENTS, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = angles.begin(); iter != angles.end(); ++iter) {
    dependenciesBank.addAnglesDependency(
        shapesBank.getUnifiedPointId((*iter)["a1End1Id"]), shapesBank.getUnifiedPointId((*iter)["a1VertexId"]),
        shapesBank.getUnifiedPointId((*iter)["a1End2Id"]), AngleType::CONVEX, shapesBank.getUnifiedPointId((*iter)["a2End1Id"]),
        shapesBank.getUnifiedPointId((*iter)["a2VertexId"]), shapesBank.getUnifiedPointId((*iter)["a2End2Id"]), AngleType::CONVEX,
        IDependency::Type::EQUAL_ANGLES, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
    dependenciesBank.addAnglesDependency(
        shapesBank.getUnifiedPointId((*iter)["a1End1Id"]), shapesBank.getUnifiedPointId((*iter)["a1VertexId"]),
        shapesBank.getUnifiedPointId((*iter)["a1End2Id"]), AngleType::CONCAVE, shapesBank.getUnifiedPointId((*iter)["a2End1Id"]),
        shapesBank.getUnifiedPointId((*iter)["a2VertexId"]), shapesBank.getUnifiedPointId((*iter)["a2End2Id"]),
        AngleType::CONCAVE, IDependency::Type::EQUAL_ANGLES, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addTangentLinesAndCircles(json lines, json circles) {
  json::iterator iter;

  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    dependenciesBank.addLineCircleDependency(
        shapesBank.getUnifiedLineId((*iter)["lineId"]), shapesBank.getUnifiedCircleId((*iter)["circleId"]),
        IDependency::Type::TANGENT_LINE_TO_CIRCLE, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    dependenciesBank.addCirclesDependency(shapesBank.getUnifiedCircleId((*iter)["id1"]),
                                          shapesBank.getUnifiedCircleId((*iter)["id2"]), IDependency::Type::TANGENT_CIRCLE_TO_CIRCLE,
                                          IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addCirclePolygonDependencies(json inscribed, json circumscribed, json escribed) {
  json::iterator iter;

  for (iter = inscribed.begin(); iter != inscribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency(shapesBank.getUnifiedCircleId((*iter)["circleId"]),
                                                shapesBank.getUnifiedPointsVector(parseUnsignedIntVector((*iter)["polygon"])),
                                                IDependency::Type::INSCRIBED_CIRCLE, IDependency::Reason::USER_DEFINED, {},
                                                IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = circumscribed.begin(); iter != circumscribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency(shapesBank.getUnifiedCircleId((*iter)["circleId"]),
                                                shapesBank.getUnifiedPointsVector(parseUnsignedIntVector((*iter)["polygon"])),
                                                IDependency::Type::CIRCUMSCRIBED_CIRCLE, IDependency::Reason::USER_DEFINED, {},
                                                IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = escribed.begin(); iter != escribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency(shapesBank.getUnifiedCircleId((*iter)["circleId"]),
                                                shapesBank.getUnifiedPointsVector(parseUnsignedIntVector((*iter)["polygon"])),
                                                IDependency::Type::ESCRIBED_CIRCLE, IDependency::Reason::USER_DEFINED, {},
                                                IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addBisectors(json bisectorLines) {
  json::iterator iter;

  for (iter = bisectorLines.begin(); iter != bisectorLines.end(); ++iter) {
    dependenciesBank.addLineAngleDependency(
        shapesBank.getUnifiedLineId((*iter)["lineId"]), shapesBank.getUnifiedPointId((*iter)["point1Id"]),
        shapesBank.getUnifiedPointId((*iter)["vertexId"]), shapesBank.getUnifiedPointId((*iter)["point2Id"]),
        parseAngleType((*iter)["angleType"]), IDependency::Type::BISECTOR_LINE, IDependency::Reason::USER_DEFINED, {},
        IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addMidPerpendiculars(json midPerpendicularLines) {
  json::iterator iter;

  for (iter = midPerpendicularLines.begin(); iter != midPerpendicularLines.end(); ++iter) {
    dependenciesBank.addLinePointPairDependency(
        shapesBank.getUnifiedLineId((*iter)["lineId"]), shapesBank.getUnifiedPointId((*iter)["end1Id"]),
        shapesBank.getUnifiedPointId((*iter)["end2Id"]), IDependency::Type::MID_PERPENDICULAR_LINE, IDependency::Reason::USER_DEFINED,
        {}, IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addPolygonTypes(json types) {
  json::iterator iter;

  for (iter = types.begin(); iter != types.end(); ++iter) {
    dependenciesBank.addPolygonTypeDependency(shapesBank.getUnifiedPointsVector(parseUnsignedIntVector((*iter)["polygon"])),
                                              parsePolygonType((*iter)["type"]), IDependency::Type::POLYGON_TYPE,
                                              IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }
}

void Expert::addSpecialSegments(json medians, json altitudes, json midSegments) {
  json::iterator iter;

  for (iter = medians.begin(); iter != medians.end(); ++iter) {
    dependenciesBank.addPointPairsDependency(
        shapesBank.getUnifiedPointId((*iter)["s1End1Id"]), shapesBank.getUnifiedPointId((*iter)["s1End2Id"]),
        shapesBank.getUnifiedPointId((*iter)["s2End1Id"]), shapesBank.getUnifiedPointId((*iter)["s2End2Id"]),
        IDependency::Type::MEDIAN, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = altitudes.begin(); iter != altitudes.end(); ++iter) {
    dependenciesBank.addLinePointPairDependency(shapesBank.getUnifiedLineId((*iter)["lineId"]),
                                                shapesBank.getUnifiedPointId((*iter)["end1Id"]),
                                                shapesBank.getUnifiedPointId((*iter)["end2Id"]), IDependency::Type::ALTITUDE,
                                                IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }

  for (iter = midSegments.begin(); iter != midSegments.end(); ++iter) {
    dependenciesBank.addPointsPairsPairPointsPairDependency(
        shapesBank.getUnifiedPointId((*iter)["a1End1Id"]), shapesBank.getUnifiedPointId((*iter)["a1End2Id"]),
        shapesBank.getUnifiedPointId((*iter)["a2End1Id"]), shapesBank.getUnifiedPointId((*iter)["a2End2Id"]),
        shapesBank.getUnifiedPointId((*iter)["sEnd1"]), shapesBank.getUnifiedPointId((*iter)["sEnd2"]),
        IDependency::Type::MID_SEGMENT, IDependency::Reason::USER_DEFINED, {}, IDependency::UsefulnessLevel::HIGH);
  }
}

std::vector<unsigned int> Expert::parseUnsignedIntVector(json inputVector) {
  std::vector<unsigned int> result = {};

  json::iterator iter;
  for (iter = inputVector.begin(); iter != inputVector.end(); ++iter) {
    result.push_back(*iter);
  }

  return result;
}

LineType Expert::parseLineType(unsigned int lineType) {
  switch (lineType) {
    case VERTICAL_LINE_TYPE_ID:
      return LineType::VERTICAL;
    case HORIZONTAL_LINE_TYPE_ID:
      return LineType::HORIZONTAL;
    //case SLANTED_LINE_TYPE_ID:
    //	return LineType::SLANTED;
    default:
      return LineType::SLANTED;
  }
}

AngleType Expert::parseAngleType(unsigned int angleType) {
  switch (angleType) {
    case CONVEX_ANGLE_TYPE_ID:
      return AngleType::CONVEX;
    case CONCAVE_ANGLE_TYPE_ID:
      return AngleType::CONCAVE;
    default:
      return AngleType::UNKNOWN;
  }
}

PolygonType Expert::parsePolygonType(unsigned int polygonType) {
  switch (polygonType) {
    case ISOSCELES_ACUTE_TRIANGLE_POLYGON_TYPE_ID:
      return PolygonType::ISOSCELES_ACUTE_TRIANGLE;
    case EQUILATERAL_TRIANGLE_POLYGON_TYPE_ID:
      return PolygonType::EQUILATERAL_TRIANGLE;
    case SCALENE_RIGHT_TRIANGLE_POLYGON_TYPE_ID:
      return PolygonType::SCALENE_RIGHT_TRIANGLE;
    case ISOSCELES_RIGHT_TRIANGLE_POLYGON_TYPE_ID:
      return PolygonType::ISOSCELES_RIGHT_TRIANGLE;
    case OBTUSE_ISOSCELES_TRIANGLE_POLYGON_TYPE_ID:
      return PolygonType::OBTUSE_ISOSCELES_TRIANGLE;
    case SQUARE_POLYGON_TYPE_ID:
      return PolygonType::SQUARE;
    case RECTANGLE_POLYGON_TYPE_ID:
      return PolygonType::RECTANGLE;
    case REGULAR_POLYGON_TYPE_ID:
      return PolygonType::REGULAR_POLYGON;
    case PARALLELOGRAM_POLYGON_TYPE_ID:
      return PolygonType::PARALLELOGRAM;
    case KITE_POLYGON_TYPE_ID:
      return PolygonType::KITE;
    case RHOMBUS_POLYGON_TYPE_ID:
      return PolygonType::RHOMBUS;
    case SCALENE_TRAPEZOID_POLYGON_TYPE_ID:
      return PolygonType::SCALENE_TRAPEZOID;
    case ISOSCELES_TRAPEZOID_POLYGON_TYPE_ID:
      return PolygonType::ISOSCELES_TRAPEZOID;
    case RIGHT_TRAPEZOID_POLYGON_TYPE_ID:
      return PolygonType::RIGHT_TRAPEZOID;
    default:
      return PolygonType::UNKNOWN;
  }
}

void Expert::findIntersectionPointsOfLines() {
  std::vector<expertBackground::LineModel> lines = shapesBank.getLinesVector();

  for (unsigned int line1Id = 0; line1Id < (linesNumber < 1 ? 0 : (linesNumber - 1)); line1Id++) {
    for (unsigned int line2Id = line1Id + 1; line2Id < linesNumber; line2Id++) {
      const std::vector<unsigned int>& points1 = lines[line1Id].getIncludedPoints();
      const std::vector<unsigned int>& points2 = lines[line2Id].getIncludedPoints();

      for (const unsigned int point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(), [point1Id](unsigned int otherPoint) { return otherPoint == point1Id; })) {
          const unsigned int commonPointId = point1Id;
          intersectionPointsOfLines[line1Id][line2Id] = {commonPointId};
          intersectionPointsOfLines[line2Id][line1Id] = {commonPointId};
          pointsOnLinesIntersections[commonPointId].push_back({line1Id, line2Id});
          break;
        }
      }
    }
  }
}

void Expert::findIntersectionPointsOfCircles() {
  std::vector<expertBackground::CircleModel> circles = shapesBank.getCirclesVector();

  for (unsigned int circle1Id = 0; circle1Id < (circlesNumber < 1 ? 0 : (circlesNumber - 1)); circle1Id++) {
    for (unsigned int circle2Id = circle1Id + 1; circle2Id < circlesNumber; circle2Id++) {
      const std::vector<unsigned int>& points1 = circles[circle1Id].getIncludedPoints();
      const std::vector<unsigned int>& points2 = circles[circle2Id].getIncludedPoints();

      unsigned int counter = 0;
      for (const unsigned int point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(), [point1Id](unsigned int otherPoint) { return otherPoint == point1Id; })) {
          const unsigned int commonPointId = point1Id;
          intersectionPointsOfCircles[circle1Id][circle2Id].push_back(commonPointId);
          intersectionPointsOfCircles[circle2Id][circle1Id].push_back(commonPointId);
          pointsOnCirclesIntersections[commonPointId].push_back({circle1Id, circle2Id});
          counter++;
          if (counter == 2) {
            break;
          }
        }
      }
    }
  }
}

void Expert::findIntersectionPointsOfLinesCircles() {
  std::vector<expertBackground::LineModel> lines = shapesBank.getLinesVector();
  std::vector<expertBackground::CircleModel> circles = shapesBank.getCirclesVector();

  for (unsigned int lineId = 0; lineId < linesNumber; lineId++) {
    for (unsigned int circleId = 0; circleId < circlesNumber; circleId++) {
      const std::vector<unsigned int>& points1 = lines[lineId].getIncludedPoints();
      const std::vector<unsigned int>& points2 = circles[circleId].getIncludedPoints();

      unsigned int counter = 0;
      for (const unsigned int point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(), [point1Id](unsigned int otherPoint) { return otherPoint == point1Id; })) {
          const unsigned int commonPointId = point1Id;
          intersectionPointsOfLinesAndCircles[lineId][circleId].push_back(commonPointId);
          pointsOnLineAndCircleIntersections[commonPointId].push_back({lineId, circleId});
          counter++;
          if (counter == 2) {
            break;
          }
        }
      }
    }
  }
}

void Expert::checkPointsOnShapes() {
  std::vector<unsigned int> pointsOn;

  for (unsigned int lineId = 0; lineId < linesNumber; lineId++) {
    pointsOn = shapesBank.getLine(lineId).getIncludedPoints();
    for (const unsigned int pointId : pointsOn) {
      pointsOnShapes[pointId][0] = true;
    }
  }

  for (unsigned int circleId = 0; circleId < circlesNumber; circleId++) {
    pointsOn = shapesBank.getCircle(circleId).getIncludedPoints();
    for (const unsigned int pointId : pointsOn) {
      pointsOnShapes[pointId][0] = true;
    }
  }
}

json Expert::getIntersectionPointsAsJson() {
  return {{"line_line", json(intersectionPointsOfLines)},
          {"circle_circle", json(intersectionPointsOfCircles)},
          {"line_circle", json(intersectionPointsOfLinesAndCircles)},
          {"points_on_line_line", json(pointsOnLinesIntersections)},
          {"points_on_circle_circle", json(pointsOnCirclesIntersections)},
          {"points_on_line_circle", json(pointsOnLineAndCircleIntersections)}};
}

}  // namespace expert