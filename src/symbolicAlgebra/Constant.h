#ifndef CONSTANT_H
#define CONSTANT_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Constant : public Expression {
 public:
  Constant() = delete;
  explicit Constant(std::string name);

  Constant(const Constant& other) = default;
  Constant(Constant&& other) = default;

  Constant& operator=(const Constant& other) = default;
  Constant& operator=(Constant&& other) = default;

  ~Constant() = default;
};
}  // namespace symbolicAlgebra

#endif /* CONSTANT_H */