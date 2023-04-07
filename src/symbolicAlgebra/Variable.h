#ifndef VARIABLE_H
#define VARIABLE_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Variable : public Expression {
 public:
  explicit Variable(std::string name);
};
}  // namespace symbolicAlgebra

#endif /* VARIABLE_H */