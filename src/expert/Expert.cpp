#include "Expert.h"

namespace expert {
Expert::Expert() { }

void Expert::importTask(json task) {
  localTimer.startMeasurement();

  json::iterator iter;
  json points = task["points"];
  json circles = task["circles"];
  json lines = task["lines"];

  for (iter = points.begin(); iter != points.end(); ++iter) {
    shapesBank.addPoint((*iter)["id"], (*iter)["x"], (*iter)["y"], (*iter)["name"]);
  }

  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    shapesBank.addLine((*iter)["id"], parseLineType((*iter)["type"]), (*iter)["a"], (*iter)["b"], (*iter)["pointsOn"]);
  }

  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    const std::string centerName = shapesBank.getPoint((*iter)["centerId"]).getName();

    shapesBank.addCircle((*iter)["id"], (*iter)["centerId"], (*iter)["r"], (*iter)["pointsOn"]);
  }

  schemeGraph = expertBackground::Graph(shapesBank.getPointsNumber());
  for(const auto& line: shapesBank.getLinesVector()) {
    const auto& includedPoints = line.getIncludedPoints();
    for (size_t firstPointOnLineId = 0; firstPointOnLineId < includedPoints.size(); firstPointOnLineId++) {
      for (size_t secondPointOnLineId = firstPointOnLineId + 1; secondPointOnLineId < includedPoints.size(); secondPointOnLineId++) {
        schemeGraph.addEdge(
            shapesBank.getPointPositionInVector(includedPoints[firstPointOnLineId]),
            shapesBank.getPointPositionInVector(includedPoints[secondPointOnLineId]));
      }
    }
  }

  const size_t pointsNumber{shapesBank.getPointsNumber()};
  const size_t linesNumber{shapesBank.getLinesNumber()};
  const size_t circlesNumber{shapesBank.getCirclesNumber()};

  intersectionPointsOfLines.resize(linesNumber, std::vector<std::vector<size_t>>(linesNumber));
  intersectionPointsOfLinesAndCircles.resize(linesNumber, std::vector<std::vector<size_t>>(circlesNumber));
  intersectionPointsOfCircles.resize(circlesNumber, std::vector<std::vector<size_t>>(circlesNumber));

  pointsOnLinesIntersections.resize(pointsNumber, {});
  pointsOnCirclesIntersections.resize(pointsNumber, {});
  pointsOnLineAndCircleIntersections.resize(pointsNumber, {});

  pointsOnShapes.resize(pointsNumber, {false, false});

  findIntersectionPointsOfLines();
  findIntersectionPointsOfCircles();
  findIntersectionPointsOfLinesCircles();
  checkPointsOnShapes();

  dependenciesBank = expertBackground::DependenciesBank(&shapesBank);

  addValues(task["segmentLengths"], task["angleMeasures"], task["formulas"], task["perimeters"], task["areas"]);
  addLinesDependencies(task["perpendicularLines"], task["parallelLines"]);
  addEqualismDependencies(task["equalSegments"], task["equalAngles"]);
  addTangentLinesAndCircles(task["tangentLines"], task["tangentCircles"]);
  addCirclePolygonDependencies(task["inscribedCircles"], task["circumscribedCircles"], task["escribedCircles"]);
  addBisectors(task["bisectors"]);
  addMidPerpendiculars(task["midPerpendiculars"]);
  addPolygonTypes(task["polygonTypes"]);
  addSpecialSegments(task["medians"], task["altitudes"], task["midSegments"]);

  localTimer.takeMeasurement("Importing and parsing data", false, std::cout);
}

json Expert::exportSolution() {
  localTimer.startMeasurement();

  json output = {{"points", shapesBank.getPointsJson()},
                 {"lines", shapesBank.getLinesJson()},
                 {"circles", shapesBank.getCirclesJson()},
                 {"intersections", getIntersectionPointsJson()},
                 {"indexes_of_variables", dependenciesBank.getVariablesIndexesJson()},
                 {"dependencies", dependenciesBank.getDependenciesJson()}};

  localTimer.takeMeasurement("Exporting data", false, std::cout);

  return output;
}

void Expert::useKnowledge() {
  const size_t dependenciesFoundNumber = 0;

  localTimer.startMeasurement();

  /*
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
  */
  localTimer.takeMeasurement("Using knowledge ended!\nFound " + std::to_string(dependenciesFoundNumber) + " new dependencies!",
                             false, std::cout);
}

void Expert::addValues(json lengths, json angleValues, json formulas, json perimeters, json areas) {
  json::iterator iter;

  for (iter = lengths.begin(); iter != lengths.end(); ++iter) {
    dependenciesBank.addLength((*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                               expertBackground::ExpressionModel(static_cast<std::string>((*iter)["length"])),
                               IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }

  for (iter = angleValues.begin(); iter != angleValues.end(); ++iter) {
    if (parseAngleType((*iter)["angleIsConvex"]) == AngleType::CONCAVE) {
      dependenciesBank.addConcaveAngle((*iter)["angleEnd1Id"], (*iter)["angleVertexId"], (*iter)["angleEnd2Id"],
                                       expertBackground::ExpressionModel(static_cast<std::string>((*iter)["measure"])),
                                       IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
    }
    else if (parseAngleType((*iter)["angleIsConvex"]) == AngleType::CONVEX) {
      dependenciesBank.addConvexAngle((*iter)["angleEnd1Id"], (*iter)["angleVertexId"], (*iter)["angleEnd2Id"],
                                      expertBackground::ExpressionModel(static_cast<std::string>((*iter)["measure"])),
                                      IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
    }
  }

  for (iter = formulas.begin(); iter != formulas.end(); ++iter) {
    const std::string formula{*iter};

    bool equalSignOccur = false;
    size_t equalSignIndex{0};
    for(size_t index = 0; index < formula.size(); index++) {
      if(formula.at(index) == '=') {
        equalSignOccur = true;
        equalSignIndex = index;
      }
    }

    if(!equalSignOccur) {
      continue;
    }

    std::string leftSide{};
    std::string rightSide{};

    for(size_t index = 0; index < equalSignIndex; index++) {
      leftSide += formula.at(index);
    }
    
    for(size_t index = equalSignIndex + 1; index < formula.size(); index++) {
      rightSide += formula.at(index);
    }

    dependenciesBank.addEquation(symbolicAlgebra::Expression(leftSide), symbolicAlgebra::Expression(rightSide),
                                 IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }

  for (iter = perimeters.begin(); iter != perimeters.end(); ++iter) {
    dependenciesBank.addPolygonExpressionDependency((*iter)["polygonVerticesIds"], false, symbolicAlgebra::Expression{(*iter)["perimeter"]},
                                                    PolygonExpressionDependencies::POLYGON_PERIMETER, 
                                                    IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }

  for (iter = areas.begin(); iter != areas.end(); ++iter) {
    dependenciesBank.addPolygonExpressionDependency((*iter)["polygonVerticesIds"], false, symbolicAlgebra::Expression{(*iter)["area"]},
                                                    PolygonExpressionDependencies::POLYGON_AREA, 
                                                    IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }
}

void Expert::addLinesDependencies(json perpendicular, json parallel) {
  json::iterator iter;

  for (iter = perpendicular.begin(); iter != perpendicular.end(); ++iter) {
    dependenciesBank.addLinesDependency((*iter)["line1Id"], (*iter)["line2Id"], LinesDependencies::PERPENDICULAR_LINES,
                                        IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }

  for (iter = parallel.begin(); iter != parallel.end(); ++iter) {
    dependenciesBank.addLinesDependency((*iter)["line1Id"], (*iter)["line2Id"], LinesDependencies::PARALLEL_LINES,
                                        IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }
}

void Expert::addEqualismDependencies(json segments, json angles) {
  json::iterator iter;

  for (iter = segments.begin(); iter != segments.end(); ++iter) {
    dependenciesBank.addPointsPairsDependency((*iter)["segment1End1Id"], (*iter)["segment1End2Id"], (*iter)["segment2End1Id"], (*iter)["segment2End2Id"],
                                              PointsPairsDependencies::EQUAL_SEGMENTS, IDependency::Reason::USER_DEFINED,
                                              {}, IDependency::Importance::HIGH);
  }

  for (iter = angles.begin(); iter != angles.end(); ++iter) {
    dependenciesBank.addAnglesDependency((*iter)["angle1End1Id"], (*iter)["angle1VertexId"], (*iter)["angle1End2Id"], AngleType::CONVEX,
                                         (*iter)["angle2End1Id"], (*iter)["angle2VertexId"], (*iter)["angle2End2Id"], AngleType::CONVEX,
                                         AnglesDependencies::EQUAL_ANGLES, IDependency::Reason::USER_DEFINED, {},
                                         IDependency::Importance::HIGH);

    dependenciesBank.addAnglesDependency((*iter)["angle1End1Id"], (*iter)["angle1VertexId"], (*iter)["angle1End2Id"], AngleType::CONCAVE,
                                         (*iter)["angle2End1Id"], (*iter)["angle2VertexId"], (*iter)["angle2End2Id"], AngleType::CONCAVE,
                                         AnglesDependencies::EQUAL_ANGLES, IDependency::Reason::USER_DEFINED, {},
                                         IDependency::Importance::HIGH);
  }
}

void Expert::addTangentLinesAndCircles(json lines, json circles) {
  json::iterator iter;

  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    dependenciesBank.addLineCircleDependency((*iter)["lineId"], (*iter)["circleId"],
                                             LineCircleDependencies::TANGENT_LINE_TO_CIRCLE, IDependency::Reason::USER_DEFINED,
                                             {}, IDependency::Importance::HIGH);
  }

  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    dependenciesBank.addCirclesDependency((*iter)["circle1Id"], (*iter)["circle2Id"], CirclesDependencies::TANGENT_CIRCLE_TO_CIRCLE,
                                          IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }
}

void Expert::addCirclePolygonDependencies(json inscribed, json circumscribed, json escribed) {
  json::iterator iter;

  for (iter = inscribed.begin(); iter != inscribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency((*iter)["circleId"], (*iter)["polygonVerticesIds"],
                                                CirclePolygonDependencies::INSCRIBED_CIRCLE, IDependency::Reason::USER_DEFINED,
                                                {}, IDependency::Importance::HIGH);
  }

  for (iter = circumscribed.begin(); iter != circumscribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency((*iter)["circleId"], (*iter)["polygonVerticesIds"],
                                                CirclePolygonDependencies::CIRCUMSCRIBED_CIRCLE, IDependency::Reason::USER_DEFINED,
                                                {}, IDependency::Importance::HIGH);
  }

  for (iter = escribed.begin(); iter != escribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency((*iter)["circleId"], (*iter)["polygonVerticesIds"],
                                                CirclePolygonDependencies ::ESCRIBED_CIRCLE, IDependency::Reason::USER_DEFINED,
                                                {}, IDependency::Importance::HIGH);
  }
}

void Expert::addBisectors(json bisectorLines) {
  json::iterator iter;

  for (iter = bisectorLines.begin(); iter != bisectorLines.end(); ++iter) {
    dependenciesBank.addLineAngleDependency((*iter)["lineId"], (*iter)["angleEnd1Id"], (*iter)["angleVertexId"], (*iter)["angleEnd2Id"],
                                            parseAngleType((*iter)["angleType"]), LineAngleDependencies::BISECTOR_LINE,
                                            IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }
}

void Expert::addMidPerpendiculars(json midPerpendicularLines) {
  json::iterator iter;

  for (iter = midPerpendicularLines.begin(); iter != midPerpendicularLines.end(); ++iter) {
    dependenciesBank.addLinePointsPairDependency((*iter)["lineId"], (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                 LinePointsPairDependencies::MID_PERPENDICULAR_LINE,
                                                 IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }
}

void Expert::addPolygonTypes(json types) {
  json::iterator iter;

  for (iter = types.begin(); iter != types.end(); ++iter) {
    dependenciesBank.addPolygonTypeDependency((*iter)["polygonVerticesIds"], (*iter)["polygonType"], PolygonTypeDependencies::POLYGON_TYPE,
                                              IDependency::Reason::USER_DEFINED, {}, IDependency::Importance::HIGH);
  }
}

void Expert::addSpecialSegments(json medians, json altitudes, json midSegments) {
  json::iterator iter;

  for (iter = medians.begin(); iter != medians.end(); ++iter) {
    dependenciesBank.addPolygonPointsPairDependency((*iter)["polygonVerticesIds"], true, (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                              PolygonPointsPairDependencies::MEDIAN, IDependency::Reason::USER_DEFINED, {},
                                              IDependency::Importance::HIGH);
  }

  for (iter = altitudes.begin(); iter != altitudes.end(); ++iter) {
    dependenciesBank.addPolygonPointsPairDependency((*iter)["polygonVerticesIds"], true, (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                 PolygonPointsPairDependencies::ALTITUDE, IDependency::Reason::USER_DEFINED,
                                                 {}, IDependency::Importance::HIGH);
  }

  for (iter = midSegments.begin(); iter != midSegments.end(); ++iter) {
    dependenciesBank.addPolygonPointsPairDependency((*iter)["polygonVerticesIds"], true, (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                    PolygonPointsPairDependencies::MID_SEGMENT,
                                                    IDependency::Reason::USER_DEFINED, {},
                                                    IDependency::Importance::HIGH);
  }
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
  const std::vector<expertBackground::LineModel>& lines = shapesBank.getLinesVector();
  const size_t linesNumber{lines.size()};
  
  for (size_t line1Id = 0; line1Id < (linesNumber < 1 ? 0 : (linesNumber - 1)); line1Id++) {
    for (size_t line2Id = line1Id + 1; line2Id < linesNumber; line2Id++) {
      const std::vector<std::string>& points1 = lines[line1Id].getIncludedPoints();
      const std::vector<std::string>& points2 = lines[line2Id].getIncludedPoints();

      for (const std::string& point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(), [point1Id](const std::string& otherPoint) { return otherPoint == point1Id; })) {
          const std::string& commonPointId = point1Id;
          intersectionPointsOfLines[line1Id][line2Id].push_back(shapesBank.getPointPositionInVector(commonPointId));
          intersectionPointsOfLines[line2Id][line1Id].push_back(shapesBank.getPointPositionInVector(commonPointId));
          pointsOnLinesIntersections[shapesBank.getPointPositionInVector(commonPointId)].push_back({
              line1Id, line2Id
          });
          break;
        }
      }
    }
  }
}

void Expert::findIntersectionPointsOfCircles() {
  const std::vector<expertBackground::CircleModel>& circles = shapesBank.getCirclesVector();
  const size_t circlesNumber{circles.size()};

  for (size_t circle1Id = 0; circle1Id < (circlesNumber < 1 ? 0 : (circlesNumber - 1)); circle1Id++) {
    for (size_t circle2Id = circle1Id + 1; circle2Id < circlesNumber; circle2Id++) {
      const std::vector<std::string>& points1 = circles[circle1Id].getIncludedPoints();
      const std::vector<std::string>& points2 = circles[circle2Id].getIncludedPoints();

      size_t counter = 0;
      for (const std::string& point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(), [point1Id](const std::string& otherPoint) { return otherPoint == point1Id; })) {
          const std::string& commonPointId = point1Id;
          intersectionPointsOfCircles[circle1Id][circle2Id].push_back(shapesBank.getPointPositionInVector(commonPointId));
          intersectionPointsOfCircles[circle2Id][circle1Id].push_back(shapesBank.getPointPositionInVector(commonPointId));
          pointsOnCirclesIntersections[shapesBank.getPointPositionInVector(commonPointId)].push_back({
              circle1Id, circle2Id
          });
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
  const std::vector<expertBackground::LineModel>& lines = shapesBank.getLinesVector();
  const std::vector<expertBackground::CircleModel>& circles = shapesBank.getCirclesVector();
  const size_t linesNumber{lines.size()};
  const size_t circlesNumber{circles.size()};

  for (size_t lineId = 0; lineId < linesNumber; lineId++) {
    for (size_t circleId = 0; circleId < circlesNumber; circleId++) {
      const std::vector<std::string>& points1 = lines[lineId].getIncludedPoints();
      const std::vector<std::string>& points2 = circles[circleId].getIncludedPoints();

      size_t counter = 0;
      for (const std::string& point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(), [point1Id](const std::string& otherPoint) { return otherPoint == point1Id; })) {
          const std::string& commonPointId = point1Id;
          intersectionPointsOfLinesAndCircles[lineId][circleId].push_back(shapesBank.getPointPositionInVector(commonPointId));
          pointsOnLineAndCircleIntersections[shapesBank.getPointPositionInVector(commonPointId)].push_back({
              lineId, circleId
          });
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
  for (const expertBackground::LineModel& line: shapesBank.getLinesVector()) {
    for (const std::string& pointId: line.getIncludedPoints()) {
      pointsOnShapes[shapesBank.getPointPositionInVector(pointId)][0] = true;
    }
  }

  for (const expertBackground::CircleModel& circle: shapesBank.getCirclesVector()) {
    for (const std::string& pointId: circle.getIncludedPoints()) {
      pointsOnShapes[shapesBank.getPointPositionInVector(pointId)][0] = true;
    }
  }
}

json Expert::getIntersectionPointsJson() {
  return {{"line_line", json(intersectionPointsOfLines)},
          {"circle_circle", json(intersectionPointsOfCircles)},
          {"line_circle", json(intersectionPointsOfLinesAndCircles)},
          {"points_on_line_line", json(pointsOnLinesIntersections)},
          {"points_on_circle_circle", json(pointsOnCirclesIntersections)},
          {"points_on_line_circle", json(pointsOnLineAndCircleIntersections)}};
}

}  // namespace expert