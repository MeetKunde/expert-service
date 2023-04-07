#ifndef ATOM_NUMBER

#ifdef FORWARD_DECLARATION
#ifndef NUMBER_FORWARD
#define NUMBER_FORWARD

namespace symbolicAlgebra::implementation {
class Number;
}

#endif  // NUMBER_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_NUMBER
#ifndef NUMBER_DECLARE
#define NUMBER_DECLARE

namespace symbolicAlgebra::implementation {
class Number : public Atom {
 public:
  int nominator;
  int denominator;
  double rational;

 public:
  explicit Number();
  explicit Number(int value);
  explicit Number(double value);
  explicit Number(int nominatorValue, int denominatorValue);

  std::unique_ptr<Atom> copy() const override;
  void print(std::ostream& stream) const override;

  bool canBeEvaluated() const override;

  double evaluate() const override;

  std::unique_ptr<Atom> substitute(const std::string& variableName, const std::unique_ptr<Atom>& val) const override;

  void getVariablesNames(std::set<std::string>& names) const override;

  std::unique_ptr<Atom> differentiate(const std::string& variable) const override;

  std::unique_ptr<Atom> simplify() const override;

  std::unique_ptr<Atom> expand() const override;

  bool compare(const std::unique_ptr<Atom>& other) const override;

  std::unique_ptr<Atom> coefficient(const std::unique_ptr<Atom>& node) const override;

  inline int getNominator() const { return nominator; }
  inline int getDenominator() const { return denominator; }
  inline double getRational() const { return rational; }

  Number& operator+=(const Number& rhs);
  friend Number operator+(Number lhs, const Number& rhs);

  Number& operator-=(const Number& rhs);
  friend Number operator-(Number lhs, const Number& rhs);

  Number& operator*=(const Number& rhs);
  friend Number operator*(Number lhs, const Number& rhs);

  Number& operator/=(const Number& rhs);
  friend Number operator/(Number lhs, const Number& rhs);

  Number& operator^=(const Number& rhs);
  friend Number operator^(Number lhs, const Number& rhs);

  friend bool operator==(const Number& lhs, const Number& rhs);
  friend bool operator!=(const Number& lhs, const Number& rhs);
};
}  // namespace symbolicAlgebra::implementation

#endif  // NUMBER_DECLARE
#endif  // DECLARATION

#endif  // ATOM_NUMBER