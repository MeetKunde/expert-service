#ifndef VARIABLE_H
#define VARIABLE_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Variable : public Expression {
 public:
  Variable() = delete;
  explicit Variable(std::string name);

  Variable(const Variable& other) = default;
  Variable(Variable&& other) = default;

  Variable& operator=(const Variable& other) = default;
  Variable& operator=(Variable&& other) = default;

  ~Variable() = default;
};
}  // namespace symbolicAlgebra

#endif /* VARIABLE_H */