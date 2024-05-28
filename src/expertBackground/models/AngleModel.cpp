#include "AngleModel.h"

namespace expertBackground {
AngleModel::AngleModel(std::string vertexId, std::string point1Id, std::string point2Id, Type type)
    : arm1{std::move(point1Id), vertexId}, arm2{std::move(point2Id), vertexId}, type{type} {}

AngleModel::AngleModel(std::string arm1Point1Id, std::string arm1Point2Id, std::string arm2Point1Id, std::string arm2Point2Id, Type type)
    : arm1{std::move(arm1Point1Id), std::move(arm1Point2Id)}, arm2{std::move(arm2Point1Id), std::move(arm2Point2Id)}, type{type} {}

json AngleModel::getJson() const {
  return {{"arm1", {
           "end1", arm1.getPoint1Id(),
           "end2", arm1.getPoint2Id(),
          }},
          {"arm2", {
           "end1", arm2.getPoint1Id(),
           "end2", arm2.getPoint2Id(),
          }},
          {"type", static_cast<int>(type)}};
}

bool operator==(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return angleModel1.arm1 == angleModel2.arm1 && angleModel1.arm2 == angleModel2.arm2 && 
  (angleModel1.type == angleModel2.type || angleModel1.type == AngleModel::Type::UNKNOWN || angleModel2.type == AngleModel::Type::UNKNOWN);
}

bool operator!=(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return !(angleModel1 == angleModel2);
}

bool operator<(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  if(angleModel1.arm1 != angleModel2.arm1) {
    return angleModel1.arm1 < angleModel2.arm1;
  } else if(angleModel1.arm2 != angleModel2.arm2) {
    return angleModel1.arm2 < angleModel2.arm2;
  } else {
    return angleModel1.type < angleModel2.type;
  }
}

bool operator>(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return angleModel2 < angleModel1;
}

bool operator<=(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return angleModel1 < angleModel2 || angleModel1 == angleModel2;
}

bool operator>=(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return angleModel1 > angleModel2 || angleModel1 == angleModel2;
}

std::ostream& operator<<(std::ostream& stream, const AngleModel& angleModel) {
  stream << "<)" << angleModel.arm1 << ", " << angleModel.arm2;
  switch (angleModel.type) {
    case AngleModel::Type::CONVEX:
      stream << " is convex";
      break;
    case AngleModel::Type::CONCAVE:
      stream << " is concave";
      break;
    default:
      stream << " is unknown";
      break;
  }

  return stream;
}

json& operator<<(json& j, const AngleModel& angleModel) {
  j = angleModel.getJson();

  return j;
}
}  // namespace expertBackground