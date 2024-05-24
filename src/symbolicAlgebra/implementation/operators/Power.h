#ifndef ATOM_POWER

#ifdef FORWARD_DECLARATION
#ifndef POWER_FORWARD
#define POWER_FORWARD

namespace symbolicAlgebra::implementation {
class Power;
}

#endif  // POWER_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_POWER
#ifndef POWER_DECLARE
#define POWER_DECLARE

namespace symbolicAlgebra::implementation {
class Power : public Atom {
 public:
  Power() = delete;
  explicit Power(std::unique_ptr<Atom> base, std::unique_ptr<Atom> exponent);

  Power(const Power& power) = default;
  Power(Power&& power) = default;

  Power& operator=(const Power& power) = default;
  Power& operator=(Power&& power) = default;

  ~Power() override = default;

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

#endif  // POWER_DECLARE
#endif  // DECLARATION

#endif  // ATOM_POWER
