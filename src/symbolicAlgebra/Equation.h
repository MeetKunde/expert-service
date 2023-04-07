#ifndef EQUATION_H
#define EQUATION_H

#include "Expression.h"

namespace symbolicAlgebra {
class Equation {
 public:
  Expression lhs;
  Expression rhs;

  Equation();
  Equation(const std::string& lhs, const std::string& rhs);
  Equation(const Expression& lhs, const Expression& rhs);

  friend std::ostream& operator<<(std::ostream& stream, const Equation& equation);
};
}  // namespace symbolicAlgebra

#endif /* EQUATION_H */