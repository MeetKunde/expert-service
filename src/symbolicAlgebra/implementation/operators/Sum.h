#ifndef ATOM_SUM

#ifdef FORWARD_DECLARATION
#ifndef SUM_FORWARD
#define SUM_FORWARD

namespace symbolicAlgebra::implementation {
class Sum;
}

#endif  // SUM_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_SUM
#ifndef SUM_DECLARE
#define SUM_DECLARE

namespace symbolicAlgebra::implementation {
class Sum : public Atom {
 public:
  explicit Sum();
  explicit Sum(std::unique_ptr<Atom> addend);
  explicit Sum(std::unique_ptr<Atom> addend1, std::unique_ptr<Atom> addend2);
  explicit Sum(std::vector<std::unique_ptr<Atom>> summands);

  std::unique_ptr<Atom> copy() const override;
  void print(std::ostream& stream) const override;
  bool canBeEvaluated() const override;
  double evaluate() const override;
  std::unique_ptr<Atom> substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const override;
  void getVariablesNames(std::set<std::string>& names) const override;
  std::unique_ptr<Atom> differentiate(const std::string& variable) const override;
  std::unique_ptr<Atom> simplify() const override;
  std::unique_ptr<Atom> simplify2() const;
  std::unique_ptr<Atom> expand() const override;
  bool compare(const std::unique_ptr<Atom>& other) const override;
  std::unique_ptr<Atom> coefficient(const std::unique_ptr<Atom>& node) const override;
};
}  // namespace symbolicAlgebra::implementation

#endif  // SUM_DECLARE
#endif  // DECLARATION

#endif  // ATOM_SUM