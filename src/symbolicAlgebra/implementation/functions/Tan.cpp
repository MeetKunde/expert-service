#include "../Utilities.h"

#ifdef DEFINITION
#ifndef TAN_DEFINE
#define TAN_DEFINE
#define ATOM_TAN

namespace symbolicAlgebra::implementation {
Tan::Tan(std::unique_ptr<Atom> arg)
    : Product(std::make_unique<Sin>(arg->copy()),
              std::make_unique<Power>(std::make_unique<Cos>(arg->copy()), std::make_unique<Number>(-1))) {}
}  // namespace symbolicAlgebra::implementation

#endif  // TAN_DEFINE
#endif  // DEFINITION