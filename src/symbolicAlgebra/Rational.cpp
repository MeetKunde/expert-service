#include "Rational.h"

namespace symbolicAlgebra {
Rational::Rational(double value) : Expression(std::make_unique<implementation::Number>(value)) {}
}  // namespace symbolicAlgebra
