#include "Variable.h"

namespace symbolicAlgebra {
Variable::Variable(std::string name) : Expression(std::make_unique<implementation::Var>(std::move(name))) {}
}  // namespace symbolicAlgebra
