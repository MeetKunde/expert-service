#ifndef EXPERT_SERVICE_EXPRESSION_MODEL_H
#define EXPERT_SERVICE_EXPRESSION_MODEL_H

#include <symbolicAlgebra.h>
#include "IModel.h"

namespace expertBackground {
typedef symbolicAlgebra::Variable Var;
typedef symbolicAlgebra::Constant Const;
typedef symbolicAlgebra::Integer Int;
typedef symbolicAlgebra::Rational Rat;
typedef symbolicAlgebra::Equation Equ;

class ExpressionModel : public IModel, public symbolicAlgebra::Expression {
 public:
  ExpressionModel();
  explicit ExpressionModel(const std::string& expressionString);
  explicit ExpressionModel(const symbolicAlgebra::Expression& expression);
  ExpressionModel(const ExpressionModel& other);
  ExpressionModel& operator=(const ExpressionModel& other);

  json getJsonObject() const override;
  ~ExpressionModel() override;
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_EXPRESSION_MODEL_H
