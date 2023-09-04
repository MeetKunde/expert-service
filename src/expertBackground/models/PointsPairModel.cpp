#include "PointsPairModel.h"

namespace expertBackground {
PointsPairModel::PointsPairModel(std::string point1, std::string point2) {
  if (point1 < point2) {
    point1Id = point1;
    point2Id = point2;
  }
  else {
    point1Id = point2;
    point2Id = point1;
  }
}

PointsPairModel::PointsPairModel(const PointsPairModel& pointsPair)
    : point1Id{pointsPair.point1Id}, point2Id{pointsPair.point2Id} {}

PointsPairModel& PointsPairModel::operator=(const PointsPairModel& pointsPair) {
  point1Id = pointsPair.point1Id;
  point2Id = pointsPair.point2Id;

  return *this;
}

json PointsPairModel::getJsonObject() const {
  return {{"end1Id", point1Id}, {"end2Id", point2Id}};
}

bool operator==(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  return pointsPair1.point1Id == pointsPair2.point1Id && pointsPair1.point2Id == pointsPair2.point2Id;
}

bool operator!=(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  return !(pointsPair1 == pointsPair2);
}

bool operator<(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  if (pointsPair1.point1Id == pointsPair2.point1Id) {
    return pointsPair1.point2Id < pointsPair2.point2Id;
  }

  return pointsPair1.point1Id < pointsPair2.point1Id;
}

bool operator>(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  return pointsPair2 < pointsPair1;
}

bool operator<=(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  return pointsPair1 < pointsPair2 || pointsPair1 == pointsPair2;
}

bool operator>=(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  return pointsPair1 > pointsPair2 || pointsPair1 == pointsPair2;
}

std::ostream& operator<<(std::ostream& stream, const PointsPairModel& pointsPair) {
  stream << "(" << pointsPair.point1Id << ", " << pointsPair.point2Id << ")";

  return stream;
}
PointsPairModel::~PointsPairModel() {}
}  // namespace expertBackground