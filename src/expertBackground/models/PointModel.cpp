#include "PointModel.h"

namespace expertBackground {
PointModel::PointModel(std::string identifier, float xCoordinate, float yCoordinate, std::string name)
    : id{std::move(identifier)}, x{xCoordinate}, y{yCoordinate}, name{std::move(name)} {}

PointModel::PointModel(const PointModel& pointModel) : id{pointModel.id}, x{pointModel.x}, y{pointModel.y}, name{pointModel.name} {}

PointModel& PointModel::operator=(const PointModel& pointModel) {
  id = pointModel.id;
  x = pointModel.x;
  y = pointModel.y;

  return *this;
}

json PointModel::getJsonObject() const {
  return {{"id", id}, {"x", x}, {"y", y}, {"name", name}};
}

bool operator==(const PointModel& pointModel1, const PointModel& pointModel2) {
  return fabs(pointModel1.x - pointModel2.x) < MathHelper::COMPARISON_EPSILON &&
         fabs(pointModel1.y - pointModel2.y) < MathHelper::COMPARISON_EPSILON;
}

bool operator!=(const PointModel& pointModel1, const PointModel& pointModel2) {
  return !(pointModel1 == pointModel2);
}

bool operator<(const PointModel& pointModel1, const PointModel& pointModel2) {
  if (fabs(pointModel1.x - pointModel2.x) < MathHelper::COMPARISON_EPSILON) {
    return pointModel1.y < pointModel2.y;
  }

  return pointModel1.x < pointModel2.x;
}

bool operator>(const PointModel& pointModel1, const PointModel& pointModel2) {
  return pointModel2 < pointModel1;
}

bool operator<=(const PointModel& pointModel1, const PointModel& pointModel2) {
  return pointModel1 < pointModel2 || pointModel1 == pointModel2;
}

bool operator>=(const PointModel& pointModel1, const PointModel& pointModel2) {
  return pointModel1 > pointModel2 || pointModel1 == pointModel2;
}

std::ostream& operator<<(std::ostream& stream, const PointModel& pointModel) {
  stream << "(" << pointModel.x << ", " << pointModel.y << ")";

  return stream;
}
PointModel::~PointModel() {}
}  // namespace expertBackground