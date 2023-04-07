#include "../Utilities.h"

#ifdef DEFINITION
#ifndef COS_DEFINE
#define COS_DEFINE
#define ATOM_COS

namespace symbolicAlgebra::implementation {
Cos::Cos(std::unique_ptr<Atom> arg) : Atom(AtomType::COS, std::move(arg)) {}

std::unique_ptr<Atom> Cos::copy() const {
  return std::make_unique<Cos>(args.front()->copy());
}

void Cos::print(std::ostream& stream) const {
  stream << "cos(";
  args.front()->print(stream);
  stream << ")";
}

bool Cos::canBeEvaluated() const {
  return args.front()->canBeEvaluated();
}

double Cos::evaluate() const {
  return cos(args.front()->evaluate());
}

std::unique_ptr<Atom> Cos::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  return std::make_unique<Cos>(args.front()->substitute(variableName, node));
}

void Cos::getVariablesNames(std::set<std::string>& names) const {
  args.front()->getVariablesNames(names);
}

std::unique_ptr<Atom> Cos::differentiate(const std::string& variable) const {
  return std::make_unique<Product>(
      std::make_unique<Product>(std::make_unique<implementation::Number>(-1), std::make_unique<Sin>(args.front()->copy())),
      args.front()->differentiate(variable));
}

std::unique_ptr<Atom> Cos::simplify() const {
  std::unique_ptr<Atom> simplifiedArgument = args.front()->simplify();

  if (Atom::isNumber(simplifiedArgument) && simplifiedArgument->evaluate() == 0.0) {
    return std::make_unique<Number>(1);
  }

  if (simplifiedArgument->type == AtomType::PRODUCT && !simplifiedArgument->args.empty() &&
      Atom::isNumber(simplifiedArgument->args.front()) && simplifiedArgument->args.front()->evaluate() < 0.0) {
    *simplifiedArgument->args.begin() = std::make_unique<Number>(-simplifiedArgument->args.front()->evaluate());

    return std::make_unique<Cos>(std::move(simplifiedArgument));
  }

  return std::make_unique<Cos>(std::move(simplifiedArgument));
}

std::unique_ptr<Atom> Cos::expand() const {
  return std::make_unique<Cos>(args.front()->expand());
}

bool Cos::compare(const std::unique_ptr<Atom>& other) const {
  if (other->type == AtomType::COS) {
    return args.front()->compare(other->args.front());
  }

  return false;
}

std::unique_ptr<Atom> Cos::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}

}  // namespace symbolicAlgebra::implementation

#endif  // COS_DEFINE
#endif  // DEFINITION