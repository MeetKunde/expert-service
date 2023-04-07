#ifndef ATOM_TAN

#ifdef FORWARD_DECLARATION
#ifndef TAN_FORWARD
#define TAN_FORWARD

namespace symbolicAlgebra::implementation {
class Tan;
}

#endif  // TAN_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_TAN
#ifndef TAN_DECLARE
#define TAN_DECLARE

namespace symbolicAlgebra::implementation {
class Tan : public Product {
 public:
  explicit Tan(std::unique_ptr<Atom> arg);
};
}  // namespace symbolicAlgebra::implementation

#endif  // TAN_DECLARE
#endif  // DECLARATION

#endif  // ATOM_TAN