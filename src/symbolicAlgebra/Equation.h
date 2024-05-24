#ifndef EQUATION_H
#define EQUATION_H

#include "Expression.h"

namespace symbolicAlgebra {
class Equation {
 public:
  Equation() = default;
  explicit Equation(const std::string& lhs, const std::string& rhs);
  explicit Equation(const Expression& lhs, const Expression& rhs);

  Equation(const Equation& other) = default;
  Equation(Equation&& other) = default;

  Equation& operator=(const Equation& other) = default;
  Equation& operator=(Equation&& other) = default;

  ~Equation() = default;

  inline const Expression& getLhs() const { return lhs; }
  inline const Expression& getRhs() const { return rhs; }

  void substituteLhs(const std::string& variableName, const Expression& subExpression);
  void substituteRhs(const std::string& variableName, const Expression& subExpression);

  friend std::ostream& operator<<(std::ostream& stream, const Equation& equation);

 protected:
  Expression lhs;
  Expression rhs;
};
}  // namespace symbolicAlgebra

#endif /* EQUATION_H */