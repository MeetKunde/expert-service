#ifndef RATIONAL_NUM_H
#define RATIONAL_NUM_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Rational : public Expression {
 public:
  Rational() = delete;
  explicit Rational(double value);

  Rational(const Rational& other) = default;
  Rational(Rational&& other) = default;

  Rational& operator=(const Rational& other) = default;
  Rational& operator=(Rational&& other) = default;

  ~Rational() = default;
};
}  // namespace symbolicAlgebra

#endif /* RATIONAL_NUM_H */