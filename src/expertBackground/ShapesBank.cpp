#include "ShapesBank.h"
#include "HeuristicsBank.h"
#include <iostream>

namespace expertBackground {
ShapesBank::ShapesBank()
    : pointIdCounter{0},
      lineIdCounter{0},
      circleIdCounter{0},
      points{},
      circles{},
      pointIdsConverter{},
      lineIdsConverter{},
      circleIdsConverter{},
      heuristicsBank{nullptr} { }

void ShapesBank::addPoint(const std::string& identifier, float xCoordinate, float yCoordinate, const std::string& name) {
  points.emplace_back(identifier, xCoordinate, yCoordinate, name);
  pointIdsConverter.insert(std::make_pair(identifier, pointIdCounter));
  pointIdCounter++;

  if(heuristicsBank != nullptr) {
    heuristicsBank->markNewPointsFlag();
  }
}

json ShapesBank::getPointsAsJsonObjects() const {
  json result;
  unsigned int idCounter = 0;

  std::vector<PointModel>::const_iterator iter;
  for (iter = points.begin(); iter != points.end(); ++iter) {
    result.push_back({{"position", idCounter}, {"object", iter->getJsonObject()}});
    idCounter++;
  }

  return result;
}

void ShapesBank::addLine(const std::string& identifier, LineModel::LineType lineType, float lineA, float lineB,
                         const std::vector<std::string>& includedPoints) {
  std::vector<std::string> includedPointsCopy{includedPoints};
  const unsigned int includedPointsLength = includedPointsCopy.size();

  if (lineType == LineModel::LineType::VERTICAL) {
    for (unsigned int point1Id = 0; point1Id < includedPointsLength; point1Id++) {
      for (unsigned int point2Id = point1Id; point2Id < includedPointsLength; point2Id++) {
        const PointModel point1 = points[pointIdsConverter.at(includedPointsCopy[point1Id])];
        const PointModel point2 = points[pointIdsConverter.at(includedPointsCopy[point2Id])];

        if (point1.getY() > point2.getY()) {
          std::swap(includedPointsCopy[point1Id], includedPointsCopy[point2Id]);
        }
      }
    }
  }
  else {
    for (unsigned int point1Id = 0; point1Id < includedPointsLength; point1Id++) {
      for (unsigned int point2Id = point1Id; point2Id < includedPointsLength; point2Id++) {
        const PointModel point1 = points[pointIdsConverter[includedPointsCopy[point1Id]]];
        const PointModel point2 = points[pointIdsConverter[includedPointsCopy[point2Id]]];

        if (point1.getX() > point2.getX()) {
          std::swap(includedPointsCopy[point1Id], includedPointsCopy[point2Id]);
        }
      }
    }
  }

  lines.emplace_back(identifier, lineType, lineA, lineB, includedPointsCopy);
  lineIdsConverter.insert(std::make_pair(identifier, lineIdCounter));
  lineIdCounter++;

  if(heuristicsBank != nullptr) {
    heuristicsBank->markNewlinesFlag();
  }
}

std::string ShapesBank::getLineIdThrowTwoPoints(const std::string& point1Id, const std::string& point2Id) const {
  for (unsigned int lineId = 0; lineId < lines.size(); lineId++) {
    std::vector<std::string> linePoints = lines[lineId].getIncludedPoints();
    if (std::find(linePoints.begin(), linePoints.end(), point1Id) != linePoints.end() &&
        std::find(linePoints.begin(), linePoints.end(), point2Id) != linePoints.end()) {

      return lines[lineId].getId();
    }
  }

  throw std::invalid_argument("Does not exist line which is throw given 2 points!");
}

json ShapesBank::getLinesAsJsonObjects() const {
  json result;
  unsigned int idCounter = 0;

  std::vector<LineModel>::const_iterator iter;
  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    result.push_back({{"position", idCounter}, {"object", iter->getJsonObject()}});
    idCounter++;
  }

  return result;
}

void ShapesBank::addCircle(const std::string& identifier, const std::string& centerId, float centerX, float centerY,
                           const std::string& centerName, float radius, const std::vector<std::string>& includedPoints) {
  std::vector<std::string> includedPointsCopy{includedPoints};
  const unsigned int includedPointsLength = includedPointsCopy.size();

  for (int point1Id = 0; point1Id < includedPointsLength; point1Id++) {
    for (int point2Id = point1Id; point2Id < includedPointsLength; point2Id++) {
      const PointModel point1 = points[pointIdsConverter[includedPointsCopy[point1Id]]];
      const PointModel point2 = points[pointIdsConverter[includedPointsCopy[point2Id]]];

      if (ShapesBank::counterClockwiseComparator(point1, point2, centerX, centerY)) {
        std::swap(includedPointsCopy[point1Id], includedPointsCopy[point2Id]);
      }
    }
  }

  circles.emplace_back(identifier, centerId, centerX, centerY, centerName, radius, includedPointsCopy);
  circleIdsConverter.insert(std::make_pair(identifier, circleIdCounter));
  circleIdCounter++;

  if(heuristicsBank != nullptr) {
    heuristicsBank->markNewCirclesFlag();
  }
}

std::string ShapesBank::getCircleIdWithTwoPoints(const std::string& centerPointId, const std::string& pointOnCircleId) const {
  for (unsigned int circleId = 0; circleId < circles.size(); circleId++) {
    if(circles[circleId].getCenterId() != centerPointId) {
      continue ;
    }

    std::vector<std::string> circlePoints = circles[circleId].getIncludedPoints();
    if (std::find(circlePoints.begin(), circlePoints.end(), pointOnCircleId) != circlePoints.end()) {

      return circles[circleId].getId();
    }
  }

  throw std::invalid_argument("Does not exist circle with given center on point on!");
}

json ShapesBank::getCirclesAsJsonObjects() const {
  json result;
  unsigned int idCounter = 0;

  std::vector<CircleModel>::const_iterator iter;
  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    result.push_back({{"position", idCounter}, {"object", iter->getJsonObject()}});
    idCounter++;
  }

  return result;
}

void ShapesBank::findIntersectionPointsOfLines() {
  const size_t linesNumber{lines.size()};

  intersectionPointsOfLines.resize(linesNumber, std::vector<std::vector<size_t>>(linesNumber));
  pointsOnLinesIntersections.resize(points.size(), {});

  for (size_t line1Id = 0; line1Id < (linesNumber < 1 ? 0 : (linesNumber - 1)); line1Id++) {
    for (size_t line2Id = line1Id + 1; line2Id < linesNumber; line2Id++) {
      const std::vector<std::string>& points1 = lines[line1Id].getIncludedPoints();
      const std::vector<std::string>& points2 = lines[line2Id].getIncludedPoints();

      for (const std::string& point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(),
                        [point1Id](const std::string& otherPoint) { return otherPoint == point1Id; })) {
          const std::string& commonPointId = point1Id;
          intersectionPointsOfLines[line1Id][line2Id].push_back(getPointPositionInVector(commonPointId));
          intersectionPointsOfLines[line2Id][line1Id].push_back(getPointPositionInVector(commonPointId));
          pointsOnLinesIntersections[getPointPositionInVector(commonPointId)].push_back({
              line1Id, line2Id
          });
          break;
        }
      }
    }
  }
}

void ShapesBank::findIntersectionPointsOfCircles() {
  const size_t circlesNumber{circles.size()};

  intersectionPointsOfCircles.resize(circlesNumber, std::vector<std::vector<size_t>>(circlesNumber));
  pointsOnCirclesIntersections.resize(points.size(), {});

  for (size_t circle1Id = 0; circle1Id < (circlesNumber < 1 ? 0 : (circlesNumber - 1)); circle1Id++) {
    for (size_t circle2Id = circle1Id + 1; circle2Id < circlesNumber; circle2Id++) {
      const std::vector<std::string>& points1 = circles[circle1Id].getIncludedPoints();
      const std::vector<std::string>& points2 = circles[circle2Id].getIncludedPoints();

      size_t counter = 0;
      for (const std::string& point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(),
                        [point1Id](const std::string& otherPoint) { return otherPoint == point1Id; })) {
          const std::string& commonPointId = point1Id;
          intersectionPointsOfCircles[circle1Id][circle2Id].push_back(getPointPositionInVector(commonPointId));
          intersectionPointsOfCircles[circle2Id][circle1Id].push_back(getPointPositionInVector(commonPointId));
          pointsOnCirclesIntersections[getPointPositionInVector(commonPointId)].push_back({
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

void ShapesBank::findIntersectionPointsOfLinesCircles() {
  const size_t linesNumber{lines.size()};
  const size_t circlesNumber{circles.size()};

  intersectionPointsOfLinesAndCircles.resize(linesNumber, std::vector<std::vector<size_t>>(circlesNumber));
  pointsOnLineAndCircleIntersections.resize(points.size(), {});

  for (size_t lineId = 0; lineId < linesNumber; lineId++) {
    for (size_t circleId = 0; circleId < circlesNumber; circleId++) {
      const std::vector<std::string>& points1 = lines[lineId].getIncludedPoints();
      const std::vector<std::string>& points2 = circles[circleId].getIncludedPoints();

      size_t counter = 0;
      for (const std::string& point1Id : points1) {
        if (std::any_of(points2.begin(), points2.end(),
                        [point1Id](const std::string& otherPoint) { return otherPoint == point1Id; })) {
          const std::string& commonPointId = point1Id;
          intersectionPointsOfLinesAndCircles[lineId][circleId].push_back(getPointPositionInVector(commonPointId));
          pointsOnLineAndCircleIntersections[getPointPositionInVector(commonPointId)].push_back({
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

void ShapesBank::findPointsOnShapes() {
  pointsOnShapes.resize(points.size(), {false, false});

  for (const LineModel& line: lines) {
    for (const std::string& pointId: line.getIncludedPoints()) {
      pointsOnShapes[getPointPositionInVector(pointId)][0] = true;
    }
  }

  for (const CircleModel& circle: circles) {
    for (const std::string& pointId: circle.getIncludedPoints()) {
      pointsOnShapes[getPointPositionInVector(pointId)][0] = true;
    }
  }
}

json ShapesBank::getIntersectionPointsAsJson() {
  return {{"line_line", json(intersectionPointsOfLines)},
          {"circle_circle", json(intersectionPointsOfCircles)},
          {"line_circle", json(intersectionPointsOfLinesAndCircles)},
          {"points_on_line_line", json(pointsOnLinesIntersections)},
          {"points_on_circle_circle", json(pointsOnCirclesIntersections)},
          {"points_on_line_circle", json(pointsOnLineAndCircleIntersections)}};
}

bool ShapesBank::counterClockwiseComparator(const PointModel& point1, const PointModel& point2, float centerX, float centerY) {
  const float diffX1 = point1.getX() - centerX;
  const float diffY1 = point1.getY() - centerY;
  const float diffX2 = point2.getX() - centerX;
  const float diffY2 = point2.getY() - centerY;

  if (diffX1 >= 0 && diffX2 < 0) {
    return true;
  }

  if (diffX1 < 0 && diffX2 >= 0) {
    return false;
  }

  if (diffX1 == 0 && diffX2 == 0) {
    if (diffY1 >= 0 || diffY2 >= 0) {
      return diffY1 > diffY2;
    }

    return diffY2 > diffY1;
  }

  const double det = diffX1 * diffY2 - diffX2 * diffY1;
  if (det < 0) {
    return true;
  }
  if (det > 0) {
    return false;
  }

  const double distance1 = diffX1 * diffX1 + diffY1 * diffY1;
  const double distance2 = diffX2 * diffX2 + diffY2 * diffY2;

  return distance1 > distance2;
}
}  // namespace expertBackground