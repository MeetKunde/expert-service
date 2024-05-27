#include "LineModel.h"

namespace expertBackground {
LineModel::LineModel(std::string identifier, Type type, float coeffA, float coeffB, std::vector<std::string> includedPoints)
    : id{std::move(identifier)}, type{type}, coeffA{coeffA}, coeffB{coeffB}, includedPointIds{std::move(includedPoints)} {}

json LineModel::getJson() const {
  return {{"id", id},
          {"type", static_cast<int>(type)},
          {"a", coeffA},
          {"b", coeffB},
          {"pointsOn", json(includedPointIds)}};
}

bool operator==(const LineModel& lineModel1, const LineModel& lineModel2) {
  return lineModel1.type == lineModel2.type &&
         fabs(lineModel1.coeffA- lineModel2.coeffA) < MathHelper::COMPARISON_EPSILON &&
         fabs(lineModel1.coeffB - lineModel2.coeffB) < MathHelper::COMPARISON_EPSILON;
}

bool operator!=(const LineModel& lineModel1, const LineModel& lineModel2) {
  return !(lineModel1 == lineModel2);
}

bool operator<(const LineModel& lineModel1, const LineModel& lineModel2) {
  if (fabs(lineModel1.coeffA - lineModel2.coeffA) < MathHelper::COMPARISON_EPSILON &&
      fabs(lineModel1.coeffB - lineModel2.coeffB) < MathHelper::COMPARISON_EPSILON) {
    return lineModel1.type < lineModel2.type;
  }

  if (fabs(lineModel1.coeffA - lineModel2.coeffA) < MathHelper::COMPARISON_EPSILON) {
    return lineModel1.coeffB < lineModel2.coeffB;
  }

  return lineModel1.coeffA < lineModel2.coeffA;
}

bool operator>(const LineModel& lineModel1, const LineModel& lineModel2) {
  return lineModel2 < lineModel1;
}

bool operator<=(const LineModel& lineModel1, const LineModel& lineModel2) {
  return !(lineModel1 > lineModel2);
}

bool operator>=(const LineModel& lineModel1, const LineModel& lineModel2) {
  return !(lineModel1 < lineModel2);
}

std::ostream& operator<<(std::ostream& stream, const LineModel& lineModel) {
  if (lineModel.type == LineModel::Type::VERTICAL) {
    stream << "x = " << lineModel.coeffB;
  }
  else if (lineModel.type == LineModel::Type::HORIZONTAL) {
    stream << "y = " << lineModel.coeffB;
  }
  else {
    stream << "y = " << lineModel.coeffA << "x + " << lineModel.coeffB;
  }

  return stream;
}

json& operator<<(json& j, const LineModel& lineModel) {
  j = lineModel.getJson();
  return j;
}
}  // namespace expertBackground