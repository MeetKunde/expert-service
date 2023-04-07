#ifndef CONSTANT_H
#define CONSTANT_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Constant : public Expression {
 public:
  explicit Constant(std::string name);
};
}  // namespace symbolicAlgebra

#endif /* CONSTANT_H */