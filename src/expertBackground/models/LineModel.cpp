#include "LineModel.h"

namespace expertBackground {
LineModel::LineModel(std::string identifier, LineType type, float lineA, float lineB, std::vector<std::string> includedPoints)
    : id{std::move(identifier)}, lineType{type}, lineA{lineA}, lineB{lineB}, includedPointIds{std::move(includedPoints)} {}

LineModel::LineModel(const LineModel& lineModel)
    : id{lineModel.id},
      lineType{lineModel.lineType},
      lineA{lineModel.lineA},
      lineB{lineModel.lineB},
      includedPointIds{lineModel.includedPointIds} {}

LineModel& LineModel::operator=(const LineModel& lineModel) {
  id = lineModel.id;
  lineType = lineModel.lineType;
  lineA = lineModel.lineA;
  lineB = lineModel.lineB;
  includedPointIds = lineModel.includedPointIds;

  return *this;
}

json LineModel::getJsonObject() const {
  return {{"id", id},
          {"type", lineType},
          {"a", lineA},
          {"b", lineB},
          {"end1Id", includedPointIds.front()},
          {"end2Id", includedPointIds.back()},
          {"pointsOn", json(includedPointIds)}};
}

bool operator==(const LineModel& lineModel1, const LineModel& lineModel2) {
  return lineModel1.lineType == lineModel2.lineType &&
         fabs(lineModel1.lineA - lineModel2.lineA) < MathHelper::COMPARISON_EPSILON &&
         fabs(lineModel1.lineB - lineModel2.lineB) < MathHelper::COMPARISON_EPSILON;
}

bool operator!=(const LineModel& lineModel1, const LineModel& lineModel2) {
  return !(lineModel1 == lineModel2);
}

bool operator<(const LineModel& lineModel1, const LineModel& lineModel2) {
  if (fabs(lineModel1.lineA - lineModel2.lineA) < MathHelper::COMPARISON_EPSILON &&
      fabs(lineModel1.lineB - lineModel2.lineB) < MathHelper::COMPARISON_EPSILON) {
    return lineModel1.lineType < lineModel2.lineType;
  }

  if (fabs(lineModel1.lineA - lineModel2.lineA) < MathHelper::COMPARISON_EPSILON) {
    return lineModel1.lineB < lineModel2.lineB;
  }

  return lineModel1.lineA < lineModel2.lineA;
}

bool operator>(const LineModel& lineModel1, const LineModel& lineModel2) {
  return lineModel2 < lineModel1;
}

bool operator<=(const LineModel& lineModel1, const LineModel& lineModel2) {
  return lineModel1 < lineModel2 || lineModel1 == lineModel2;
}

bool operator>=(const LineModel& lineModel1, const LineModel& lineModel2) {
  return lineModel1 > lineModel2 || lineModel1 == lineModel2;
}

std::ostream& operator<<(std::ostream& stream, const LineModel& lineModel) {
  if (lineModel.lineType == LineModel::LineType::VERTICAL) {
    stream << "x = " << lineModel.lineB;
  }
  else if (lineModel.lineType == LineModel::LineType::HORIZONTAL) {
    stream << "y = " << lineModel.lineB;
  }
  else {
    stream << "y = " << lineModel.lineA << "x + " << lineModel.lineB;
  }

  return stream;
}
LineModel::~LineModel() {}
}  // namespace expertBackground