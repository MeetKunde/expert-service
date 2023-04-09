#include "../Expert.h"

namespace expert {
unsigned int Expert::findVerticalAngles() {
  unsigned int result = 0;

  for (unsigned int intersectionPointId = 0; intersectionPointId < pointsNumber; intersectionPointId++) {
    for (const std::pair<unsigned int, unsigned int> lineIdsPair : pointsOnLinesIntersections[intersectionPointId]) {
      std::vector<unsigned int> line1Points = shapesBank.getLine(lineIdsPair.first).getIncludedPoints();
      std::vector<unsigned int> line2Points = shapesBank.getLine(lineIdsPair.second).getIncludedPoints();
      const unsigned int vertexIndexOnLine1 =
          distance(line1Points.begin(), find(line1Points.begin(), line1Points.end(), intersectionPointId));
      const unsigned int vertexIndexOnLine2 =
          distance(line2Points.begin(), find(line2Points.begin(), line2Points.end(), intersectionPointId));

      if (vertexIndexOnLine1 == 0 || vertexIndexOnLine1 == (line1Points.size() - 1)) {
        continue;
      }
      if (vertexIndexOnLine2 == 0 || vertexIndexOnLine2 == (line2Points.size() - 1)) {
        continue;
      }

      result += setEqualAngles(line1Points.front(), intersectionPointId, line2Points.front(), line1Points.back(),
                               intersectionPointId, line2Points.back(), AngleType::CONVEX, IDependency::Reason::VERTICAL_ANGLES,
                               IDependency::UsefulnessLevel::MEDIUM);

      result += setEqualAngles(line1Points.front(), intersectionPointId, line2Points.back(), line1Points.back(),
                               intersectionPointId, line2Points.front(), AngleType::CONVEX, IDependency::Reason::VERTICAL_ANGLES,
                               IDependency::UsefulnessLevel::MEDIUM);
    }
  }

  return result;
}

unsigned int Expert::findSupplementaryAngles() {
  unsigned int result = 0;

  for (unsigned int lineId = 0; lineId < linesNumber; lineId++) {
    std::vector<unsigned int> line1Points = shapesBank.getLine(lineId).getIncludedPoints();

    for (unsigned int vertexPointId = 1; vertexPointId < (line1Points.size() - 1); vertexPointId++) {
      const unsigned int vertexPoint = line1Points[vertexPointId];
      const unsigned int firstPoint = line1Points.front();
      const unsigned int lastPoint = line1Points.back();
      std::vector<unsigned int> justSelected(line1Points.size());

      std::copy(line1Points.begin(), line1Points.end(), justSelected.begin());

      std::set<unsigned int> lineIds = {};
      for (const std::pair<unsigned int, unsigned int> linesPair : pointsOnLinesIntersections[vertexPoint]) {
        lineIds.insert(linesPair.first);
        lineIds.insert(linesPair.second);
      }
      lineIds.erase(lineId);

      for (const unsigned int otherLineId : lineIds) {
        std::vector<unsigned int> line2Points = shapesBank.getLine(otherLineId).getIncludedPoints();
        std::copy(line2Points.begin(), line2Points.end(), std::back_inserter(justSelected));

        if (vertexPoint != line2Points.front()) {
          result += dependenciesBank.addConvexAngle(
              firstPoint, vertexPoint, line2Points.front(),
              Integer(MathHelper::STRAIGHT_ANGLE_VALUE) -
                  dependenciesBank.getConvexAngleVariable(line2Points.front(), vertexPoint, lastPoint),
              IDependency::Reason::SUPPLEMENTARY_ANGLES, {}, IDependency::UsefulnessLevel::MEDIUM);
        }

        if (vertexPoint != line2Points.back()) {
          result += dependenciesBank.addConvexAngle(
              firstPoint, vertexPoint, line2Points.back(),
              Integer(MathHelper::STRAIGHT_ANGLE_VALUE) -
                  dependenciesBank.getConvexAngleVariable(line2Points.back(), vertexPoint, lastPoint),
              IDependency::Reason::SUPPLEMENTARY_ANGLES, {}, IDependency::UsefulnessLevel::MEDIUM);
        }
      }

      for (unsigned int pointId = 0; pointId < pointsNumber; pointId++) {
        if (find(justSelected.begin(), justSelected.end(), pointId) == justSelected.end()) {
          result +=
              dependenciesBank.addConvexAngle(firstPoint, vertexPoint, pointId,
                                              Integer(MathHelper::STRAIGHT_ANGLE_VALUE) -
                                                  dependenciesBank.getConvexAngleVariable(pointId, vertexPoint, lastPoint),
                                              IDependency::Reason::SUPPLEMENTARY_ANGLES, {}, IDependency::UsefulnessLevel::LOW);
        }
      }
    }
  }

  return result;
}

unsigned int Expert::findAlternateAngles() {
  unsigned int result = 0;

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  for (const auto& parallelLinesPair : parallelLines) {
    const unsigned int line1Id = parallelLinesPair->getFirstElement().getId();
    const unsigned int line2Id = parallelLinesPair->getSecondElement().getId();

    if (line1Id == line2Id) {
      continue;
    }

    for (unsigned int otherLineId = 0; otherLineId < linesNumber; otherLineId++) {
      if (line1Id == otherLineId || line2Id == otherLineId) {
        continue;
      }

      if (intersectionPointsOfLines[line1Id][otherLineId].empty() || intersectionPointsOfLines[line2Id][otherLineId].empty()) {
        continue;
      }

      std::vector<unsigned int> parallel1Points = shapesBank.getLine(line1Id).getIncludedPoints();
      std::vector<unsigned int> parallel2Points = shapesBank.getLine(line2Id).getIncludedPoints();
      std::vector<unsigned int> transversalPoints = shapesBank.getLine(otherLineId).getIncludedPoints();

      if (parallel1Points.size() < 2 || parallel2Points.size() < 2 || transversalPoints.size() < 2) {
        continue;
      }

      const unsigned int vertex1Id = intersectionPointsOfLines[line1Id][otherLineId][0];
      const unsigned int vertex2Id = intersectionPointsOfLines[line2Id][otherLineId][0];

      const unsigned int vertex1IndexParallel1 =
          distance(parallel1Points.begin(), find(parallel1Points.begin(), parallel1Points.end(), vertex1Id));
      const unsigned int vertex2IndexParallel2 =
          distance(parallel2Points.begin(), find(parallel2Points.begin(), parallel2Points.end(), vertex2Id));
      const unsigned int vertex1IndexTransversal =
          distance(transversalPoints.begin(), find(transversalPoints.begin(), transversalPoints.end(), vertex1Id));
      const unsigned int vertex2IndexTransversal =
          distance(transversalPoints.begin(), find(transversalPoints.begin(), transversalPoints.end(), vertex2Id));

      const unsigned int topReferenceVertex =
          vertex1IndexTransversal < vertex2IndexTransversal ? transversalPoints.front() : transversalPoints.back();
      const unsigned int bottomReferenceVertex =
          vertex2IndexTransversal < vertex1IndexTransversal ? transversalPoints.front() : transversalPoints.back();

      const double topDistance = MathHelper::getDistance(shapesBank.getPoint(topReferenceVertex).getCoordinates(),
                                                         shapesBank.getPoint(vertex1Id).getCoordinates());

      const double bottomDistance = MathHelper::getDistance(shapesBank.getPoint(bottomReferenceVertex).getCoordinates(),
                                                            shapesBank.getPoint(vertex2Id).getCoordinates());

      std::vector<unsigned int> transversalPointsTop = {topReferenceVertex};
      std::vector<unsigned int> transversalPointsBottom = {bottomReferenceVertex};
      std::vector<unsigned int> transversalPointsMiddle = {};

      for (unsigned int pointIdIndex = 1; pointIdIndex < (transversalPoints.size() - 1); pointIdIndex++) {
        if (MathHelper::getDistance(shapesBank.getPoint(transversalPointsTop.front()).getCoordinates(),
                                    shapesBank.getPoint(transversalPoints[pointIdIndex]).getCoordinates()) < topDistance) {
          transversalPointsTop.push_back(transversalPoints[pointIdIndex]);
        }
        else if (MathHelper::getDistance(shapesBank.getPoint(transversalPointsBottom.front()).getCoordinates(),
                                         shapesBank.getPoint(transversalPoints[pointIdIndex]).getCoordinates()) <
                 bottomDistance) {
          transversalPointsBottom.push_back(transversalPoints[pointIdIndex]);
        }
        else {
          transversalPointsMiddle.push_back(transversalPoints[pointIdIndex]);
        }
      }

      if (topReferenceVertex == vertex1Id) {
        transversalPointsTop = {};
        transversalPointsMiddle.insert(transversalPointsMiddle.begin(), vertex1Id);
      }
      if (bottomReferenceVertex == vertex2Id) {
        transversalPointsBottom = {};
        transversalPointsMiddle.insert(transversalPointsMiddle.begin(), vertex2Id);
      }

      std::vector<unsigned int> transversalPointsTopMiddle = {};
      std::vector<unsigned int> transversalPointsBottomMiddle = {};

      for (unsigned int pointIdIndex = 0; pointIdIndex < transversalPointsTop.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsTop[pointIdIndex]);
      }
      for (unsigned int pointIdIndex = 0; pointIdIndex < transversalPointsBottom.size(); pointIdIndex++) {
        transversalPointsBottomMiddle.push_back(transversalPointsBottom[pointIdIndex]);
      }
      for (unsigned int pointIdIndex = 0; pointIdIndex < transversalPointsMiddle.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
        transversalPointsBottomMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
      }

      transversalPointsTopMiddle.erase(
          std::find(transversalPointsTopMiddle.begin(), transversalPointsTopMiddle.end(), vertex2Id));
      transversalPointsBottomMiddle.erase(
          std::find(transversalPointsBottomMiddle.begin(), transversalPointsBottomMiddle.end(), vertex1Id));

      std::vector<unsigned int> leftTopPoints = {};
      std::vector<unsigned int> rightTopPoints = {};
      std::vector<unsigned int> leftBottomPoints = {};
      std::vector<unsigned int> rightBottomPoints = {};

      double cosAngle1 = 0.0;
      if (vertex1Id != parallel1Points.front()) {
        cosAngle1 = MathHelper::getAngleCosine(shapesBank.getPoint(vertex2Id).getCoordinates(),
                                               shapesBank.getPoint(vertex1Id).getCoordinates(),
                                               shapesBank.getPoint(parallel1Points.front()).getCoordinates());
      }
      else {
        cosAngle1 = -MathHelper::getAngleCosine(shapesBank.getPoint(vertex2Id).getCoordinates(),
                                                shapesBank.getPoint(vertex1Id).getCoordinates(),
                                                shapesBank.getPoint(parallel1Points.back()).getCoordinates());
      }

      if (fabs(cosAngle1) < MathHelper::COMPARISON_EPSILON) {  // right angle
                                                               // problem: cannot conclude points orientation
      }
      else if (cosAngle1 > 0.0) {  // acute angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          leftTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
          leftTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
      }

      double cosAngle2 = 0.0;
      if (vertex2Id != parallel2Points.front()) {
        cosAngle2 = MathHelper::getAngleCosine(shapesBank.getPoint(vertex1Id).getCoordinates(),
                                               shapesBank.getPoint(vertex2Id).getCoordinates(),
                                               shapesBank.getPoint(parallel2Points.front()).getCoordinates());
      }
      else {
        cosAngle2 = -MathHelper::getAngleCosine(shapesBank.getPoint(vertex1Id).getCoordinates(),
                                                shapesBank.getPoint(vertex2Id).getCoordinates(),
                                                shapesBank.getPoint(parallel2Points.back()).getCoordinates());
      }

      if (fabs(cosAngle2) < MathHelper::COMPARISON_EPSILON) {  // right angle
                                                               // problem: cannot conclude points orientation
      }
      else if (cosAngle2 > 0.0) {  // acute angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }

      for (unsigned int point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              result +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 rightBottomPoints[point2Id], vertex2Id, transversalPointsTopMiddle[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit1;
            }
          }
        }
      }

    exit1 : {}

      for (unsigned int point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              result +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 leftBottomPoints[point2Id], vertex2Id, transversalPointsTopMiddle[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit2;
            }
          }
        }
      }

    exit2 : {}

      for (unsigned int point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              result +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id], rightBottomPoints[point2Id],
                                 vertex2Id, transversalPointsBottom[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit3;
            }
          }
        }
      }

    exit3 : {}

      for (unsigned int point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              result +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id], leftBottomPoints[point2Id],
                                 vertex2Id, transversalPointsBottom[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit4;
            }
          }
        }
      }

    exit4 : {}
    }
  }

  return result;
}

unsigned int Expert::findCorrespondingAngles() {
  unsigned int result = 0;

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  for (const auto& parallelLinesPair : parallelLines) {
    const unsigned int line1Id = parallelLinesPair->getFirstElement().getId();
    const unsigned int line2Id = parallelLinesPair->getSecondElement().getId();

    if (line1Id == line2Id) {
      continue;
    }

    for (unsigned int otherLineId = 0; otherLineId < linesNumber; otherLineId++) {
      if (line1Id == otherLineId || line2Id == otherLineId) {
        continue;
      }

      if (intersectionPointsOfLines[line1Id][otherLineId].empty() || intersectionPointsOfLines[line2Id][otherLineId].empty()) {
        continue;
      }

      std::vector<unsigned int> parallel1Points = shapesBank.getLine(line1Id).getIncludedPoints();
      std::vector<unsigned int> parallel2Points = shapesBank.getLine(line2Id).getIncludedPoints();
      std::vector<unsigned int> transversalPoints = shapesBank.getLine(otherLineId).getIncludedPoints();

      if (parallel1Points.size() < 2 || parallel2Points.size() < 2 || transversalPoints.size() < 2) {
        continue;
      }

      const unsigned int vertex1Id = intersectionPointsOfLines[line1Id][otherLineId][0];
      const unsigned int vertex2Id = intersectionPointsOfLines[line2Id][otherLineId][0];

      const unsigned int vertex1IndexParallel1 =
          distance(parallel1Points.begin(), find(parallel1Points.begin(), parallel1Points.end(), vertex1Id));
      const unsigned int vertex2IndexParallel2 =
          distance(parallel2Points.begin(), find(parallel2Points.begin(), parallel2Points.end(), vertex2Id));
      const unsigned int vertex1IndexTransversal =
          distance(transversalPoints.begin(), find(transversalPoints.begin(), transversalPoints.end(), vertex1Id));
      const unsigned int vertex2IndexTransversal =
          distance(transversalPoints.begin(), find(transversalPoints.begin(), transversalPoints.end(), vertex2Id));

      const unsigned int topReferenceVertex =
          vertex1IndexTransversal < vertex2IndexTransversal ? transversalPoints.front() : transversalPoints.back();
      const unsigned int bottomReferenceVertex =
          vertex2IndexTransversal < vertex1IndexTransversal ? transversalPoints.front() : transversalPoints.back();

      const double topDistance = MathHelper::getDistance(shapesBank.getPoint(topReferenceVertex).getCoordinates(),
                                                         shapesBank.getPoint(vertex1Id).getCoordinates());

      const double bottomDistance = MathHelper::getDistance(shapesBank.getPoint(bottomReferenceVertex).getCoordinates(),
                                                            shapesBank.getPoint(vertex2Id).getCoordinates());

      std::vector<unsigned int> transversalPointsTop = {topReferenceVertex};
      std::vector<unsigned int> transversalPointsBottom = {bottomReferenceVertex};
      std::vector<unsigned int> transversalPointsMiddle = {};

      for (unsigned int pointIdIndex = 1; pointIdIndex < (transversalPoints.size() - 1); pointIdIndex++) {
        if (MathHelper::getDistance(shapesBank.getPoint(transversalPointsTop.front()).getCoordinates(),
                                    shapesBank.getPoint(transversalPoints[pointIdIndex]).getCoordinates()) < topDistance) {
          transversalPointsTop.push_back(transversalPoints[pointIdIndex]);
        }
        else if (MathHelper::getDistance(shapesBank.getPoint(transversalPointsBottom.front()).getCoordinates(),
                                         shapesBank.getPoint(transversalPoints[pointIdIndex]).getCoordinates()) <
                 bottomDistance) {
          transversalPointsBottom.push_back(transversalPoints[pointIdIndex]);
        }
        else {
          transversalPointsMiddle.push_back(transversalPoints[pointIdIndex]);
        }
      }

      if (topReferenceVertex == vertex1Id) {
        transversalPointsTop = {};
        transversalPointsMiddle.insert(transversalPointsMiddle.begin(), vertex1Id);
      }
      if (bottomReferenceVertex == vertex2Id) {
        transversalPointsBottom = {};
        transversalPointsMiddle.insert(transversalPointsMiddle.begin(), vertex2Id);
      }

      std::vector<unsigned int> transversalPointsTopMiddle = {};
      std::vector<unsigned int> transversalPointsBottomMiddle = {};

      for (unsigned int pointIdIndex = 0; pointIdIndex < transversalPointsTop.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsTop[pointIdIndex]);
      }
      for (unsigned int pointIdIndex = 0; pointIdIndex < transversalPointsBottom.size(); pointIdIndex++) {
        transversalPointsBottomMiddle.push_back(transversalPointsBottom[pointIdIndex]);
      }
      for (unsigned int pointIdIndex = 0; pointIdIndex < transversalPointsMiddle.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
        transversalPointsBottomMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
      }

      transversalPointsTopMiddle.erase(
          std::find(transversalPointsTopMiddle.begin(), transversalPointsTopMiddle.end(), vertex2Id));
      transversalPointsBottomMiddle.erase(
          std::find(transversalPointsBottomMiddle.begin(), transversalPointsBottomMiddle.end(), vertex1Id));

      std::vector<unsigned int> leftTopPoints = {};
      std::vector<unsigned int> rightTopPoints = {};
      std::vector<unsigned int> leftBottomPoints = {};
      std::vector<unsigned int> rightBottomPoints = {};

      double cosAngle1 = 0.0;
      if (vertex1Id != parallel1Points.front()) {
        cosAngle1 = MathHelper::getAngleCosine(shapesBank.getPoint(vertex2Id).getCoordinates(),
                                               shapesBank.getPoint(vertex1Id).getCoordinates(),
                                               shapesBank.getPoint(parallel1Points.front()).getCoordinates());
      }
      else {
        cosAngle1 = -MathHelper::getAngleCosine(shapesBank.getPoint(vertex2Id).getCoordinates(),
                                                shapesBank.getPoint(vertex1Id).getCoordinates(),
                                                shapesBank.getPoint(parallel1Points.back()).getCoordinates());
      }

      if (fabs(cosAngle1) < MathHelper::COMPARISON_EPSILON) {  // right angle
                                                               // problem: cannot conclude points orientation
      }
      else if (cosAngle1 > 0.0) {  // acute angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          leftTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
          leftTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
      }

      double cosAngle2 = 0.0;
      if (vertex2Id != parallel2Points.front()) {
        cosAngle2 = MathHelper::getAngleCosine(shapesBank.getPoint(vertex1Id).getCoordinates(),
                                               shapesBank.getPoint(vertex2Id).getCoordinates(),
                                               shapesBank.getPoint(parallel2Points.front()).getCoordinates());
      }
      else {
        cosAngle2 = -MathHelper::getAngleCosine(shapesBank.getPoint(vertex1Id).getCoordinates(),
                                                shapesBank.getPoint(vertex2Id).getCoordinates(),
                                                shapesBank.getPoint(parallel2Points.back()).getCoordinates());
      }

      if (fabs(cosAngle2) < MathHelper::COMPARISON_EPSILON) {  // right angle
                                                               // problem: cannot conclude points orientation
      }
      else if (cosAngle2 > 0.0) {  // acute angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (unsigned int pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (unsigned int pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }

      for (unsigned int point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              result +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id], leftBottomPoints[point2Id],
                                 vertex2Id, transversalPointsTopMiddle[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit1;
            }
          }
        }
      }

    exit1 : {}

      for (unsigned int point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              result +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id], rightBottomPoints[point2Id],
                                 vertex2Id, transversalPointsTopMiddle[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit2;
            }
          }
        }
      }

    exit2 : {}

      for (unsigned int point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              result +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 leftBottomPoints[point2Id], vertex2Id, transversalPointsBottom[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit3;
            }
          }
        }
      }

    exit3 : {}

      for (unsigned int point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (unsigned int point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (unsigned int point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (unsigned int point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              result +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 rightBottomPoints[point2Id], vertex2Id, transversalPointsBottom[point4Id], AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, IDependency::UsefulnessLevel::MEDIUM);

              goto exit4;
            }
          }
        }
      }

    exit4 : {}
    }
  }

  return result;
}
}  // namespace expert
