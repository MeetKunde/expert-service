#include "Const.h"
#include "../Utilities.h"

#ifdef DEFINITION
#ifndef CONSTANT_DEFINE
#define CONSTANT_DEFINE
#define ATOM_CONSTANT

namespace symbolicAlgebra::implementation {
Const::Const(std::string name) : Atom(Atom::AtomType::CONSTANT), name(std::move(name)) {}

std::unique_ptr<Atom> Const::copy() const {
  return std::make_unique<Const>(name);
}

void Const::print(std::ostream& stream) const {
  stream << name;
}

inline bool Const::canBeEvaluated() const {
  return false;
}

double Const::evaluate() const {
  return 0.0;
}

std::unique_ptr<Atom> Const::substitute(const std::string& /*variableName*/, const std::unique_ptr<Atom>& /*node*/) const {
  return copy();
}

void Const::getVariablesNames(std::set<std::string>& /*names*/) const {}

std::unique_ptr<Atom> Const::differentiate(const std::string& /*variable*/) const {
  return std::make_unique<Number>(0);
}

std::unique_ptr<Atom> Const::simplify() const {
  return copy();
}

std::unique_ptr<Atom> Const::expand() const {
  return copy();
}

bool Const::compare(const std::unique_ptr<Atom>& other) const {
  if (other->getType() == AtomType::CONSTANT) {
    Const* castedOther = static_cast<Const*>(other.get());

    return name == castedOther->name;
  }

  return false;
}

std::unique_ptr<Atom> Const::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}
}  // namespace symbolicAlgebra::implementation

#endif  // CONSTANT_DEFINE
#endif  // DEFINITION