#ifndef ATOM_H
#define ATOM_H

#include <bits/stdc++.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <ostream>
#include <set>
#include <sstream>
#include <string>

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

  std::vector<std::unique_ptr<Atom>> args;
  AtomType type;

  Atom();
  explicit Atom(AtomType type);
  Atom(AtomType type, std::unique_ptr<Atom> arg);
  Atom(AtomType type, std::unique_ptr<Atom> arg1, std::unique_ptr<Atom> arg2);
  Atom(AtomType type, std::vector<std::unique_ptr<Atom>> args);
  Atom(const Atom& atom);
  Atom& operator=(const Atom& atom);
  virtual ~Atom();

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

  static inline bool isNumber(const std::unique_ptr<Atom>& atom) {
    return (atom->type == AtomType::NUMBER_INT || atom->type == AtomType::NUMBER_RAT || atom->type == AtomType::NUMBER_FRAC);
  }
  static inline bool isOperator(const std::unique_ptr<Atom>& atom) {
    return (atom->type == AtomType::SUM || atom->type == AtomType::PRODUCT || atom->type == AtomType::POWER);
  }
  static inline bool isOperand(const std::unique_ptr<Atom>& atom) {
    return (atom->type == AtomType::NUMBER_INT || atom->type == AtomType::NUMBER_RAT || atom->type == AtomType::NUMBER_FRAC ||
            atom->type == AtomType::CONSTANT || atom->type == AtomType::VARIABLE);
  }
  static inline bool isFunction(const std::unique_ptr<Atom>& atom) {
    return (atom->type == AtomType::SIN || atom->type == AtomType::COS || atom->type == AtomType::LOG);
  }

  friend std::ostream& operator<<(std::ostream& stream, const Atom& atom);
};
}  // namespace symbolicAlgebra::implementation

#endif /* ATOM_H */