#ifndef ATOM_SIN

#ifdef FORWARD_DECLARATION
#ifndef SIN_FORWARD
#define SIN_FORWARD

namespace symbolicAlgebra::implementation {
class Sin;
}

#endif  // SIN_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_SIN
#ifndef SIN_DECLARE
#define SIN_DECLARE

namespace symbolicAlgebra::implementation {
class Sin : public Atom {
 public:
  Sin() = delete;
  explicit Sin(std::unique_ptr<Atom> arg);

  Sin(const Sin& sin) = default;
  Sin(Sin&& sin) = default;

  Sin& operator=(const Sin& sin) = default;
  Sin& operator=(Sin&& sin) = default;

  ~Sin() override = default;

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

#endif  // SIN_DECLARE
#endif  // DECLARATION

#endif  // ATOM_SIN