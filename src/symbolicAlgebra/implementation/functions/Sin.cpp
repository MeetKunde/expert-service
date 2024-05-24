#include "../Utilities.h"

#ifdef DEFINITION
#ifndef SIN_DEFINE
#define SIN_DEFINE
#define ATOM_SIN

namespace symbolicAlgebra::implementation {
Sin::Sin(std::unique_ptr<Atom> arg) : Atom(AtomType::SIN, std::move(arg)) {}

std::unique_ptr<Atom> Sin::copy() const {
  return std::make_unique<Sin>(args.front()->copy());
}

void Sin::print(std::ostream& stream) const {
  stream << "sin(";
  args.front()->print(stream);
  stream << ")";
}

bool Sin::canBeEvaluated() const {
  return args.front()->canBeEvaluated();
}

double Sin::evaluate() const {
  return sin(args.front()->evaluate());
}

std::unique_ptr<Atom> Sin::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  return std::make_unique<Sin>(args.front()->substitute(variableName, node));
}

void Sin::getVariablesNames(std::set<std::string>& names) const {
  args.front()->getVariablesNames(names);
}

std::unique_ptr<Atom> Sin::differentiate(const std::string& variable) const {
  return std::make_unique<Product>(std::make_unique<Cos>(args.front()->copy()), args.front()->differentiate(variable));
}

std::unique_ptr<Atom> Sin::simplify() const {
  std::unique_ptr<Atom> simplifiedArgument = args.front()->simplify();

  if (Atom::isNumber(simplifiedArgument) && simplifiedArgument->evaluate() == 0.0) {
    return std::make_unique<Number>(0);
  }

  if (simplifiedArgument->getType() == AtomType::PRODUCT && !simplifiedArgument->getArgs().empty() &&
      Atom::isNumber(simplifiedArgument->getArgs().front()) && simplifiedArgument->getArgs().front()->evaluate() < 0.0) {
    *simplifiedArgument->getEditableArgs().begin() = std::make_unique<Number>(-simplifiedArgument->getArgs().front()->evaluate());

    return std::make_unique<Product>(std::make_unique<Number>(-1), std::make_unique<Sin>(std::move(simplifiedArgument)));
  }

  return std::make_unique<Sin>(std::move(simplifiedArgument));
}

std::unique_ptr<Atom> Sin::expand() const {
  return std::make_unique<Sin>(args.front()->expand());
}

bool Sin::compare(const std::unique_ptr<Atom>& other) const {
  if (other->getType() == AtomType::SIN) {
    return args.front()->compare(other->getArgs().front());
  }

  return false;
}

std::unique_ptr<Atom> Sin::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}

}  // namespace symbolicAlgebra::implementation

#endif  // SIN_DEFINE
#endif  // DEFINITION