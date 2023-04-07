#ifndef FRACTION_NUM_H
#define FRACTION_NUM_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Fraction : public Expression {
 public:
  explicit Fraction(int nominator, int denominator);
};
}  // namespace symbolicAlgebra

#endif /* FRACTION_NUM_H */