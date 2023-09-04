#include "../Expert.h"
/*
namespace expert {
unsigned int Expert::setRightAnglesBasedOnPerpendicularities() {
  unsigned int result = 0;

  const std::vector<std::shared_ptr<LinesDependency>> perpendicularLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PERPENDICULAR_LINES);

  for (const auto& dependency : perpendicularLines) {
    const unsigned int line1Id = dependency->getFirstElement().getId();
    const unsigned int line2Id = dependency->getSecondElement().getId();

    if (intersectionPointsOfLines[line1Id][line2Id].empty()) {
      continue;
    }

    const unsigned int intersectionPoint = intersectionPointsOfLines[line1Id][line2Id][0];
    std::vector<unsigned int> line1Points = shapesBank.getLine(line1Id).getIncludedPoints();
    std::vector<unsigned int> line2Points = shapesBank.getLine(line2Id).getIncludedPoints();

    unsigned int linesEnds[4];
    linesEnds[0] = line1Points.front();
    linesEnds[1] = line2Points.front();
    linesEnds[2] = line1Points.back();
    linesEnds[3] = line2Points.back();

    for (unsigned int endNumber = 0; endNumber < 4; endNumber++) {
      if (linesEnds[endNumber] != intersectionPoint && linesEnds[endNumber] != linesEnds[(endNumber + 1) % 4] &&
          linesEnds[(endNumber + 1) % 4] != intersectionPoint) {
        result += dependenciesBank.addConvexAngle(
            linesEnds[endNumber], intersectionPoint, linesEnds[(endNumber + 1) % 4], Integer(MathHelper::RIGHT_ANGLE_VALUE),
            IDependency::Reason::PERPENDICULAR_LINES, {dependency->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
    }
  }

  return result;
}

unsigned int Expert::findParallelLinesBasedOnParallelLines() {
  unsigned int result = 0;

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  for (std::vector<std::shared_ptr<LinesDependency>>::const_iterator dependency1 = parallelLines.begin();
       dependency1 != parallelLines.end(); ++dependency1) {
    for (std::vector<std::shared_ptr<LinesDependency>>::const_iterator dependency2 = dependency1 + 1;
         dependency2 != parallelLines.end(); ++dependency2) {

      const unsigned int line1Id = (*dependency1)->getFirstElement().getId();
      const unsigned int line2Id = (*dependency1)->getSecondElement().getId();
      const unsigned int line3Id = (*dependency2)->getFirstElement().getId();
      const unsigned int line4Id = (*dependency2)->getSecondElement().getId();

      if (line1Id == line3Id) {
        result += dependenciesBank.addLinesDependency(
            line2Id, line4Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line1Id == line4Id) {
        result += dependenciesBank.addLinesDependency(
            line2Id, line3Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line2Id == line3Id) {
        result += dependenciesBank.addLinesDependency(
            line1Id, line4Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line2Id == line4Id) {
        result += dependenciesBank.addLinesDependency(
            line1Id, line3Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PARALLELISM_TRANSITIVITY,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
    }
  }

  return result;
}

unsigned int Expert::findParallelLinesBasedOnPerpendicularLines() {
  unsigned int result = 0;

  std::vector<std::shared_ptr<LinesDependency>> perpendicularLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PERPENDICULAR_LINES);

  for (std::vector<std::shared_ptr<LinesDependency>>::const_iterator dependency1 = perpendicularLines.begin(); dependency1 != perpendicularLines.end();
       ++dependency1) {
    for (std::vector<std::shared_ptr<LinesDependency>>::const_iterator dependency2 = dependency1 + 1; dependency2 != perpendicularLines.end();
         ++dependency2) {

      const unsigned int line1Id = (*dependency1)->getFirstElement().getId();
      const unsigned int line2Id = (*dependency1)->getSecondElement().getId();
      const unsigned int line3Id = (*dependency2)->getFirstElement().getId();
      const unsigned int line4Id = (*dependency2)->getSecondElement().getId();

      if (line1Id == line3Id) {
        result += dependenciesBank.addLinesDependency(
            line2Id, line4Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line1Id == line4Id) {
        result += dependenciesBank.addLinesDependency(
            line2Id, line3Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line2Id == line3Id) {
        result += dependenciesBank.addLinesDependency(
            line1Id, line4Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line2Id == line4Id) {
        result += dependenciesBank.addLinesDependency(
            line1Id, line3Id, IDependency::Type::PARALLEL_LINES, IDependency::Reason::PERPENDICULARITY_COMPOSITION,
            {(*dependency1)->getId(), (*dependency2)->getId()}, IDependency::UsefulnessLevel::MEDIUM);
      }
    }
  }

  return result;
}

unsigned int Expert::findPerpendicularLinesBasedOnLines() {
  unsigned int result = 0;

  const std::vector<std::shared_ptr<LinesDependency>> parallelLines = dependenciesBank.getLinesDependencies(LinesDependencies::PARALLEL_LINES);

  const std::vector<std::shared_ptr<LinesDependency>> perpendicularLines =
      dependenciesBank.getLinesDependencies(LinesDependencies::PERPENDICULAR_LINES);

  for (const auto& dependency1 : perpendicularLines) {
    for (const auto& dependency2 : parallelLines) {
      const unsigned int line1Id = dependency1->getFirstElement().getId();
      const unsigned int line2Id = dependency1->getSecondElement().getId();
      const unsigned int line3Id = dependency2->getFirstElement().getId();
      const unsigned int line4Id = dependency2->getSecondElement().getId();

      if (line1Id == line3Id) {
        result += dependenciesBank.addLinesDependency(line2Id, line4Id, IDependency::Type::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {dependency1->getId(), dependency2->getId()},
                                                      IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line1Id == line4Id) {
        result += dependenciesBank.addLinesDependency(line2Id, line3Id, IDependency::Type::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {dependency1->getId(), dependency2->getId()},
                                                      IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line2Id == line3Id) {
        result += dependenciesBank.addLinesDependency(line1Id, line4Id, IDependency::Type::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {dependency1->getId(), dependency2->getId()},
                                                      IDependency::UsefulnessLevel::MEDIUM);
      }
      else if (line2Id == line4Id) {
        result += dependenciesBank.addLinesDependency(line1Id, line3Id, IDependency::Type::PERPENDICULAR_LINES,
                                                      IDependency::Reason::PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
                                                      {dependency1->getId(), dependency2->getId()},
                                                      IDependency::UsefulnessLevel::MEDIUM);
      }
    }
  }

  return result;
}

unsigned int Expert::findPerpendicularLinesBasedOnRightAngles() {

  unsigned int result = 0;

  for (unsigned int line1Id = 0; line1Id < linesNumber; line1Id++) {
    for (unsigned int line2Id = line1Id + 1; line2Id < linesNumber; line2Id++) {
      if (intersectionPointsOfLines[line1Id][line2Id].empty()) {
        continue;
      }

      const unsigned int intersectionPoint = intersectionPointsOfLines[line1Id][line2Id][0];
      unsigned int linesEnds[4];
      linesEnds[0] = shapesBank.getLine(line1Id).getIncludedPoints().front();
      linesEnds[1] = shapesBank.getLine(line2Id).getIncludedPoints().front();
      linesEnds[2] = shapesBank.getLine(line1Id).getIncludedPoints().back();
      linesEnds[3] = shapesBank.getLine(line2Id).getIncludedPoints().back();

      const Integer rigthAngle(MathHelper::RIGHT_ANGLE_VALUE);

      std::pair<EvaluationResult, std::vector<unsigned int>> evaluation;
      for (unsigned int endNumber = 0; endNumber < 4; endNumber++) {
        evaluation = dependenciesBank.evaluateEquation(
            dependenciesBank.getConvexAngleVariable(linesEnds[endNumber], intersectionPoint, linesEnds[(endNumber + 1) % 4]),
            rigthAngle);
        if (evaluation.first == EvaluationResult::SUCCESS) {
          result += dependenciesBank.addLinesDependency(line1Id, line2Id, IDependency::Type::PERPENDICULAR_LINES,
                                                        IDependency::Reason::RIGHT_ANGLE, evaluation.second,
                                                        IDependency::UsefulnessLevel::MEDIUM);

          break;
        }
      }
    }
  }

  return 0;
}

}  // namespace expert
*/