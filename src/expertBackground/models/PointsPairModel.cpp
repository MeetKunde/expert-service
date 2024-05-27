#include "PointsPairModel.h"

namespace expertBackground {
PointsPairModel::PointsPairModel(std::string point1, std::string point2) {
  if (point1 < point2) {
    this->point1Id = std::move(point1);
    this->point2Id = std::move(point2);
  }
  else {
    this->point1Id = std::move(point2);
    this->point2Id = std::move(point1);
  }
}

json PointsPairModel::getJson() const {
  return {{"point1Id", point1Id}, {"point2Id", point2Id}};
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
  return !(pointsPair1 > pointsPair2);
}

bool operator>=(const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2) {
  return !(pointsPair1 < pointsPair2);
}

std::ostream& operator<<(std::ostream& stream, const PointsPairModel& pointsPair) {
  stream << "(" << pointsPair.point1Id << ", " << pointsPair.point2Id << ")";

  return stream;
}

json& operator<<(json& j, const PointsPairModel& pointsPair) {
  j = pointsPair.getJson();

  return j;
}
}  // namespace expertBackground