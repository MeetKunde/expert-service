#include "Fraction.h"

namespace symbolicAlgebra {
Fraction::Fraction(int nominator, int denominator)
    : Expression(std::make_unique<implementation::Number>(nominator, denominator)) {}
}  // namespace symbolicAlgebra
