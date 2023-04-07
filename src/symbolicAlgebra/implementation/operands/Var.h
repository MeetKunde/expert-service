#ifndef ATOM_VARIABLE

#ifdef FORWARD_DECLARATION
#ifndef VARIABLE_FORWARD
#define VARIABLE_FORWARD

namespace symbolicAlgebra::implementation {
class Var;
}

#endif  // VARIABLE_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_VARIABLE
#ifndef VARIABLE_DECLARE
#define VARIABLE_DECLARE

namespace symbolicAlgebra::implementation {
class Var : public Atom {
 private:
  std::string name;

 public:
  explicit Var(std::string name);

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
};
}  // namespace symbolicAlgebra::implementation

#endif  // VARIABLE_DECLARE
#endif  // DECLARATION

#endif  // ATOM_VARIABLE
