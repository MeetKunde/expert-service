#ifndef ATOM_COT

#ifdef FORWARD_DECLARATION
#ifndef COT_FORWARD
#define COT_FORWARD

namespace symbolicAlgebra::implementation {
class Cot;
}

#endif  // COT_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_COT
#ifndef COT_DECLARE
#define COT_DECLARE

namespace symbolicAlgebra::implementation {
class Cot : public Product {
 public:
  explicit Cot(std::unique_ptr<Atom> arg);
};
}  // namespace symbolicAlgebra::implementation

#endif  // COT_DECLARE
#endif  // DECLARATION

#endif  // ATOM_COT