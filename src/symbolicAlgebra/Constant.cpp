#include "Constant.h"

namespace symbolicAlgebra {
Constant::Constant(std::string name) : Expression(std::make_unique<implementation::Const>(std::move(name))) {}
}  // namespace symbolicAlgebra
