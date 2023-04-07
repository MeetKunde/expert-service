#ifndef RATIONAL_NUM_H
#define RATIONAL_NUM_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Rational : public Expression {
 public:
  explicit Rational(double value);
};
}  // namespace symbolicAlgebra

#endif /* RATIONAL_NUM_H */