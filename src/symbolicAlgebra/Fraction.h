#ifndef FRACTION_NUM_H
#define FRACTION_NUM_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Fraction : public Expression {
 public:
  Fraction() = delete;
  explicit Fraction(int nominator, int denominator);

  Fraction(const Fraction& other) = default;
  Fraction(Fraction&& other) = default;

  Fraction& operator=(const Fraction& other) = default;
  Fraction& operator=(Fraction&& other) = default;

  ~Fraction() = default;
};
}  // namespace symbolicAlgebra

#endif /* FRACTION_NUM_H */