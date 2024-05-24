#include "Equation.h"

namespace symbolicAlgebra {
Equation::Equation(const std::string& lhs, const std::string& rhs) : lhs(Expression(lhs)), rhs(Expression(rhs)) {}

Equation::Equation(const Expression& lhs, const Expression& rhs) : lhs(lhs), rhs(rhs) {}

void Equation::substituteLhs(const std::string& variableName, const Expression& subExpression) {
  lhs.substitute(variableName, subExpression);
}

void Equation::substituteRhs(const std::string& variableName, const Expression& subExpression) {
  rhs.substitute(variableName, subExpression);
}

std::ostream& operator<<(std::ostream& stream, const Equation& equation) {
  stream << equation.lhs << " = " << equation.rhs;

  return stream;
}
}  // namespace symbolicAlgebra