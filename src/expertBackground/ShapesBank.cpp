#include "ShapesBank.h"

namespace expertBackground {
ShapesBank::ShapesBank()
    : pointIdCounter{0},
      lineIdCounter{0},
      circleIdCounter{0},
      points{},
      circles{},
      pointIdsConverter{},
      lineIdsConverter{},
      circleIdsConverter{} {}

void ShapesBank::addPoint(std::string identifier, float xCoordinate, float yCoordinate, std::string name) {
  points.emplace_back(identifier, xCoordinate, yCoordinate, name);
  pointIdsConverter.insert(std::make_pair(identifier, pointIdCounter));
  pointIdCounter++;
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

void ShapesBank::addLine(std::string identifier, LineModel::LineType lineType, float lineA, float lineB,
                         std::vector<std::string> includedPoints) {
  const unsigned int includedPointsLength = includedPoints.size();

  if (lineType == LineModel::LineType::VERTICAL) {
    for (unsigned int point1Id = 0; point1Id < includedPointsLength; point1Id++) {
      for (unsigned int point2Id = point1Id; point2Id < includedPointsLength; point2Id++) {
        const PointModel point1 = points[pointIdsConverter.at(includedPoints[point1Id])];
        const PointModel point2 = points[pointIdsConverter.at(includedPoints[point2Id])];

        if (point1.getY() > point2.getY()) {
          std::swap(includedPoints[point1Id], includedPoints[point2Id]);
        }
      }
    }
  }
  else {
    for (unsigned int point1Id = 0; point1Id < includedPointsLength; point1Id++) {
      for (unsigned int point2Id = point1Id; point2Id < includedPointsLength; point2Id++) {
        const PointModel point1 = points[pointIdsConverter[includedPoints[point1Id]]];
        const PointModel point2 = points[pointIdsConverter[includedPoints[point2Id]]];

        if (point1.getX() > point2.getX()) {
          std::swap(includedPoints[point1Id], includedPoints[point2Id]);
        }
      }
    }
  }

  lines.emplace_back(identifier, lineType, lineA, lineB, includedPoints);
  lineIdsConverter.insert(std::make_pair(identifier, lineIdCounter));
  lineIdCounter++;
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

void ShapesBank::addCircle(std::string identifier, std::string centerId, float centerX, float centerY, std::string centerName, float radius,
                           std::vector<std::string> includedPoints) {
  const unsigned int includedPointsLength = includedPoints.size();
  for (int point1Id = 0; point1Id < includedPointsLength; point1Id++) {
    for (int point2Id = point1Id; point2Id < includedPointsLength; point2Id++) {
      const PointModel point1 = points[pointIdsConverter[includedPoints[point1Id]]];
      const PointModel point2 = points[pointIdsConverter[includedPoints[point2Id]]];

      if (ShapesBank::counterClockwiseComparator(point1, point2, centerX, centerY)) {
        std::swap(includedPoints[point1Id], includedPoints[point2Id]);
      }
    }
  }

  circles.emplace_back(identifier, centerId, centerX, centerY, centerName, radius, includedPoints);
  circleIdsConverter.insert(std::make_pair(identifier, circleIdCounter));
  circleIdCounter++;
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