#include "../Expert.h"

namespace expert {
unsigned int Expert::exploreLineBasedDependencies() {
  unsigned int sumOfNewDependencies{0};

  if(heuristicsBank.getNewPointsFlag() || heuristicsBank.getNewDependencyFlag(IDependency::Type::PERPENDICULAR_LINES)) {
    sumOfNewDependencies += setRightAnglesBasedOnPerpendicularLines();
  }

  if(heuristicsBank.getNewDependencyFlag(IDependency::Type::PARALLEL_LINES)) {
    sumOfNewDependencies += findParallelLinesBasedOnParallelLines();
  }

  if(heuristicsBank.getNewDependencyFlag(IDependency::Type::PERPENDICULAR_LINES)) {
    sumOfNewDependencies += findParallelLinesBasedOnPerpendicularLines();
  }

  if(heuristicsBank.getNewDependencyFlag(IDependency::Type::PERPENDICULAR_LINES) || heuristicsBank.getNewDependencyFlag(IDependency::Type::PARALLEL_LINES)) {
    sumOfNewDependencies += findPerpendicularLinesBasedOnLines();
  }

  if(heuristicsBank.getNewPointsFlag() || heuristicsBank.getNewLinesFlag() || heuristicsBank.getNewEvaluationFlag()) {
    sumOfNewDependencies += findPerpendicularLinesBasedOnRightAngles();
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setRightAnglesBasedOnPerpendicularLines() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::shared_ptr<LinesDependency>> perpendicularLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PERPENDICULAR_LINES);

  for (size_t index = 0; index < perpendicularLines.size(); index++) {
    const std::string& line1Id = perpendicularLines[index]->getFirstObject().getId();
    const std::string& line2Id = perpendicularLines[index]->getSecondObject().getId();

    const size_t line1IdPos{shapesBank.getLinePositionInVector(line1Id)};
    const size_t line2IdPos{shapesBank.getLinePositionInVector(line2Id)};

    if (shapesBank.getIntersectionPointsOfLines(line1IdPos, line2IdPos).empty()) {
      continue;
    }

    const size_t intersectionPoint = shapesBank.getIntersectionPointsOfLines(line1IdPos, line2IdPos)[0];
    const std::string& intersectionPointId = shapesBank.getPointsVector().at(intersectionPoint).getId();
    const std::vector<std::string>& line1Points = shapesBank.getLine(line1Id).getIncludedPoints();
    const std::vector<std::string>& line2Points = shapesBank.getLine(line2Id).getIncludedPoints();

    std::string linesEnds[4];
    linesEnds[0] = line1Points.front();
    linesEnds[1] = line2Points.front();
    linesEnds[2] = line1Points.back();
    linesEnds[3] = line2Points.back();

    for (size_t endNumber = 0; endNumber < 4; endNumber++) {
      if (linesEnds[endNumber] != intersectionPointId &&
          linesEnds[endNumber] != linesEnds[(endNumber + 1) % 4] &&
          linesEnds[(endNumber + 1) % 4] != intersectionPointId) {

        sumOfNewDependencies += dependenciesBank.addConvexAngle(
            linesEnds[endNumber], intersectionPointId, linesEnds[(endNumber + 1) % 4],
            Integer(MathHelper::RIGHT_ANGLE_VALUE),
            IDependency::Reason::PERPENDICULAR_LINES, {perpendicularLines[index]->getId()},
            IDependency::ImportanceLevel::MEDIUM);
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findParallelLinesBasedOnParallelLines() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  for (size_t index1 = 0; index1 < parallelLines.size(); index1++) {
    for (size_t index2 = index1 + 1; index2 < parallelLines.size(); index2++) {

      const std::string& line1Id = parallelLines[index1]->getFirstObject().getId();
      const std::string& line2Id = parallelLines[index1]->getSecondObject().getId();
      const std::string& line3Id = parallelLines[index2]->getFirstObject().getId();
      const std::string& line4Id = parallelLines[index2]->getSecondObject().getId();

      if (line1Id == line3Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line2Id, line4Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {parallelLines[index1]->getId(), parallelLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line1Id == line4Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line2Id, line3Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {parallelLines[index1]->getId(), parallelLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line2Id == line3Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line1Id, line4Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {parallelLines[index1]->getId(), parallelLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line2Id == line4Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line1Id, line3Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {parallelLines[index1]->getId(), parallelLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findParallelLinesBasedOnPerpendicularLines() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::shared_ptr<LinesDependency>> perpendicularLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PERPENDICULAR_LINES);

  for (size_t index1 = 0; index1 < perpendicularLines.size(); index1++) {
    for (size_t index2 = index1 + 1; index2 < perpendicularLines.size(); index2++) {

      const std::string& line1Id = perpendicularLines[index1]->getFirstObject().getId();
      const std::string& line2Id = perpendicularLines[index1]->getSecondObject().getId();
      const std::string& line3Id = perpendicularLines[index2]->getFirstObject().getId();
      const std::string& line4Id = perpendicularLines[index2]->getSecondObject().getId();

      if (line1Id == line3Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line2Id, line4Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {perpendicularLines[index1]->getId(), perpendicularLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line1Id == line4Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line2Id, line3Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {perpendicularLines[index1]->getId(), perpendicularLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line2Id == line3Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line1Id, line4Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {perpendicularLines[index1]->getId(), perpendicularLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line2Id == line4Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(
            line1Id, line3Id, LinesDependencies::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {perpendicularLines[index1]->getId(), perpendicularLines[index2]->getId()}, IDependency::ImportanceLevel::MEDIUM);
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findPerpendicularLinesBasedOnLines() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines = dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  const std::vector<std::shared_ptr<LinesDependency>> perpendicularLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PERPENDICULAR_LINES);

  for (size_t index1 = 0; index1 < perpendicularLines.size(); index1++) {
    for (size_t index2 = 0; index2 < parallelLines.size(); index2++) {
      const std::string& line1Id = perpendicularLines[index1]->getFirstObject().getId();
      const std::string& line2Id = perpendicularLines[index1]->getSecondObject().getId();
      const std::string& line3Id = parallelLines[index2]->getFirstObject().getId();
      const std::string& line4Id = parallelLines[index2]->getSecondObject().getId();

      if (line1Id == line3Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(line2Id, line4Id, LinesDependencies::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {perpendicularLines[index1]->getId(), parallelLines[index2]->getId()},
                                                      IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line1Id == line4Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(line2Id, line3Id, LinesDependencies::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {perpendicularLines[index1]->getId(), parallelLines[index2]->getId()},
                                                      IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line2Id == line3Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(line1Id, line4Id, LinesDependencies::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {perpendicularLines[index1]->getId(), parallelLines[index2]->getId()},
                                                      IDependency::ImportanceLevel::MEDIUM);
      }
      else if (line2Id == line4Id) {
        sumOfNewDependencies += dependenciesBank.addLinesDependency(line1Id, line3Id, LinesDependencies::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {perpendicularLines[index1]->getId(), parallelLines[index2]->getId()},
                                                      IDependency::ImportanceLevel::MEDIUM);
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findPerpendicularLinesBasedOnRightAngles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<LineModel> lines = shapesBank.getLinesVector();
  for (size_t line1Pos = 0; line1Pos < lines.size(); line1Pos++) {
    for (size_t line2Pos = line1Pos + 1; line2Pos < lines.size(); line2Pos++) {
      if (shapesBank.getIntersectionPointsOfLines(line1Pos, line2Pos).empty()) {
        continue;
      }

      const PointModel intersectionPoint = shapesBank.getPointsVector().at(shapesBank.getIntersectionPointsOfLines(line1Pos, line2Pos)[0]);
      const PointModel linesEnds[4] = {
        shapesBank.getPoint(lines.at(line1Pos).getIncludedPoints().front()),
        shapesBank.getPoint(lines.at(line2Pos).getIncludedPoints().front()),
        shapesBank.getPoint(lines.at(line1Pos).getIncludedPoints().back()),
        shapesBank.getPoint(lines.at(line2Pos).getIncludedPoints().back())
      };

      const Integer rightAngle(MathHelper::RIGHT_ANGLE_VALUE);
      std::pair<bool, std::vector<size_t>> evaluation = {false, {}};

      for (size_t endNumber = 0; endNumber < 4; endNumber++) {
        evaluation = dependenciesBank.evaluateEquation(
            ExpressionModel{dependenciesBank.getAngleMeasureVariable(linesEnds[endNumber], intersectionPoint, linesEnds[(endNumber + 1) % 4], true)},
            ExpressionModel{rightAngle});

        if (evaluation.first) {
          sumOfNewDependencies += dependenciesBank.addLinesDependency(lines.at(line1Pos).getId(), lines.at(line2Pos).getId(),
                                                                      LinesDependencies::PERPENDICULAR_LINES,
                                                                      IDependency::Reason::RIGHT_ANGLE, evaluation.second,
                                                                      IDependency::ImportanceLevel::MEDIUM);

          break;
        }
      }
    }
  }

  return 0;
}

}  // namespace expert