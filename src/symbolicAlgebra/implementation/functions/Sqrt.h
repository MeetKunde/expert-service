#ifndef ATOM_SQRT

#ifdef FORWARD_DECLARATION
#ifndef SQRT_FORWARD
#define SQRT_FORWARD

namespace symbolicAlgebra::implementation {
class Sqrt;
}

#endif  // SQRT_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_SQRT
#ifndef SQRT_DECLARE
#define SQRT_DECLARE

namespace symbolicAlgebra::implementation {
class Sqrt : public Power {
 public:
  explicit Sqrt(std::unique_ptr<Atom> arg);
};
}  // namespace symbolicAlgebra::implementation

#endif  // SQRT_DECLARE
#endif  // DECLARATION

#endif  // ATOM_SQRT