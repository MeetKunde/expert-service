#ifndef ATOM_CONSTANT

#ifdef FORWARD_DECLARATION
#ifndef CONSTANT_FORWARD
#define CONSTANT_FORWARD

namespace symbolicAlgebra::implementation {
class Const;
}

#endif  // CONSTANT_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_CONSTANT
#ifndef CONSTANT_DECLARE
#define CONSTANT_DECLARE

namespace symbolicAlgebra::implementation {
class Const : public Atom {
 public:
  Const() = delete;
  explicit Const(std::string name);

  Const(const Const& constant) = default;
  Const(Const&& constant) = default;

  Const& operator=(const Const& constant) = default;
  Const& operator=(Const&& constant) = default;

  ~Const() override = default;

  inline std::string getName() const { return name; }

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

 private:
  std::string name;
};
}  // namespace symbolicAlgebra::implementation

#endif  // CONSTANT_DECLARE
#endif  // DECLARATION

#endif  // ATOM_CONSTANT