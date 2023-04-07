#include "ShapesBank.h"

namespace expertBackground {
ShapesBank::ShapesBank()
    : pointIdCounter(0),
      lineIdCounter(0),
      circleIdCounter(0),
      points({}),
      circles({}),
      pointIdsConverter({}),
      lineIdsConverter({}),
      circleIdsConverter({}) {}

ShapesBank::ShapesBank(unsigned int maxPointIdCounter, unsigned int maxLineIdCounter, unsigned int maxCircleIdCounter)
    : pointIdCounter(0),
      lineIdCounter(0),
      circleIdCounter(0),
      points({}),
      lines({}),
      circles({}),
      pointIdsConverter(std::vector<unsigned int>(maxPointIdCounter + 1, 0)),
      lineIdsConverter(std::vector<unsigned int>(maxLineIdCounter + 1, 0)),
      circleIdsConverter(std::vector<unsigned int>(maxCircleIdCounter + 1, 0)) {}

void ShapesBank::addPoint(unsigned int identifier, float xCoordinate, float yCoordinate) {
  points.emplace_back(identifier, xCoordinate, yCoordinate);
  pointIdsConverter.at(identifier) = pointIdCounter;
  pointIdCounter++;
}

json ShapesBank::getPointsAsJsonObjects() {
  json result;
  unsigned int idCounter = 0;

  std::vector<PointModel>::iterator iter;
  for (iter = points.begin(); iter != points.end(); ++iter) {
    result.push_back({{"ID", idCounter}, {"object", iter->getJsonObject()}});
    idCounter++;
  }

  return result;
}

void ShapesBank::addLine(unsigned int identifier, LineModel::LineType lineType, float lineA, float lineB,
                         std::vector<unsigned int> includedPoints) {
  const unsigned int maxId = includedPoints.size();

  if (lineType == LineModel::LineType::VERTICAL) {
    for (unsigned int point1Id = 0; point1Id < maxId; point1Id++) {
      for (unsigned int point2Id = point1Id; point2Id < maxId; point2Id++) {
        const PointModel point1 = points[includedPoints[point1Id]];
        const PointModel point2 = points[includedPoints[point2Id]];

        if (point1.getY() > point2.getY()) {
          const unsigned int tmp = includedPoints[point1Id];
          includedPoints[point1Id] = includedPoints[point2Id];
          includedPoints[point2Id] = tmp;
        }
      }
    }
  }
  else {
    for (unsigned int point1Id = 0; point1Id < maxId; point1Id++) {
      for (unsigned int point2Id = point1Id; point2Id < maxId; point2Id++) {
        const PointModel point1 = points[includedPoints[point1Id]];
        const PointModel point2 = points[includedPoints[point2Id]];

        if (point1.getX() > point2.getX()) {
          const unsigned int tmp = includedPoints[point1Id];
          includedPoints[point1Id] = includedPoints[point2Id];
          includedPoints[point2Id] = tmp;
        }
      }
    }
  }

  lines.emplace_back(identifier, lineType, lineA, lineB, includedPoints);
  lineIdsConverter.at(identifier) = lineIdCounter;
  lineIdCounter++;
}

unsigned int ShapesBank::getLineIdThrowTwoPoints(unsigned int point1Id, unsigned int point2Id) const {
  for (unsigned int lineId = 0; lineId < lines.size(); lineId++) {
    std::vector<unsigned int> linePoints = lines[lineId].getIncludedPoints();
    if (std::find(linePoints.begin(), linePoints.end(), point1Id) != linePoints.end() &&
        std::find(linePoints.begin(), linePoints.end(), point2Id) != linePoints.end()) {

      return lineId;
    }
  }

  throw std::invalid_argument("Does not exist line which is throw given 2 points!");
}

json ShapesBank::getLinesAsJsonObjects() {
  json result;
  unsigned int idCounter = 0;

  std::vector<LineModel>::iterator iter;
  for (iter = lines.begin(); iter != lines.end(); ++iter) {
    result.push_back({{"ID", idCounter}, {"object", iter->getJsonObject()}});
    idCounter++;
  }

  return result;
}

void ShapesBank::addCircle(unsigned int identifier, unsigned int centerId, float centerX, float centerY, float radius,
                           std::vector<unsigned int> includedPoints) {
  const unsigned int maxId = includedPoints.size();
  for (int point1Id = 0; point1Id < maxId; point1Id++) {
    for (int point2Id = point1Id; point2Id < maxId; point2Id++) {
      const PointModel point1 = points[includedPoints[point1Id]];
      const PointModel point2 = points[includedPoints[point2Id]];

      if (ShapesBank::counterClockwiseComparator(point1, point2, centerX, centerY)) {
        const unsigned int tmp = includedPoints[point1Id];
        includedPoints[point1Id] = includedPoints[point2Id];
        includedPoints[point2Id] = tmp;
      }
    }
  }

  circles.emplace_back(identifier, centerId, centerX, centerY, radius, includedPoints);
  circleIdsConverter.at(identifier) = circleIdCounter;
  circleIdCounter++;
}

json ShapesBank::getCirclesAsJsonObjects() {
  json result;
  unsigned int idCounter = 0;

  std::vector<CircleModel>::iterator iter;
  for (iter = circles.begin(); iter != circles.end(); ++iter) {
    result.push_back({{"ID", idCounter}, {"object", iter->getJsonObject()}});
    idCounter++;
  }

  return result;
}

std::vector<unsigned int> ShapesBank::getUnifiedPointsVector(std::vector<unsigned int> pointIds) {
  std::vector<unsigned int> result = {};

  std::vector<unsigned int>::iterator iter;
  for (iter = pointIds.begin(); iter != pointIds.end(); ++iter) {
    result.push_back(pointIdsConverter.at(*iter));
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