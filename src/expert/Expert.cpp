#include "Expert.h"

namespace expert {
Expert::Expert() : shapesBank{}, dependenciesBank{&shapesBank}, heuristicsBank{&shapesBank, &dependenciesBank} { }

void Expert::importTask(json task, std::ostream& stream) {
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

    shapesBank.addCircle((*iter)["id"], (*iter)["centerId"], (*iter)["cx"], (*iter)["cy"], centerName, (*iter)["r"], (*iter)["pointsOn"]);
  }

  shapesBank.findIntersectionPointsOfLines();
  shapesBank.findIntersectionPointsOfCircles();
  shapesBank.findIntersectionPointsOfLinesCircles();
  shapesBank.findPointsOnShapes();

  dependenciesBank.initializeBaseVariables();

  initializeBaseValues();
  addValues(task["segmentLengths"], task["angleMeasures"], task["formulas"], task["perimeters"], task["areas"]);
  addLinesDependencies(task["perpendicularLines"], task["parallelLines"]);
  addEqualismDependencies(task["equalSegments"], task["equalAngles"]);
  addTangentLinesAndCircles(task["tangentLines"], task["tangentCircles"]);
  addCirclePolygonDependencies(task["inscribedCircles"], task["circumscribedCircles"], task["escribedCircles"]);
  addBisectors(task["bisectors"]);
  addMidPerpendiculars(task["midPerpendiculars"]);
  addPolygonTypes(task["polygonTypes"]);
  addSpecialSegments(task["medians"], task["altitudes"], task["midSegments"]);

  heuristicsBank.initializeSchemeGraph();
  heuristicsBank.initializeVariablesGraph();

  localTimer.takeMeasurement("Importing and parsing data", false, stream);
}

json Expert::exportSolution(std::ostream& stream) {
  localTimer.startMeasurement();

  json output = {{"points", shapesBank.getPointsAsJsonObjects()},
                 {"lines", shapesBank.getLinesAsJsonObjects()},
                 {"circles", shapesBank.getCirclesAsJsonObjects()},
                 {"intersections", shapesBank.getIntersectionPointsAsJson()},
                 {"indexes_of_variables", dependenciesBank.getVariablesIndexesAsJsonObject()},
                 {"dependencies", dependenciesBank.getDependenciesAsJsonObjects()}};

  localTimer.takeMeasurement("Exporting data", false, stream);

  return output;
}

void Expert::useKnowledge(std::ostream& stream) {
  unsigned int allDependenciesFoundNumber{0};
  unsigned int newDependenciesFoundNumber;
  unsigned int roundsCounter{1};

  do {
    localTimer.startMeasurement();

    newDependenciesFoundNumber = 0;

    newDependenciesFoundNumber += dependenciesBank.extractVariables();

    newDependenciesFoundNumber += explorePolygonTypeBasedDependencies();
    newDependenciesFoundNumber += exploreSpecificSegmentsBasedDependencies();
    newDependenciesFoundNumber += exploreTangentLineAndCircleBasedDependencies();
    newDependenciesFoundNumber += explorePolygonCircleBasedDependencies();
    newDependenciesFoundNumber += exploreSpecificLineBasedDependencies();
    newDependenciesFoundNumber += exploreLineBasedDependencies();
    newDependenciesFoundNumber += exploreAngleBasedTheorems();
    newDependenciesFoundNumber += explorePolygonsTheorems();

    allDependenciesFoundNumber += newDependenciesFoundNumber;
    heuristicsBank.clearAllFlags();

    localTimer.takeMeasurement("Round number " + std::to_string(roundsCounter) + " ended. Found " +
                               std::to_string(newDependenciesFoundNumber) + " new dependencies!",
                               false, stream);
    roundsCounter++;
  }
  while(newDependenciesFoundNumber > 0);
}

void Expert::initializeBaseValues() {
  const std::vector<PointModel>& points = shapesBank.getPointsVector();

  for (size_t pointPos = 0; pointPos < points.size(); pointPos++) {
    dependenciesBank.addLength(points.at(pointPos).getId(), points.at(pointPos).getId(),
                               symbolicAlgebra::Expression("0"), IDependency::Reason::POINTS_ARE_THE_SAME,
                               {EXERCISE_DESCRIPTION_ID}, IDependency::ImportanceLevel::LOW);

    dependenciesBank.addConvexAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(),
                                    symbolicAlgebra::Expression("0"), IDependency::Reason::POINTS_ARE_THE_SAME,
                                    {EXERCISE_DESCRIPTION_ID}, IDependency::ImportanceLevel::LOW);

    dependenciesBank.addConcaveAngle(points.at(pointPos).getId(), points.at(pointPos).getId(), points.at(pointPos).getId(),
                                     symbolicAlgebra::Expression("360"), IDependency::Reason::POINTS_ARE_THE_SAME,
                                     {EXERCISE_DESCRIPTION_ID}, IDependency::ImportanceLevel::LOW);
  }

  for (size_t point1Pos = 0; point1Pos < points.size(); point1Pos++) {
    for (size_t point2Pos = 0; point2Pos < points.size(); point2Pos++) {
      if (point1Pos != point2Pos) {
        dependenciesBank.addConvexAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                                        symbolicAlgebra::Expression("0"), IDependency::Reason::ARMS_ARE_THE_SAME,
                                        {EXERCISE_DESCRIPTION_ID}, IDependency::ImportanceLevel::LOW);

        dependenciesBank.addConcaveAngle(points.at(point2Pos).getId(), points.at(point1Pos).getId(), points.at(point2Pos).getId(),
                                         symbolicAlgebra::Expression("360"), IDependency::Reason::ARMS_ARE_THE_SAME,
                                         {EXERCISE_DESCRIPTION_ID}, IDependency::ImportanceLevel::LOW);
      }
    }
  }
}

void Expert::addValues(json lengths, json angleValues, json formulas, json perimeters, json areas) {
  json::iterator iter;

  for (iter = lengths.begin(); iter != lengths.end(); ++iter) {
    dependenciesBank.addLength((*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                               ExpressionModel(static_cast<std::string>((*iter)["length"])),
                               IDependency::Reason::USER_DEFINED, {EXERCISE_DESCRIPTION_ID},
                               IDependency::ImportanceLevel::HIGH);
  }

  for (iter = angleValues.begin(); iter != angleValues.end(); ++iter) {
    if ((*iter)["angleIsConvex"]) {
      dependenciesBank.addConvexAngle((*iter)["angleEnd1Id"], (*iter)["angleVertexId"], (*iter)["angleEnd2Id"],
                                       ExpressionModel(static_cast<std::string>((*iter)["measure"])),
                                       IDependency::Reason::USER_DEFINED,
                                       {EXERCISE_DESCRIPTION_ID},
                                       IDependency::ImportanceLevel::HIGH);
    }
    else {
      dependenciesBank.addConcaveAngle((*iter)["angleEnd1Id"], (*iter)["angleVertexId"], (*iter)["angleEnd2Id"],
                                      ExpressionModel(static_cast<std::string>((*iter)["measure"])),
                                      IDependency::Reason::USER_DEFINED,
                                      {EXERCISE_DESCRIPTION_ID},
                                      IDependency::ImportanceLevel::HIGH);
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

    dependenciesBank.addEquation(symbolicAlgebra::Expression(leftSide),
                                 symbolicAlgebra::Expression(rightSide),
                                 IDependency::Reason::USER_DEFINED,
                                 {EXERCISE_DESCRIPTION_ID},
                                 IDependency::ImportanceLevel::HIGH);
  }

  for (iter = perimeters.begin(); iter != perimeters.end(); ++iter) {
    dependenciesBank.addPolygonExpressionDependency((*iter)["polygonVerticesIds"], false,
                                                    symbolicAlgebra::Expression{(*iter)["perimeter"]},
                                                    PolygonExpressionDependencies::POLYGON_PERIMETER, 
                                                    IDependency::Reason::USER_DEFINED,
                                                    {EXERCISE_DESCRIPTION_ID},
                                                    IDependency::ImportanceLevel::HIGH);
  }

  for (iter = areas.begin(); iter != areas.end(); ++iter) {
    dependenciesBank.addPolygonExpressionDependency((*iter)["polygonVerticesIds"], false,
                                                    symbolicAlgebra::Expression{(*iter)["area"]},
                                                    PolygonExpressionDependencies::POLYGON_AREA, 
                                                    IDependency::Reason::USER_DEFINED,
                                                    {EXERCISE_DESCRIPTION_ID},
                                                    IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addLinesDependencies(json perpendicular, json parallel) {
  json::iterator iter;

  for (iter = perpendicular.begin(); iter != perpendicular.end(); ++iter) {
    dependenciesBank.addLinesDependency((*iter)["line1Id"], (*iter)["line2Id"],
                                        LinesDependencies::PERPENDICULAR_LINES,
                                        IDependency::Reason::USER_DEFINED,
                                        {EXERCISE_DESCRIPTION_ID},
                                        IDependency::ImportanceLevel::HIGH);
  }

  for (iter = parallel.begin(); iter != parallel.end(); ++iter) {
    dependenciesBank.addLinesDependency((*iter)["line1Id"], (*iter)["line2Id"],
                                        LinesDependencies::PARALLEL_LINES,
                                        IDependency::Reason::USER_DEFINED,
                                        {EXERCISE_DESCRIPTION_ID},
                                        IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addEqualismDependencies(json segments, json angles) {
  json::iterator iter;

  for (iter = segments.begin(); iter != segments.end(); ++iter) {
    setEqualSides((*iter)["segment1End1Id"], (*iter)["segment1End2Id"],
                  (*iter)["segment2End1Id"], (*iter)["segment2End2Id"],
                  IDependency::Reason::USER_DEFINED, {EXERCISE_DESCRIPTION_ID},
                  IDependency::ImportanceLevel::HIGH);
  }

  for (iter = angles.begin(); iter != angles.end(); ++iter) {
    setEqualAngles((*iter)["angle1End1Id"], (*iter)["angle1VertexId"], (*iter)["angle1End2Id"],
                   (*iter)["angle2End1Id"], (*iter)["angle2VertexId"], (*iter)["angle2End2Id"], AngleType::UNKNOWN,
                   IDependency::Reason::USER_DEFINED,{EXERCISE_DESCRIPTION_ID},
                   IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addTangentLinesAndCircles(json lines, json circles) {
  json::iterator iter;

  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    dependenciesBank.addLineCircleDependency((*iter)["lineId"],
                                             (*iter)["circleId"],
                                             LineCircleDependencies::TANGENT_LINE_TO_CIRCLE,
                                             IDependency::Reason::USER_DEFINED,
                                             {EXERCISE_DESCRIPTION_ID},
                                             IDependency::ImportanceLevel::HIGH);
  }

  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    dependenciesBank.addCirclesDependency((*iter)["circle1Id"],
                                          (*iter)["circle2Id"],
                                          CirclesDependencies::TANGENT_CIRCLE_TO_CIRCLE,
                                          IDependency::Reason::USER_DEFINED,
                                          {EXERCISE_DESCRIPTION_ID},
                                          IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addCirclePolygonDependencies(json inscribed, json circumscribed, json escribed) {
  json::iterator iter;

  for (iter = inscribed.begin(); iter != inscribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency((*iter)["circleId"],
                                                (*iter)["polygonVerticesIds"],
                                                CirclePolygonDependencies::INSCRIBED_CIRCLE,
                                                IDependency::Reason::USER_DEFINED,
                                                {EXERCISE_DESCRIPTION_ID},
                                                IDependency::ImportanceLevel::HIGH);
  }

  for (iter = circumscribed.begin(); iter != circumscribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency((*iter)["circleId"],
                                                (*iter)["polygonVerticesIds"],
                                                CirclePolygonDependencies::CIRCUMSCRIBED_CIRCLE,
                                                IDependency::Reason::USER_DEFINED,
                                                {EXERCISE_DESCRIPTION_ID},
                                                IDependency::ImportanceLevel::HIGH);
  }

  for (iter = escribed.begin(); iter != escribed.end(); ++iter) {
    dependenciesBank.addCirclePolygonDependency((*iter)["circleId"],
                                                (*iter)["polygonVerticesIds"],
                                                CirclePolygonDependencies ::ESCRIBED_CIRCLE,
                                                IDependency::Reason::USER_DEFINED,
                                                {EXERCISE_DESCRIPTION_ID},
                                                IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addBisectors(json bisectorLines) {
  json::iterator iter;

  for (iter = bisectorLines.begin(); iter != bisectorLines.end(); ++iter) {
    dependenciesBank.addLineAngleDependency((*iter)["lineId"],
                                            (*iter)["angleEnd1Id"], (*iter)["angleVertexId"], (*iter)["angleEnd2Id"],parseAngleType((*iter)["angleType"]),
                                            LineAngleDependencies::BISECTOR_LINE,
                                            IDependency::Reason::USER_DEFINED,
                                            {EXERCISE_DESCRIPTION_ID},
                                            IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addMidPerpendiculars(json midPerpendicularLines) {
  json::iterator iter;

  for (iter = midPerpendicularLines.begin(); iter != midPerpendicularLines.end(); ++iter) {
    dependenciesBank.addLinePointsPairDependency((*iter)["lineId"],
                                                 (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                 LinePointsPairDependencies::MID_PERPENDICULAR_LINE,
                                                 IDependency::Reason::USER_DEFINED,
                                                 {EXERCISE_DESCRIPTION_ID},
                                                 IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addPolygonTypes(json types) {
  json::iterator iter;

  for (iter = types.begin(); iter != types.end(); ++iter) {
    dependenciesBank.addPolygonTypeDependency((*iter)["polygonVerticesIds"],
                                              (*iter)["polygonType"],
                                              PolygonTypeDependencies::POLYGON_TYPE,
                                              IDependency::Reason::USER_DEFINED,
                                              {EXERCISE_DESCRIPTION_ID},
                                              IDependency::ImportanceLevel::HIGH);
  }
}

void Expert::addSpecialSegments(json medians, json altitudes, json midSegments) {
  json::iterator iter;

  for (iter = medians.begin(); iter != medians.end(); ++iter) {
    dependenciesBank.addPolygonPointsPairDependency((*iter)["polygonVerticesIds"], true,
                                                    (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                    PolygonPointsPairDependencies::MEDIAN,
                                                    IDependency::Reason::USER_DEFINED,
                                                    {EXERCISE_DESCRIPTION_ID},
                                                    IDependency::ImportanceLevel::HIGH);
  }

  for (iter = altitudes.begin(); iter != altitudes.end(); ++iter) {
    dependenciesBank.addPolygonPointsPairDependency((*iter)["polygonVerticesIds"], true,
                                                    (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                    PolygonPointsPairDependencies::ALTITUDE,
                                                    IDependency::Reason::USER_DEFINED,
                                                    {EXERCISE_DESCRIPTION_ID},
                                                    IDependency::ImportanceLevel::HIGH);
  }

  for (iter = midSegments.begin(); iter != midSegments.end(); ++iter) {
    dependenciesBank.addPolygonPointsPairDependency((*iter)["polygonVerticesIds"], true,
                                                    (*iter)["segmentEnd1Id"], (*iter)["segmentEnd2Id"],
                                                    PolygonPointsPairDependencies::MID_SEGMENT,
                                                    IDependency::Reason::USER_DEFINED,
                                                    {EXERCISE_DESCRIPTION_ID},
                                                    IDependency::ImportanceLevel::HIGH);
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
    //case UNKNOWN_ANGLE_TYPE_ID:
    //  return AngleType::UNKNOWN;
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

}  // namespace expert