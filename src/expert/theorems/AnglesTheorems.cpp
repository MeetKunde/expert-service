#include "../Expert.h"

namespace expert {
unsigned int Expert::exploreAngleBasedTheorems() {
  unsigned int sumOfNewDependencies{0};

  if(shapesBank.newPointLastAdded() || shapesBank.newLineLastAdded()) {
    sumOfNewDependencies += findVerticalAngles();
    sumOfNewDependencies += findSupplementaryAngles();
  }

  if(shapesBank.newPointLastAdded() || shapesBank.newLineLastAdded() || dependenciesBank.lastChange(IDependency::Type::PARALLEL_LINES)) {
    sumOfNewDependencies += findAlternateAngles();
    sumOfNewDependencies += findCorrespondingAngles();
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findVerticalAngles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<PointModel>& points = shapesBank.getPointsVector();
  const std::vector<LineModel>& lines = shapesBank.getLinesVector();
  for (size_t intersectionPointPos = 0; intersectionPointPos < points.size(); intersectionPointPos++) {
    for (const std::pair<size_t, size_t>& linePosesPair : pointsOnLinesIntersections[intersectionPointPos]) {
      const std::vector<std::string>& line1Points = lines.at(linePosesPair.first).getIncludedPoints();
      const std::vector<std::string>& line2Points = lines.at(linePosesPair.second).getIncludedPoints();
      const size_t vertexIndexOnLine1 =
          distance(line1Points.begin(), std::find(line1Points.begin(), line1Points.end(),
                                                  points.at(intersectionPointPos).getId()));
      const size_t vertexIndexOnLine2 =
          distance(line2Points.begin(), std::find(line2Points.begin(), line2Points.end(),
                                                  points.at(intersectionPointPos).getId()));

      if (vertexIndexOnLine1 == 0 || vertexIndexOnLine1 == (line1Points.size() - 1)) {
        continue;
      }
      if (vertexIndexOnLine2 == 0 || vertexIndexOnLine2 == (line2Points.size() - 1)) {
        continue;
      }

      sumOfNewDependencies += setEqualAngles(line1Points.front(), points.at(intersectionPointPos).getId(), line2Points.front(),
                                             line1Points.back(), points.at(intersectionPointPos).getId(), line2Points.back(),
                                             AngleType::UNKNOWN, IDependency::Reason::VERTICAL_ANGLES,
                                             {}, IDependency::ImportanceLevel::MEDIUM);

      sumOfNewDependencies += setEqualAngles(line1Points.front(), points.at(intersectionPointPos).getId(), line2Points.back(),
                                             line1Points.back(),points.at(intersectionPointPos).getId(), line2Points.front(),
                                             AngleType::UNKNOWN, IDependency::Reason::VERTICAL_ANGLES,
                                             {}, IDependency::ImportanceLevel::MEDIUM);
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findSupplementaryAngles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<LineModel>& lines = shapesBank.getLinesVector();
  const std::vector<PointModel>& points = shapesBank.getPointsVector();
  for (size_t linePos = 0; linePos < lines.size(); linePos++) {
    const std::vector<std::string>& line1Points = lines.at(linePos).getIncludedPoints();

    for (size_t vertexPointPos = 1; vertexPointPos < (line1Points.size() - 1); vertexPointPos++) {
      const std::string& vertexPoint = line1Points.at(vertexPointPos);
      const std::string& firstPoint = line1Points.front();
      const std::string& lastPoint = line1Points.back();

      std::vector<std::string> justSelected(line1Points.size());
      std::copy(line1Points.begin(), line1Points.end(), justSelected.begin());

      std::set<size_t> linePoses = {};
      for (const std::pair<size_t, size_t> linesPair : pointsOnLinesIntersections[shapesBank.getPointPositionInVector(vertexPoint)]) {
        linePoses.insert(linesPair.first);
        linePoses.insert(linesPair.second);
      }
      linePoses.erase(linePos);

      for (const size_t otherLinePos : linePoses) {
        std::vector<std::string> line2Points = lines.at(otherLinePos).getIncludedPoints();
        std::copy(line2Points.begin(), line2Points.end(), std::back_inserter(justSelected));

        if (vertexPoint != line2Points.front()) {
          sumOfNewDependencies += dependenciesBank.addEquation(
              DependenciesBank::getAngleMeasureVariable(
                  shapesBank.getPoint(firstPoint),
                  shapesBank.getPoint(vertexPoint),
                  shapesBank.getPoint(line2Points.front()),
                  true) +
              DependenciesBank::getAngleMeasureVariable(
                      shapesBank.getPoint(line2Points.front()),
                      shapesBank.getPoint(vertexPoint),
                      shapesBank.getPoint(lastPoint), false),
              Integer(MathHelper::STRAIGHT_ANGLE_VALUE),
              IDependency::Reason::SUPPLEMENTARY_ANGLES, {}, IDependency::ImportanceLevel::MEDIUM
              );
        }

        if (vertexPoint != line2Points.back()) {
          sumOfNewDependencies += dependenciesBank.addEquation(
              DependenciesBank::getAngleMeasureVariable(
                  shapesBank.getPoint(firstPoint),
                  shapesBank.getPoint(vertexPoint),
                  shapesBank.getPoint(line2Points.back()),
                  false) +
                  DependenciesBank::getAngleMeasureVariable(
                      shapesBank.getPoint(line2Points.back()),
                      shapesBank.getPoint(vertexPoint),
                      shapesBank.getPoint(lastPoint), true),
              Integer(MathHelper::STRAIGHT_ANGLE_VALUE),
              IDependency::Reason::SUPPLEMENTARY_ANGLES, {}, IDependency::ImportanceLevel::MEDIUM
          );
        }
      }

      for (size_t pointPos = 0; pointPos < points.size(); pointPos++) {
        if (std::find(justSelected.begin(), justSelected.end(), points.at(pointPos).getId()) == justSelected.end()) {
          sumOfNewDependencies += dependenciesBank.addEquation(
              DependenciesBank::getAngleMeasureVariable(
                  shapesBank.getPoint(firstPoint),
                  shapesBank.getPoint(vertexPoint),
                  points.at(pointPos),
                  true) +
              DependenciesBank::getAngleMeasureVariable(
                  points.at(pointPos),
                  shapesBank.getPoint(vertexPoint),
                  shapesBank.getPoint(lastPoint),
                  false),
              Integer(MathHelper::STRAIGHT_ANGLE_VALUE),
              IDependency::Reason::SUPPLEMENTARY_ANGLES, {}, IDependency::ImportanceLevel::LOW
              );
        }
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findAlternateAngles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  const std::vector<LineModel>& lines = shapesBank.getLinesVector();
  const std::vector<PointModel>& points = shapesBank.getPointsVector();

  for (size_t index1 = 0; index1 < parallelLines.size(); index1++) {
    const std::string& line1Id = parallelLines[index1]->getFirstObject().getId();
    const std::string& line2Id = parallelLines[index1]->getSecondObject().getId();

    if (line1Id == line2Id) {
      continue;
    }

    for (size_t index2 = 0; index2 < lines.size(); index2++) {
      const std::string& otherLineId = lines.at(index2).getId();
      if (line1Id == otherLineId || line2Id == otherLineId) {
        continue;
      }

      if (intersectionPointsOfLines[shapesBank.getLinePositionInVector(line1Id)][shapesBank.getLinePositionInVector(otherLineId)].empty() ||
          intersectionPointsOfLines[shapesBank.getLinePositionInVector(line2Id)][shapesBank.getLinePositionInVector(otherLineId)].empty()) {
        continue;
      }

      const std::vector<std::string>& parallel1Points = shapesBank.getLine(line1Id).getIncludedPoints();
      const std::vector<std::string>& parallel2Points = shapesBank.getLine(line2Id).getIncludedPoints();
      const std::vector<std::string>& transversalPoints = shapesBank.getLine(otherLineId).getIncludedPoints();

      if (parallel1Points.size() < 2 || parallel2Points.size() < 2 || transversalPoints.size() < 2) {
        continue;
      }

      const std::string& vertex1Id = points.at(intersectionPointsOfLines[shapesBank.getLinePositionInVector(line1Id)]
                                                                        [shapesBank.getLinePositionInVector(otherLineId)][0]).getId();
      const std::string& vertex2Id = points.at(intersectionPointsOfLines[shapesBank.getLinePositionInVector(line2Id)]
                                                                        [shapesBank.getLinePositionInVector(otherLineId)][0]).getId();

      const size_t vertex1IndexParallel1 =
          distance(parallel1Points.begin(), std::find(parallel1Points.begin(), parallel1Points.end(), vertex1Id));
      const size_t vertex2IndexParallel2 =
          distance(parallel2Points.begin(), std::find(parallel2Points.begin(), parallel2Points.end(), vertex2Id));
      const size_t vertex1IndexTransversal =
          distance(transversalPoints.begin(), std::find(transversalPoints.begin(), transversalPoints.end(), vertex1Id));
      const size_t vertex2IndexTransversal =
          distance(transversalPoints.begin(), std::find(transversalPoints.begin(), transversalPoints.end(), vertex2Id));

      const std::string& topReferenceVertex =
          vertex1IndexTransversal < vertex2IndexTransversal ? transversalPoints.front() : transversalPoints.back();
      const std::string& bottomReferenceVertex =
          vertex2IndexTransversal < vertex1IndexTransversal ? transversalPoints.front() : transversalPoints.back();

      const double topDistance = MathHelper::getDistance(shapesBank.getPoint(topReferenceVertex).getCoordinates(),
                                                         shapesBank.getPoint(vertex1Id).getCoordinates());

      const double bottomDistance = MathHelper::getDistance(shapesBank.getPoint(bottomReferenceVertex).getCoordinates(),
                                                            shapesBank.getPoint(vertex2Id).getCoordinates());

      std::vector<std::string> transversalPointsTop = {topReferenceVertex};
      std::vector<std::string> transversalPointsBottom = {bottomReferenceVertex};
      std::vector<std::string> transversalPointsMiddle = {};

      for (size_t pointIdIndex = 1; pointIdIndex < (transversalPoints.size() - 1); pointIdIndex++) {
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

      std::vector<std::string> transversalPointsTopMiddle = {};
      std::vector<std::string> transversalPointsBottomMiddle = {};

      for (size_t pointIdIndex = 0; pointIdIndex < transversalPointsTop.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsTop[pointIdIndex]);
      }
      for (size_t pointIdIndex = 0; pointIdIndex < transversalPointsBottom.size(); pointIdIndex++) {
        transversalPointsBottomMiddle.push_back(transversalPointsBottom[pointIdIndex]);
      }
      for (size_t pointIdIndex = 0; pointIdIndex < transversalPointsMiddle.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
        transversalPointsBottomMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
      }

      transversalPointsTopMiddle.erase(
          std::find(transversalPointsTopMiddle.begin(), transversalPointsTopMiddle.end(), vertex2Id));
      transversalPointsBottomMiddle.erase(
          std::find(transversalPointsBottomMiddle.begin(), transversalPointsBottomMiddle.end(), vertex1Id));

      std::vector<std::string> leftTopPoints = {};
      std::vector<std::string> rightTopPoints = {};
      std::vector<std::string> leftBottomPoints = {};
      std::vector<std::string> rightBottomPoints = {};

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
        for (size_t pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          leftTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (size_t pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
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
        for (size_t pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (size_t pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }

      for (size_t point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 rightBottomPoints[point2Id], vertex2Id, transversalPointsTopMiddle[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit1;
            }
          }
        }
      }

    exit1 : {}

      for (size_t point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 leftBottomPoints[point2Id], vertex2Id, transversalPointsTopMiddle[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit2;
            }
          }
        }
      }

    exit2 : {}

      for (size_t point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id],
                                 rightBottomPoints[point2Id], vertex2Id, transversalPointsBottom[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit3;
            }
          }
        }
      }

    exit3 : {}

      for (size_t point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id], 
                                 leftBottomPoints[point2Id], vertex2Id, transversalPointsBottom[point4Id], 
                                 AngleType::CONVEX,
                                 IDependency::Reason::ALTERNATE_ANGLES, {}, 
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit4;
            }
          }
        }
      }

    exit4 : {}
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findCorrespondingAngles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  const std::vector<LineModel>& lines = shapesBank.getLinesVector();
  const std::vector<PointModel>& points = shapesBank.getPointsVector();

  for (size_t index1 = 0; index1 < parallelLines.size(); index1++) {
    const std::string& line1Id = parallelLines[index1]->getFirstObject().getId();
    const std::string& line2Id = parallelLines[index1]->getSecondObject().getId();

    if (line1Id == line2Id) {
      continue;
    }

    for (size_t index2 = 0; index2 < lines.size(); index2++) {
      const std::string& otherLineId = lines[index2].getId();
      
      if (line1Id == otherLineId || line2Id == otherLineId) {
        continue;
      }

      if (intersectionPointsOfLines[shapesBank.getLinePositionInVector(line1Id)][shapesBank.getLinePositionInVector(otherLineId)].empty() ||
          intersectionPointsOfLines[shapesBank.getLinePositionInVector(line2Id)][shapesBank.getLinePositionInVector(otherLineId)].empty()) {
        continue;
      }

      std::vector<std::string> parallel1Points = shapesBank.getLine(line1Id).getIncludedPoints();
      std::vector<std::string> parallel2Points = shapesBank.getLine(line2Id).getIncludedPoints();
      std::vector<std::string> transversalPoints = shapesBank.getLine(otherLineId).getIncludedPoints();

      if (parallel1Points.size() < 2 || parallel2Points.size() < 2 || transversalPoints.size() < 2) {
        continue;
      }

      const std::string& vertex1Id = points.at(intersectionPointsOfLines[shapesBank.getLinePositionInVector(line1Id)]
                                                                        [shapesBank.getLinePositionInVector(otherLineId)][0]).getId();
      const std::string& vertex2Id = points.at(intersectionPointsOfLines[shapesBank.getLinePositionInVector(line2Id)]
                                                                        [shapesBank.getLinePositionInVector(otherLineId)][0]).getId();

      const size_t vertex1IndexParallel1 =
          distance(parallel1Points.begin(), std::find(parallel1Points.begin(), parallel1Points.end(), vertex1Id));
      const size_t vertex2IndexParallel2 =
          distance(parallel2Points.begin(), std::find(parallel2Points.begin(), parallel2Points.end(), vertex2Id));
      const size_t vertex1IndexTransversal =
          distance(transversalPoints.begin(), std::find(transversalPoints.begin(), transversalPoints.end(), vertex1Id));
      const size_t vertex2IndexTransversal =
          distance(transversalPoints.begin(), std::find(transversalPoints.begin(), transversalPoints.end(), vertex2Id));

      const std::string& topReferenceVertex =
          vertex1IndexTransversal < vertex2IndexTransversal ? transversalPoints.front() : transversalPoints.back();
      const std::string& bottomReferenceVertex =
          vertex2IndexTransversal < vertex1IndexTransversal ? transversalPoints.front() : transversalPoints.back();

      const double topDistance = MathHelper::getDistance(shapesBank.getPoint(topReferenceVertex).getCoordinates(),
                                                         shapesBank.getPoint(vertex1Id).getCoordinates());

      const double bottomDistance = MathHelper::getDistance(shapesBank.getPoint(bottomReferenceVertex).getCoordinates(),
                                                            shapesBank.getPoint(vertex2Id).getCoordinates());

      std::vector<std::string> transversalPointsTop = {topReferenceVertex};
      std::vector<std::string> transversalPointsBottom = {bottomReferenceVertex};
      std::vector<std::string> transversalPointsMiddle = {};

      for (size_t pointIdIndex = 1; pointIdIndex < (transversalPoints.size() - 1); pointIdIndex++) {
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

      std::vector<std::string> transversalPointsTopMiddle = {};
      std::vector<std::string> transversalPointsBottomMiddle = {};

      for (size_t pointIdIndex = 0; pointIdIndex < transversalPointsTop.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsTop[pointIdIndex]);
      }
      for (size_t pointIdIndex = 0; pointIdIndex < transversalPointsBottom.size(); pointIdIndex++) {
        transversalPointsBottomMiddle.push_back(transversalPointsBottom[pointIdIndex]);
      }
      for (size_t pointIdIndex = 0; pointIdIndex < transversalPointsMiddle.size(); pointIdIndex++) {
        transversalPointsTopMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
        transversalPointsBottomMiddle.push_back(transversalPointsMiddle[pointIdIndex]);
      }

      transversalPointsTopMiddle.erase(
          std::find(transversalPointsTopMiddle.begin(), transversalPointsTopMiddle.end(), vertex2Id));
      transversalPointsBottomMiddle.erase(
          std::find(transversalPointsBottomMiddle.begin(), transversalPointsBottomMiddle.end(), vertex1Id));

      std::vector<std::string> leftTopPoints = {};
      std::vector<std::string> rightTopPoints = {};
      std::vector<std::string> leftBottomPoints = {};
      std::vector<std::string> rightBottomPoints = {};

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
        for (size_t pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          leftTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (size_t pointIdIndex = 0; pointIdIndex < vertex1IndexParallel1; pointIdIndex++) {
          rightTopPoints.push_back(parallel1Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex1IndexParallel1 + 1; pointIdIndex < parallel1Points.size(); pointIdIndex++) {
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
        for (size_t pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }
      else {  // obtuse angle
        for (size_t pointIdIndex = 0; pointIdIndex < vertex2IndexParallel2; pointIdIndex++) {
          leftBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
        for (size_t pointIdIndex = vertex2IndexParallel2 + 1; pointIdIndex < parallel2Points.size(); pointIdIndex++) {
          rightBottomPoints.push_back(parallel2Points[pointIdIndex]);
        }
      }

      for (size_t point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id],
                                 leftBottomPoints[point2Id],vertex2Id, transversalPointsTopMiddle[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit1;
            }
          }
        }
      }

    exit1 : {}

      for (size_t point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsTop.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsTopMiddle.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsTop[point3Id],
                                 rightBottomPoints[point2Id],vertex2Id, transversalPointsTopMiddle[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit2;
            }
          }
        }
      }

    exit2 : {}

      for (size_t point1Id = 0; point1Id < leftTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < leftBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(leftTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 leftBottomPoints[point2Id], vertex2Id, transversalPointsBottom[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit3;
            }
          }
        }
      }

    exit3 : {}

      for (size_t point1Id = 0; point1Id < rightTopPoints.size(); point1Id++) {
        for (size_t point2Id = 0; point2Id < rightBottomPoints.size(); point2Id++) {
          for (size_t point3Id = 0; point3Id < transversalPointsBottomMiddle.size(); point3Id++) {
            for (size_t point4Id = 0; point4Id < transversalPointsBottom.size(); point4Id++) {
              sumOfNewDependencies +=
                  setEqualAngles(rightTopPoints[point1Id], vertex1Id, transversalPointsBottomMiddle[point3Id],
                                 rightBottomPoints[point2Id], vertex2Id, transversalPointsBottom[point4Id],
                                 AngleType::CONVEX,
                                 IDependency::Reason::CORRESPONDING_ANGLES, {},
                                 IDependency::ImportanceLevel::MEDIUM);

              goto exit4;
            }
          }
        }
      }

    exit4 : {}
    }
  }

  return sumOfNewDependencies;
}
}  // namespace expert