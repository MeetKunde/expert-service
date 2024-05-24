#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "implementation/Utilities.h"
#include "ExpressionBuilder.h"

namespace symbolicAlgebra {
class Expression {
 public:
  Expression();
  explicit Expression(std::unique_ptr<implementation::Atom> root);
  explicit Expression(const std::string& expression);

  Expression(const Expression& other);
  Expression(Expression&& other);

  Expression& operator=(const Expression& other);
  Expression& operator=(Expression&& other);

  ~Expression();

  std::string getString() const;

  bool canBeFullEvaluated() const;
  double evaluate() const;

  void simplify();
  void expand();

  void substitute(const std::string& variableName, const Expression& subExpression);
  Expression getCoefficient(const Expression& subExpression);

  void getIncludedVariables(std::set<std::string>& names) const;

  void differentiate(const std::string& variable);
  void differentiate(const std::string& variable, unsigned int times);

  Expression& operator+=(const Expression& rhs);
  friend Expression operator+(Expression lhs, const Expression& rhs);

  Expression& operator-=(const Expression& rhs);
  friend Expression operator-(Expression lhs, const Expression& rhs);

  Expression& operator*=(const Expression& rhs);
  friend Expression operator*(Expression lhs, const Expression& rhs);

  Expression& operator/=(const Expression& rhs);
  friend Expression operator/(Expression lhs, const Expression& rhs);

  Expression& operator^=(const Expression& rhs);
  friend Expression operator^(Expression lhs, const Expression& rhs);

  friend bool operator==(const Expression& lhs, const Expression& rhs);
  friend bool operator!=(const Expression& lhs, const Expression& rhs);

  friend std::ostream& operator<<(std::ostream& stream, const Expression& expression);

private:
  std::unique_ptr<implementation::Atom> root;
};
}  // namespace symbolicAlgebra

#endif /* EXPRESSION_H */