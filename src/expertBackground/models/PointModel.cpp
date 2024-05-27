#include "PointModel.h"

namespace expertBackground {
PointModel::PointModel(std::string identifier, float xCoordinate, float yCoordinate, std::string name)
    : id{std::move(identifier)}, x{xCoordinate}, y{yCoordinate}, name{std::move(name)} {}

json PointModel::getJson() const {
  return {{"id", id}, 
          {"x", x}, 
          {"y", y}, 
          {"name", name}};
}

bool operator==(const PointModel& pointModel1, const PointModel& pointModel2) {
  return pointModel1.name == pointModel2.name &&
         fabs(pointModel1.x - pointModel2.x) < MathHelper::COMPARISON_EPSILON &&
         fabs(pointModel1.y - pointModel2.y) < MathHelper::COMPARISON_EPSILON;
}

bool operator!=(const PointModel& pointModel1, const PointModel& pointModel2) {
  return !(pointModel1 == pointModel2);
}

bool operator<(const PointModel& pointModel1, const PointModel& pointModel2) {
  bool xCoordsEqual = fabs(pointModel1.x - pointModel2.x) < MathHelper::COMPARISON_EPSILON;
  bool yCoordsEqual = fabs(pointModel1.y - pointModel2.y) < MathHelper::COMPARISON_EPSILON;

  if (xCoordsEqual && yCoordsEqual) {
    return pointModel1.name < pointModel2.name;
  }

  if (xCoordsEqual) {
    return pointModel1.y < pointModel2.y;
  }
  
  return pointModel1.x < pointModel2.x;
}

bool operator>(const PointModel& pointModel1, const PointModel& pointModel2) {
  return pointModel2 < pointModel1;
}

bool operator<=(const PointModel& pointModel1, const PointModel& pointModel2) {
  return !(pointModel1 > pointModel2);
}

bool operator>=(const PointModel& pointModel1, const PointModel& pointModel2) {
  return !(pointModel1 < pointModel2);
}

std::ostream& operator<<(std::ostream& stream, const PointModel& pointModel) {
  stream << pointModel.name << "(" << pointModel.x << ", " << pointModel.y << ")";

  return stream;
}

json& operator<<(json& j, const PointModel& pointModel) {
  j = pointModel.getJson();

  return j;
}
}  // namespace expertBackground