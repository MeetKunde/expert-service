#include "AngleModel.h"

namespace expertBackground {
AngleModel::AngleModel(std::string vertexId, std::string point1Id, std::string point2Id, Type type)
    : vertexId{std::move(vertexId)}, pointsOnArms{std::move(point1Id), std::move(point2Id)}, type{type} {
}

AngleModel::AngleModel(std::string vertexId, std::string point1Id, std::string point2Id, std::string arm1, std::string arm2, Type type)
    : vertexId{std::move(vertexId)}, pointsOnArms{std::move(point1Id), std::move(point2Id)}, type{type} {
    if(pointsOnArms.getPoint1Id() == point1Id) {
        this->arm1 = arm1;
        this->arm2 = arm2;
    } else {
        this->arm1 = arm2;
        this->arm2 = arm1;
    }
}

json AngleModel::getJson() const {
  return {{"point1Id", pointsOnArms.getPoint1Id()},
          {"vertexId", vertexId},
          {"point2Id", pointsOnArms.getPoint2Id()},
          {"type", static_cast<int>(type)}};
}

bool operator==(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  if(angleModel1.hasTwoArms() && angleModel2.hasTwoArms()) {
    return angleModel1.arm1.value() == angleModel2.arm2.value() && angleModel1.vertexId == angleModel2.vertexId && 
           (angleModel1.type == angleModel2.type || angleModel1.type == AngleModel::Type::UNKNOWN || 
            angleModel2.type == AngleModel::Type::UNKNOWN);
  }
  else if(angleModel1.arm1.has_value() && angleModel2.arm1.has_value()) {
    return angleModel1.arm1.value() == angleModel2.arm1.value() && angleModel1.vertexId == angleModel2.vertexId && 
           (angleModel1.type == angleModel2.type || angleModel1.type == AngleModel::Type::UNKNOWN || 
            angleModel2.type == AngleModel::Type::UNKNOWN);
  }
  else if(angleModel1.arm2.has_value() && angleModel2.arm2.has_value()) {
    return angleModel1.arm2.value() == angleModel2.arm2.value() && angleModel1.vertexId == angleModel2.vertexId && 
           (angleModel1.type == angleModel2.type || angleModel1.type == AngleModel::Type::UNKNOWN || 
            angleModel2.type == AngleModel::Type::UNKNOWN);
  }
  else {
    return angleModel1.pointsOnArms == angleModel2.pointsOnArms && angleModel1.vertexId == angleModel2.vertexId &&
           (angleModel1.type == angleModel2.type || angleModel1.type == AngleModel::Type::UNKNOWN ||
            angleModel2.type == AngleModel::Type::UNKNOWN);
  }
}

bool operator!=(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return !(angleModel1 == angleModel2);
}

bool operator<(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  if(angleModel1.hasTwoArms() && angleModel2.hasTwoArms()) {
    if(angleModel1.arm1.value() != angleModel2.arm1.value()) {
      return angleModel1.arm1.value() < angleModel2.arm2.value();
    }
    else if(angleModel1.arm2.value() != angleModel2.arm2.value()) {
      return angleModel1.arm2.value() < angleModel2.arm2.value();
    }
    else if(angleModel1.vertexId != angleModel2.vertexId) {
      return angleModel1.vertexId < angleModel2.vertexId;
    }
    else {
      return angleModel1.type < angleModel2.type;
    }
  }
  else if(angleModel1.arm1.has_value() && angleModel2.arm1.has_value()) {
    if(angleModel1.arm1.value() != angleModel2.arm1.value()) {
      return angleModel1.arm1.value() < angleModel2.arm1.value();
    }
    else if(angleModel1.vertexId != angleModel2.vertexId) {
      return angleModel1.vertexId < angleModel2.vertexId;
    }
    else {
      return angleModel1.type < angleModel2.type;
    }
  }
  else if(angleModel1.arm2.has_value() && angleModel2.arm2.has_value()) {
    if(angleModel1.arm2.value() != angleModel2.arm2.value()) {
      return angleModel1.arm2.value() < angleModel2.arm2.value();
    }
    else if(angleModel1.vertexId != angleModel2.vertexId) {
      return angleModel1.vertexId < angleModel2.vertexId;
    }
    else {
      return angleModel1.type < angleModel2.type;
    }
  }
  else {
    if(angleModel1.pointsOnArms != angleModel2.pointsOnArms) {
      return angleModel1.pointsOnArms < angleModel2.pointsOnArms;
    }
    else if(angleModel1.vertexId != angleModel2.vertexId) {
      return angleModel1.vertexId < angleModel2.vertexId;
    }
    else {
      return angleModel1.type < angleModel2.type;
    }
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
  stream << "<)" << angleModel.getPoint1Id() << "-" << angleModel.vertexId << "-" << angleModel.getPoint2Id();
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