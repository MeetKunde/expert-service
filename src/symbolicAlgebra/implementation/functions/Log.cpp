#include "../Utilities.h"

#ifdef DEFINITION
#ifndef LOG_DEFINE
#define LOG_DEFINE
#define ATOM_LOG

namespace symbolicAlgebra::implementation {
Log::Log(std::unique_ptr<Atom> base, std::unique_ptr<Atom> argument)
    : Atom(AtomType::LOG, std::move(base), std::move(argument)) {}

std::unique_ptr<Atom> Log::copy() const {
  return std::make_unique<Log>(args.front()->copy(), args.back()->copy());
}

void Log::print(std::ostream& stream) const {
  stream << "log_(";
  args.front()->print(stream);
  stream << ")(";
  args.back()->print(stream);
  stream << ")";
}

bool Log::canBeEvaluated() const {
  return args.front()->canBeEvaluated() && args.back()->canBeEvaluated();
}

double Log::evaluate() const {
  return log2(args.back()->evaluate()) / log2(args.front()->evaluate());
}

std::unique_ptr<Atom> Log::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  return std::make_unique<Log>(args.front()->substitute(variableName, node), args.back()->substitute(variableName, node));
}

void Log::getVariablesNames(std::set<std::string>& names) const {
  args.front()->getVariablesNames(names);
  args.back()->getVariablesNames(names);
}

std::unique_ptr<Atom> Log::differentiate(const std::string& variable) const {
  std::unique_ptr<Atom> nominator1nominator = std::make_unique<Product>(
      std::make_unique<Log>(std::make_unique<Const>("e"), args.front()->copy()), args.back()->differentiate(variable));
  std::unique_ptr<Atom> nominator2nominator = std::make_unique<Product>(
      std::make_unique<Log>(std::make_unique<Const>("e"), args.back()->copy()), args.front()->differentiate(variable));
  std::unique_ptr<Atom> nominator1 = std::make_unique<Product>(
      std::move(nominator1nominator), std::make_unique<Power>(args.back()->copy(), std::make_unique<Number>(-1)));
  std::unique_ptr<Atom> nominator2 = std::make_unique<Product>(
      std::move(nominator2nominator), std::make_unique<Power>(args.front()->copy(), std::make_unique<Number>(-1)));
  std::unique_ptr<Atom> nominator = std::make_unique<Sum>(
      std::move(nominator1), std::make_unique<Product>(std::make_unique<Number>(-1), std::move(nominator2)));
  std::unique_ptr<Atom> denominator = std::make_unique<Power>(
      std::make_unique<Log>(std::make_unique<Const>("e"), args.front()->copy()), std::make_unique<Number>(2));

  return std::make_unique<Product>(std::move(nominator),
                                   std::make_unique<Power>(std::move(denominator), std::make_unique<Number>(-1)));
}

std::unique_ptr<Atom> Log::simplify() const {
  std::unique_ptr<Atom> simplifiedArgument1 = args.front()->simplify();
  std::unique_ptr<Atom> simplifiedArgument2 = args.back()->simplify();

  if (Atom::isNumber(simplifiedArgument2) && simplifiedArgument2->evaluate() == 1.0) {
    return std::make_unique<Number>(0);
  }

  if (simplifiedArgument1->compare(simplifiedArgument2)) {
    return std::make_unique<Number>(1);
  }

  if (simplifiedArgument2->type == AtomType::POWER && simplifiedArgument2->args.front()->compare(simplifiedArgument1)) {
    return simplifiedArgument2->args.back()->copy();
  }

  return std::make_unique<Log>(std::move(simplifiedArgument1), std::move(simplifiedArgument2));
}

std::unique_ptr<Atom> Log::expand() const {
  return std::make_unique<Log>(args.front()->expand(), args.back()->expand());
}

bool Log::compare(const std::unique_ptr<Atom>& other) const {
  if (other->type == AtomType::LOG) {
    return args.front()->compare(other->args.front()) && args.back()->compare(other->args.back());
  }

  return false;
}

std::unique_ptr<Atom> Log::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}

}  // namespace symbolicAlgebra::implementation

#endif  // LOG_DEFINE
#endif  // DEFINITION