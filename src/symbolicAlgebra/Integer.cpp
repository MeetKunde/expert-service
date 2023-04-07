#include "Integer.h"

namespace symbolicAlgebra {
Integer::Integer(int value) : Expression(std::make_unique<implementation::Number>(value)) {}
}  // namespace symbolicAlgebra
