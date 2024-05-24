#ifndef ATOM_COS

#ifdef FORWARD_DECLARATION
#ifndef COS_FORWARD
#define COS_FORWARD

namespace symbolicAlgebra::implementation {
class Cos;
}

#endif  // COS_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_COS
#ifndef COS_DECLARE
#define COS_DECLARE

namespace symbolicAlgebra::implementation {
class Cos : public Atom {
 public:
  Cos() = delete;
  explicit Cos(std::unique_ptr<Atom> arg);

  Cos(const Cos& cos) = default;
  Cos(Cos&& cos) = default;

  Cos& operator=(const Cos& cos) = default;
  Cos& operator=(Cos&& cos) = default;

  ~Cos() override = default;

  std::unique_ptr<Atom> copy() const override;
  void print(std::ostream& stream) const override;
  bool canBeEvaluated() const override;
  double evaluate() const override;
  std::unique_ptr<Atom> substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const override;
  void getVariablesNames(std::set<std::string>& names) const override;
  std::unique_ptr<Atom> differentiate(const std::string& variable) const override;
  std::unique_ptr<Atom> simplify() const override;
  std::unique_ptr<Atom> expand() const override;
  bool compare(const std::unique_ptr<Atom>& other) const override;
  std::unique_ptr<Atom> coefficient(const std::unique_ptr<Atom>& node) const override;
};
}  // namespace symbolicAlgebra::implementation

#endif  // COS_DECLARE
#endif  // DECLARATION

#endif  // ATOM_COS