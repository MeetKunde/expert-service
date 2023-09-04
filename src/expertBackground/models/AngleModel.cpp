#include "AngleModel.h"

namespace expertBackground {
AngleModel::AngleModel(std::string vertexId, std::string point1Id, std::string point2Id, AngleType type)
    : vertexId{std::move(vertexId)}, pointsOnArms{std::move(point1Id), std::move(point2Id)}, angleType(type) {}

AngleModel::AngleModel(const AngleModel& angleModel)
    : vertexId{angleModel.vertexId}, pointsOnArms{PointsPairModel{angleModel.pointsOnArms}}, angleType{angleModel.angleType} {}

AngleModel& AngleModel::operator=(const AngleModel& angleModel) {
  vertexId = angleModel.vertexId;
  pointsOnArms = PointsPairModel(angleModel.pointsOnArms);
  angleType = angleModel.angleType;

  return *this;
}

json AngleModel::getJsonObject() const {
  return {{"point1Id", pointsOnArms.getPoint1Id()},
          {"vertexId", vertexId},
          {"point2Id", pointsOnArms.getPoint2Id()},
          {"type", angleType}};
}

bool operator==(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return angleModel1.pointsOnArms == angleModel2.pointsOnArms && angleModel1.vertexId == angleModel2.vertexId &&
         (angleModel1.angleType == angleModel2.angleType || angleModel1.angleType == AngleModel::AngleType::UNKNOWN ||
          angleModel2.angleType == AngleModel::AngleType::UNKNOWN);
}

bool operator!=(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  return !(angleModel1 == angleModel2);
}

bool operator<(const AngleModel& angleModel1, const AngleModel& angleModel2) {
  if (angleModel1.vertexId == angleModel2.vertexId && angleModel1.pointsOnArms == angleModel2.pointsOnArms) {
    return angleModel1.angleType < angleModel2.angleType;
  }

  if (angleModel1.vertexId == angleModel2.vertexId) {
    return angleModel1.pointsOnArms < angleModel2.pointsOnArms;
  }

  return angleModel1.vertexId < angleModel2.vertexId;
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
  switch (angleModel.angleType) {
    case AngleModel::AngleType::CONVEX:
      stream << " is convex";
      break;
    case AngleModel::AngleType::CONCAVE:
      stream << " is concave";
      break;
    default:
      stream << " is unknown";
      break;
  }

  return stream;
}
AngleModel::~AngleModel() {}
}  // namespace expertBackground