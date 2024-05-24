#include "Var.h"
#include "../Utilities.h"

#ifdef DEFINITION
#ifndef VARIABLE_DEFINE
#define VARIABLE_DEFINE
#define ATOM_VARIABLE

namespace symbolicAlgebra::implementation {
Var::Var(std::string name) : Atom(AtomType::VARIABLE), name(std::move(name)) {}

std::unique_ptr<Atom> Var::copy() const {
  return std::make_unique<Var>(name);
}

void Var::print(std::ostream& stream) const {
  stream << name;
}

inline bool Var::canBeEvaluated() const {
  return false;
}

double Var::evaluate() const {
  return 0.0;
}

std::unique_ptr<Atom> Var::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  if (name == variableName) {
    return node->copy();
  }

  return copy();
}

void Var::getVariablesNames(std::set<std::string>& names) const {
  names.insert(name);
}

std::unique_ptr<Atom> Var::differentiate(const std::string& variable) const {
  if (name == variable) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}

std::unique_ptr<Atom> Var::simplify() const {
  return copy();
}

std::unique_ptr<Atom> Var::expand() const {
  return copy();
}

bool Var::compare(const std::unique_ptr<Atom>& other) const {
  if (other->getType() == AtomType::VARIABLE) {
    Var* castedOther = static_cast<Var*>(other.get());

    return name == castedOther->name;
  }

  return false;
}

std::unique_ptr<Atom> Var::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}
}  // namespace symbolicAlgebra::implementation

#endif  // VARIABLE_DEFINE
#endif  // DEFINITION