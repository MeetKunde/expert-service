#ifndef SOLVE_H
#define SOLVE_H

#include "Equation.h"

namespace symbolicAlgebra {
class Solve {
 public:
  Solve() = delete;
  
  static bool compare(const Expression& lhs, const Expression& rhs);
  static std::vector<Equation> findZeros(const Expression& equation, const std::string& variable);
  static std::vector<Equation> equation(const Equation& equation, const std::string& variable);
  static std::vector<Equation> systemOfEquations(const std::vector<Equation>& equations);

 private:
  static void systemOfEquations(const std::vector<Equation>& equations, std::stack<Equation>& justUsedEquations,
                                std::vector<Equation>& notUsed, std::vector<Equation>& results);
};
}  // namespace symbolicAlgebra

#endif /* SOLVE_H */