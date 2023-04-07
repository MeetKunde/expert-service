#include "../Utilities.h"

#ifdef DEFINITION
#ifndef SQRT_DEFINE
#define SQRT_DEFINE
#define ATOM_SQRT

namespace symbolicAlgebra::implementation {
Sqrt::Sqrt(std::unique_ptr<Atom> arg)
    : Power(std::move(arg),
            std::make_unique<Product>(std::make_unique<Number>(1),
                                      std::make_unique<Power>(std::make_unique<Number>(2), std::make_unique<Number>(-1)))) {}

}  // namespace symbolicAlgebra::implementation

#endif  // SQRT_DEFINE
#endif  // DEFINITION