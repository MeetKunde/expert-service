#include "ExpressionModel.h"

namespace expertBackground {
ExpressionModel::ExpressionModel() : IModel() {}

ExpressionModel::ExpressionModel(const std::string& expressionString) : IModel(), Expression(expressionString) {}

ExpressionModel::ExpressionModel(const symbolicAlgebra::Expression& expression) : IModel(), Expression(expression) {}

ExpressionModel::ExpressionModel(const ExpressionModel& other) : IModel(), Expression(other) {}

ExpressionModel& ExpressionModel::operator=(const ExpressionModel& other) {
  Expression::operator=(other);

  return *this;
}

json ExpressionModel::getJsonObject() const {
  std::set<std::string> vars = {};
  getIncludedVariables(vars);

  return {{"value", getString()}, {"variables", json(vars)}};
}
ExpressionModel::~ExpressionModel() {}
}  // namespace expertBackground