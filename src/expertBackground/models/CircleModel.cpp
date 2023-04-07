#include "CircleModel.h"

namespace expertBackground {
CircleModel::CircleModel(unsigned int identifier, unsigned int centerId, float centerX, float centerY, float radius,
                         std::vector<unsigned int> includedPoints)
    : id(identifier), centerPoint(centerId, centerX, centerY), radius(radius), includedPointIds(std::move(includedPoints)) {}

CircleModel::CircleModel(const CircleModel& circleModel)
    : id(circleModel.id),
      centerPoint(circleModel.centerPoint),
      radius(circleModel.radius),
      includedPointIds(circleModel.includedPointIds) {}

CircleModel& CircleModel::operator=(const CircleModel& circleModel) {
  id = circleModel.id;
  centerPoint = circleModel.centerPoint;
  radius = circleModel.radius;
  includedPointIds = circleModel.includedPointIds;

  return *this;
}

json CircleModel::getJsonObject() const {
  return {
      {"id", id},         {"centerId", centerPoint.getId()},   {"centerX", centerPoint.getX()}, {"centerY", centerPoint.getY()},
      {"radius", radius}, {"pointsOn", json(includedPointIds)}};
}

bool operator==(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return circleModel1.centerPoint == circleModel2.centerPoint &&
         fabs(circleModel1.radius - circleModel2.radius) < MathHelper::COMPARISON_EPSILON;
}

bool operator!=(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return !(circleModel1 == circleModel2);
}

bool operator<(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  if (circleModel1.centerPoint == circleModel2.centerPoint) {
    return circleModel1.radius < circleModel2.radius;
  }

  return circleModel1.centerPoint < circleModel2.centerPoint;
}

bool operator>(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return circleModel2 < circleModel1;
}

bool operator<=(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return circleModel1 < circleModel2 || circleModel1 == circleModel2;
}

bool operator>=(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return circleModel1 > circleModel2 || circleModel1 == circleModel2;
}

std::ostream& operator<<(std::ostream& stream, const CircleModel& circleModel) {
  stream << circleModel.centerPoint << " r = " << circleModel.radius;

  return stream;
}
CircleModel::~CircleModel() {}
}  // namespace expertBackground