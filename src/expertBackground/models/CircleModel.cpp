#include "CircleModel.h"

namespace expertBackground {
CircleModel::CircleModel(std::string identifier, std::string centerId, float radius, std::vector<std::string> includedPoints)
    : id{std::move(identifier)}, centerId{std::move(centerId)} , radius{radius}, includedPointIds{std::move(includedPoints)} {}

json CircleModel::getJson() const {
  return {{"id", id}, 
          {"centerId", centerId}, 
          {"r", radius}, 
          {"pointsOn", json(includedPointIds)}};
}

bool operator==(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return circleModel1.centerId == circleModel2.centerId &&
         fabs(circleModel1.radius - circleModel2.radius) < MathHelper::COMPARISON_EPSILON;
}

bool operator!=(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return !(circleModel1 == circleModel2);
}

bool operator<(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  if (circleModel1.centerId == circleModel2.centerId) {
    return circleModel1.radius < circleModel2.radius;
  }

  return circleModel1.centerId < circleModel2.centerId;
}

bool operator>(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return circleModel2 < circleModel1;
}

bool operator<=(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return !(circleModel1 > circleModel2);
}

bool operator>=(const CircleModel& circleModel1, const CircleModel& circleModel2) {
  return !(circleModel1 < circleModel2);
}

std::ostream& operator<<(std::ostream& stream, const CircleModel& circleModel) {
  stream << "O" << circleModel.centerId << " r = " << circleModel.radius;

  return stream;
}

json& operator<<(json& j, const CircleModel& circleModel) {
  j = circleModel.getJson();

  return j;
}
}  // namespace expertBackground