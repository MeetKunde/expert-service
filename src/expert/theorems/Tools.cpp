#include "Expert.h"

namespace expert {
bool Expert::pointsLiesOnOneLine(const std::vector<size_t>& pointPoses) const {
  if (pointPoses.size() < 2) {
    return true;
  }

  const std::vector<PointModel>& points = shapesBank.getPointsVector();
  const std::vector<LineModel>& lines = shapesBank.getLinesVector();

  for (size_t linePos = 0; linePos < lines.size(); linePos++) {
    const std::vector<std::string>& pointsOnLine = lines.at(linePos).getIncludedPoints();
    bool allPointsAreIncluded = true;
    for (const size_t & pointPos : pointPoses) {
      if (std::find(pointsOnLine.begin(), pointsOnLine.end(), points.at(pointPos).getId()) == pointsOnLine.end()) {
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

unsigned int Expert::setEqualSides(const std::string& segment1End1Id, const std::string& segment1End2Id,
                           const std::string& segment2End1Id, const std::string& segment2End2Id,
                           IDependency::Reason reason, const std::vector<size_t>& dependentDependencies,
                           IDependency::ImportanceLevel importanceLevel) {

  const symbolicAlgebra::Variable segment1 = dependenciesBank.getSegmentLengthVariable(shapesBank.getPoint(segment1End1Id), shapesBank.getPoint(segment1End2Id));
  const symbolicAlgebra::Variable segment2 = dependenciesBank.getSegmentLengthVariable(shapesBank.getPoint(segment2End1Id), shapesBank.getPoint(segment2End2Id));

  return dependenciesBank.addEquation(segment1, segment2, reason, dependentDependencies, importanceLevel);

  //return dependenciesBank.addPointsPairsDependency(segment1End1Id, segment1End2Id,
  //                                                 segment2End1Id, segment2End2Id,
  //                                                 PointsPairsDependencies::EQUAL_SEGMENTS, reason, dependentDependencies, importanceLevel);
}

unsigned int Expert::setEqualAngles(const std::string& angle1Point1, const std::string& angle1Vertex, const std::string& angle1Point2,
                            const std::string& angle2Point1, const std::string& angle2Vertex, const std::string& angle2Point2,
                            AngleType /*anglesType*/,
                            IDependency::Reason reason, const std::vector<size_t>& dependentDependencies, IDependency::ImportanceLevel importanceLevel) {

  const symbolicAlgebra::Variable angle1 = dependenciesBank.getAngleMeasureVariable(shapesBank.getPoint(angle1Point1), shapesBank.getPoint(angle1Vertex), shapesBank.getPoint(angle1Point2), true);
  const symbolicAlgebra::Variable angle2 = dependenciesBank.getAngleMeasureVariable(shapesBank.getPoint(angle2Point1), shapesBank.getPoint(angle2Vertex), shapesBank.getPoint(angle2Point2), true);

  return dependenciesBank.addEquation(angle1, angle2, reason, dependentDependencies, importanceLevel);

  //return dependenciesBank.addAnglesDependency(angle1Point1, angle1Vertex, angle1Point2, anglesType,
  //                                            angle2Point1, angle2Vertex, angle2Point2, anglesType,
  //                                            AnglesDependencies::EQUAL_ANGLES, reason, dependentDependencies, importanceLevel);
}

unsigned int Expert::setSidesParallelism(const std::string& segment1End1Id, const std::string& segment1End2Id,
                                 const std::string& segment2End1Id, const std::string& segment2End2Id,
                                 IDependency::Reason reason, const std::vector<size_t>& dependentDependencies,
                                 IDependency::ImportanceLevel importanceLevel) {

  return dependenciesBank.addLinesDependency(shapesBank.getLineIdThrowTwoPoints(segment1End1Id, segment1End2Id),
                                             shapesBank.getLineIdThrowTwoPoints(segment2End1Id, segment2End2Id),
                                             LinesDependencies::PARALLEL_LINES, reason, dependentDependencies, importanceLevel);
}

unsigned int Expert::setSidesPerpendicularity(const std::string& segment1End1Id, const std::string& segment1End2Id,
                                      const std::string& segment2End1Id, const std::string&segment2End2Id,
                                      IDependency::Reason reason, const std::vector<size_t>& dependentDependencies,
                                      IDependency::ImportanceLevel importanceLevel) {

    return dependenciesBank.addLinesDependency(shapesBank.getLineIdThrowTwoPoints(segment1End1Id, segment1End2Id),
                                               shapesBank.getLineIdThrowTwoPoints(segment2End1Id, segment2End2Id),
                                               LinesDependencies::PERPENDICULAR_LINES, reason, dependentDependencies, importanceLevel);
}

}  // namespace expert