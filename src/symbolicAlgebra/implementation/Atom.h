#ifndef ATOM_H
#define ATOM_H

#include <algorithm>
#include <iostream>
#include <math.h>
#include <memory>
#include <numeric>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace symbolicAlgebra {
class Expression;
}
namespace symbolicAlgebra::implementation {
class Utilities;
}

namespace symbolicAlgebra::implementation {
class Atom {
 public:
  enum class AtomType { ANY, VARIABLE, NUMBER_INT, NUMBER_RAT, NUMBER_FRAC, CONSTANT, SUM, PRODUCT, POWER, SIN, COS, LOG };

  Atom();
  explicit Atom(AtomType type);
  explicit Atom(AtomType type, std::unique_ptr<Atom> arg);
  explicit Atom(AtomType type, std::unique_ptr<Atom> arg1, std::unique_ptr<Atom> arg2);
  explicit Atom(AtomType type, std::vector<std::unique_ptr<Atom>> args);
  explicit Atom(const Atom& atom);
  Atom(Atom&& atom);

  Atom& operator=(const Atom& atom);
  Atom& operator=(Atom&& atom);
  
  virtual ~Atom() = default;

  virtual std::unique_ptr<Atom> copy() const = 0;
  virtual void print(std::ostream& stream) const = 0;
  virtual bool canBeEvaluated() const = 0;
  virtual double evaluate() const = 0;
  virtual std::unique_ptr<Atom> substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const = 0;
  virtual void getVariablesNames(std::set<std::string>& names) const = 0;
  virtual std::unique_ptr<Atom> differentiate(const std::string& variable) const = 0;
  virtual std::unique_ptr<Atom> simplify() const = 0;
  virtual std::unique_ptr<Atom> expand() const = 0;
  virtual bool compare(const std::unique_ptr<Atom>& other) const = 0;
  virtual std::unique_ptr<Atom> coefficient(const std::unique_ptr<Atom>& node) const = 0;

  inline AtomType getType() const { return type; }

  inline const std::vector<std::unique_ptr<Atom>>& getArgs() const { return args; }

  inline std::vector<std::unique_ptr<Atom>>& getEditableArgs() { return args; }

  static inline bool isNumber(const std::unique_ptr<Atom>& atom) {
    return (atom->getType() == AtomType::NUMBER_INT || atom->getType() == AtomType::NUMBER_RAT || atom->getType() == AtomType::NUMBER_FRAC);
  }
  static inline bool isOperator(const std::unique_ptr<Atom>& atom) {
    return (atom->getType() == AtomType::SUM || atom->getType() == AtomType::PRODUCT || atom->getType() == AtomType::POWER);
  }
  static inline bool isOperand(const std::unique_ptr<Atom>& atom) {
    return (atom->getType() == AtomType::NUMBER_INT || atom->getType() == AtomType::NUMBER_RAT || atom->getType() == AtomType::NUMBER_FRAC ||
            atom->getType() == AtomType::CONSTANT || atom->getType() == AtomType::VARIABLE);
  }
  static inline bool isFunction(const std::unique_ptr<Atom>& atom) {
    return (atom->getType() == AtomType::SIN || atom->getType() == AtomType::COS || atom->getType() == AtomType::LOG);
  }

  friend std::ostream& operator<<(std::ostream& stream, const Atom& atom);

 protected:
  AtomType type;
  std::vector<std::unique_ptr<Atom>> args;
};
}  // namespace symbolicAlgebra::implementation

#endif /* ATOM_H */