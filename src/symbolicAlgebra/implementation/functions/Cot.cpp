#include "../Utilities.h"

#ifdef DEFINITION
#ifndef COT_DEFINE
#define COT_DEFINE
#define ATOM_COT

namespace symbolicAlgebra::implementation {
Cot::Cot(std::unique_ptr<Atom> arg)
    : Product(std::make_unique<Cos>(arg->copy()),
              std::make_unique<Power>(std::make_unique<Sin>(arg->copy()), std::make_unique<Number>(-1))) {}

}  // namespace symbolicAlgebra::implementation

#endif  // COT_DEFINE
#endif  // DEFINITION