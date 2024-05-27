#ifndef EXPERT_SERVICE_EXPRESSION_MODEL_H
#define EXPERT_SERVICE_EXPRESSION_MODEL_H

#include <symbolicAlgebra.h>

#include <nlohmann/json.hpp>
#include <string>

typedef nlohmann::json json;

namespace expertBackground {
typedef symbolicAlgebra::Variable Var;
typedef symbolicAlgebra::Constant Const;
typedef symbolicAlgebra::Integer Int;
typedef symbolicAlgebra::Rational Rat;
typedef symbolicAlgebra::Equation Equ;

class ExpressionModel : public symbolicAlgebra::Expression {
 public:
  ExpressionModel() = delete;
  explicit ExpressionModel(const std::string& expressionString);
  explicit ExpressionModel(const symbolicAlgebra::Expression& expression);
  
  ExpressionModel(const ExpressionModel& expressionModel) = default;
  ExpressionModel(ExpressionModel&& expressionModel) = default;

  ExpressionModel& operator=(const ExpressionModel& expressionModel) = default;
  ExpressionModel& operator=(ExpressionModel&& expressionModel) = default;
  
  ~ExpressionModel() = default;

  json getJson() const;

  friend bool operator==(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2);
  friend bool operator!=(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2);
  friend bool operator<(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2);
  friend bool operator>(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2);
  friend bool operator<=(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2);
  friend bool operator>=(const ExpressionModel& expressionModel1, const ExpressionModel& expressionModel2);

  friend std::ostream& operator<<(std::ostream& stream, const ExpressionModel& expressionModel);
  friend json& operator<<(json& j, const ExpressionModel& expressionModel);
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_EXPRESSION_MODEL_H
