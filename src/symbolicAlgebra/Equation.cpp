#include "Equation.h"

namespace symbolicAlgebra {
Equation::Equation() = default;

Equation::Equation(const std::string& lhs, const std::string& rhs) : lhs(Expression(lhs)), rhs(Expression(rhs)) {}

Equation::Equation(const Expression& lhs, const Expression& rhs) : lhs(lhs), rhs(rhs) {}

std::ostream& operator<<(std::ostream& stream, const Equation& equation) {
  stream << equation.lhs << " = " << equation.rhs;

  return stream;
}
}  // namespace symbolicAlgebra