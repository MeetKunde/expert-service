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
  Tan() = delete;
  explicit Tan(std::unique_ptr<Atom> arg);

  Tan(const Tan& tan) = default;
  Tan(Tan&& tan) = default;

  Tan& operator=(const Tan& tan) = default;
  Tan& operator=(Tan&& tan) = default;

  ~Tan() override = default;
};
}  // namespace symbolicAlgebra::implementation

#endif  // TAN_DECLARE
#endif  // DECLARATION

#endif  // ATOM_TAN