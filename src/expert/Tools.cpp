#include "Expert.h"

namespace expert {
void Expert::printUnsignedIntVector(const std::vector<unsigned int>& vec) {
  for (const unsigned int element : vec) {
    std::cout << static_cast<char>(element + static_cast<int>('A')) << " ";
  }
  std::cout << std::endl;
}

bool Expert::pointsLiesOnOneLine(const std::vector<unsigned int>& points) {
  if (points.size() < 2) {
    return true;
  }

  for (unsigned int lineId = 0; lineId < linesNumber; lineId++) {
    std::vector<unsigned int> pointsOnLine = shapesBank.getLine(lineId).getIncludedPoints();
    bool allPointsAreIncluded = true;
    for (const unsigned int pointId : points) {
      if (std::find(pointsOnLine.begin(), pointsOnLine.end(), pointId) == pointsOnLine.end()) {
        allPointsAreIncluded = false;
        break;
      }
    }

    if (allPointsAreIncluded) {
      return true;
    }
  }

  return false;
}

unsigned int Expert::setEqualSides(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                   unsigned int segment2End2, IDependency::Reason reason,
                                   IDependency::UsefulnessLevel usefulness) {

  const Variable var1(expertBackground::DependenciesBank::getLengthVariable(segment1End1, segment1End2));
  const Variable var2(expertBackground::DependenciesBank::getLengthVariable(segment2End1, segment2End2));

  return dependenciesBank.addEquation(var1, var2, reason, {}, usefulness);
}

unsigned int Expert::setEqualSides(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                   unsigned int segment2End2, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                   IDependency::UsefulnessLevel usefulness) {

  const Variable var1(expertBackground::DependenciesBank::getLengthVariable(segment1End1, segment1End2));
  const Variable var2(expertBackground::DependenciesBank::getLengthVariable(segment2End1, segment2End2));

  return dependenciesBank.addEquation(var1, var2, reason, std::move(basedOn), usefulness);
}

unsigned int Expert::setEqualAngles(unsigned int angle1Point1, unsigned int angle1Vertex, unsigned int angle1Point2,
                                    unsigned int angle2Point1, unsigned int angle2Vertex, unsigned int angle2Point2,
                                    AngleType /*anglesType*/, IDependency::Reason reason,
                                    IDependency::UsefulnessLevel usefulness) {

  const Variable var1(dependenciesBank.getConvexAngleVariable(angle1Point1, angle1Vertex, angle1Point2));
  const Variable var2(dependenciesBank.getConvexAngleVariable(angle2Point1, angle2Vertex, angle2Point2));

  return dependenciesBank.addEquation(var1, var2, reason, {}, usefulness);
}

unsigned int Expert::setEqualAngles(unsigned int angle1Point1, unsigned int angle1Vertex, unsigned int angle1Point2,
                                    unsigned int angle2Point1, unsigned int angle2Vertex, unsigned int angle2Point2,
                                    AngleType /*anglesType*/, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                    IDependency::UsefulnessLevel usefulness) {

  const Variable var1(dependenciesBank.getConvexAngleVariable(angle1Point1, angle1Vertex, angle1Point2));
  const Variable var2(dependenciesBank.getConvexAngleVariable(angle2Point1, angle2Vertex, angle2Point2));

  return dependenciesBank.addEquation(var1, var2, reason, std::move(basedOn), usefulness);
}

unsigned int Expert::setSidesParallelism(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                         unsigned int segment2End2, IDependency::Reason reason,
                                         IDependency::UsefulnessLevel usefulness) {
  return dependenciesBank.addLinesDependency(shapesBank.getLineIdThrowTwoPoints(segment1End1, segment1End2),
                                             shapesBank.getLineIdThrowTwoPoints(segment2End1, segment2End2),
                                             IDependency::Type::PARALLEL_LINES, reason, {}, usefulness);
}

unsigned int Expert::setSidesParallelism(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                         unsigned int segment2End2, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                         IDependency::UsefulnessLevel usefulness) {
  return dependenciesBank.addLinesDependency(shapesBank.getLineIdThrowTwoPoints(segment1End1, segment1End2),
                                             shapesBank.getLineIdThrowTwoPoints(segment2End1, segment2End2),
                                             IDependency::Type::PARALLEL_LINES, reason, std::move(basedOn), usefulness);
}

unsigned int Expert::setSidesPerpendicularity(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                              unsigned int segment2End2, IDependency::Reason reason,
                                              IDependency::UsefulnessLevel usefulness) {
  return dependenciesBank.addLinesDependency(shapesBank.getLineIdThrowTwoPoints(segment1End1, segment1End2),
                                             shapesBank.getLineIdThrowTwoPoints(segment2End1, segment2End2),
                                             IDependency::Type::PERPENDICULAR_LINES, reason, {}, usefulness);
}

unsigned int Expert::setSidesPerpendicularity(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                              unsigned int segment2End2, IDependency::Reason reason,
                                              std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness) {
  return dependenciesBank.addLinesDependency(shapesBank.getLineIdThrowTwoPoints(segment1End1, segment1End2),
                                             shapesBank.getLineIdThrowTwoPoints(segment2End1, segment2End2),
                                             IDependency::Type::PERPENDICULAR_LINES, reason, std::move(basedOn), usefulness);
}

}  // namespace expert