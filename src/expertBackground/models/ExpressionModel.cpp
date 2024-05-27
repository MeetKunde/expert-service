#include "ExpressionModel.h"

namespace expertBackground {
ExpressionModel::ExpressionModel(const std::string& expressionString) : Expression{expressionString} {}

ExpressionModel::ExpressionModel(const symbolicAlgebra::Expression& expression) : Expression{expression} {}

json ExpressionModel::getJson() const {
  std::set<std::string> vars = {};
  getIncludedVariables(vars);

  return {{"value", getString()}, 
          {"variables", json(vars)}};
}

bool operator==(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2) {
  return expressionModel1.getString() == expressionModel2.getString();
}

bool operator!=(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2) {
  return !(expressionModel1 == expressionModel2);
}

bool operator<(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2) {
  return expressionModel1.getString() < expressionModel2.getString();
}

bool operator>(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2) {
  return expressionModel2 < expressionModel1;
}

bool operator<=(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2) {
  return !(expressionModel1 > expressionModel2);
}

bool operator>=(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2) {
  return !(expressionModel1 < expressionModel2);
}

std::ostream& operator<<(std::ostream& stream, const ExpressionModel& expressionModel) {
  stream << expressionModel.getString();

  return stream;
}

json& operator<<(json& j, const ExpressionModel& expressionModel) {
  j = expressionModel.getJson();

  return j;
}
}  // namespace expertBackground