#include "../Utilities.h"

#ifdef DEFINITION
#ifndef POWER_DEFINE
#define POWER_DEFINE
#define ATOM_POWER

namespace symbolicAlgebra::implementation {
Power::Power(std::unique_ptr<Atom> base, std::unique_ptr<Atom> exponent)
    : Atom(AtomType::POWER, std::move(base), std::move(exponent)) {}

std::unique_ptr<Atom> Power::copy() const {
  return std::make_unique<Power>(args.front()->copy(), args.back()->copy());
}

void Power::print(std::ostream& stream) const {
  const AtomType type1 = args.front()->type;
  const AtomType type2 = args.back()->type;

  if (type1 == AtomType::SUM || type1 == AtomType::PRODUCT || type1 == AtomType::POWER || type1 == AtomType::NUMBER_FRAC) {
    stream << "(";
    args.front()->print(stream);
    stream << ")";
  }
  else {
    args.front()->print(stream);
  }

  stream << "^";

  if (type2 == AtomType::SUM || type2 == AtomType::PRODUCT || type2 == AtomType::POWER) {
    stream << "(";
    args.back()->print(stream);
    stream << ")";
  }
  else {
    args.back()->print(stream);
  }
}

bool Power::canBeEvaluated() const {
  return args.front()->canBeEvaluated() && args.back()->canBeEvaluated();
}

double Power::evaluate() const {
  return pow(args.front()->evaluate(), args.back()->evaluate());
}

std::unique_ptr<Atom> Power::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  return std::make_unique<Power>(args.front()->substitute(variableName, node), args.back()->substitute(variableName, node))
      ->simplify();
}

void Power::getVariablesNames(std::set<std::string>& names) const {
  args.front()->getVariablesNames(names);
  args.back()->getVariablesNames(names);
}

std::unique_ptr<Atom> Power::differentiate(const std::string& variable) const {
  std::unique_ptr<Atom> factor1 =
      std::make_unique<Power>(args.front()->copy(), std::make_unique<Sum>(args.back()->copy(), std::make_unique<Number>(-1)));

  std::unique_ptr<Product> sum1 = std::make_unique<Product>(args.back()->copy(), args.front()->differentiate(variable));

  std::vector<std::unique_ptr<Atom>> argsList;
  argsList.emplace_back(args.front()->copy());
  argsList.emplace_back(args.back()->copy()->differentiate(variable));
  argsList.emplace_back(std::make_unique<Log>(std::make_unique<Const>("e"), args.front()->copy()));

  std::unique_ptr<Product> sum2 = std::make_unique<Product>(std::move(argsList));

  std::unique_ptr<Atom> factor2 = std::make_unique<Sum>(std::move(sum1), std::move(sum2));

  return std::make_unique<Product>(std::move(factor1), std::move(factor2));
}

std::unique_ptr<Atom> Power::simplify() const {
  if (canBeEvaluated() && Utilities::isInt(evaluate())) {
    return std::make_unique<Number>(static_cast<int>(evaluate()));
  }

  std::unique_ptr<Atom> simplifiedBase = args.front()->simplify();
  std::unique_ptr<Atom> simplifiedExponent = args.back()->simplify();

  if (Atom::isNumber(simplifiedExponent) && simplifiedExponent->evaluate() == 0.0) {
    return std::make_unique<Number>(1);
  }
  if (Atom::isNumber(simplifiedExponent) && simplifiedExponent->evaluate() == 1.0) {
    return simplifiedBase;
  }

  if (Atom::isNumber(simplifiedBase) && simplifiedBase->evaluate() == 0.0) {
    return std::make_unique<Number>(0);
  }

  if (Atom::isNumber(simplifiedBase) && simplifiedBase->evaluate() == 1.0) {
    return std::make_unique<Number>(1);
  }

  if (simplifiedBase->type == AtomType::POWER) {

    return std::make_unique<Power>(simplifiedBase->args.front()->copy(),
                                   std::make_unique<Product>(simplifiedBase->args.back()->copy(), std::move(simplifiedExponent)));
  }

  if (simplifiedBase->canBeEvaluated() && simplifiedExponent->canBeEvaluated()) {
    const double result = evaluate();
    if (Utilities::isInt(result)) {
      return std::make_unique<Number>(static_cast<int>(evaluate()));
    }
  }

  if (simplifiedBase->type == AtomType::NUMBER_FRAC && simplifiedExponent->canBeEvaluated()) {
    Number* baseAsFrac = static_cast<Number*>(simplifiedBase.get());
    const double nominator = pow(baseAsFrac->getNominator(), simplifiedExponent->evaluate());
    const double denominator = pow(baseAsFrac->getDenominator(), simplifiedExponent->evaluate());

    if (Utilities::isInt(nominator) && Utilities::isInt(denominator)) {
      return std::make_unique<Number>(static_cast<int>(nominator), static_cast<int>(denominator));
    }
  }

  if (simplifiedExponent->type == AtomType::LOG) {
    if (simplifiedBase->compare(simplifiedExponent->args.front())) {
      return simplifiedExponent->args.back()->copy();
    }
  }

  return std::make_unique<Power>(std::move(simplifiedBase), std::move(simplifiedExponent));
}

std::unique_ptr<Atom> Power::expand() const {
  std::unique_ptr<Atom> expandedBase = args.front()->expand();
  std::unique_ptr<Atom> expandedExponent = args.back()->expand();

  if (expandedExponent->type == AtomType::SUM) {
    std::vector<std::unique_ptr<Atom>> resultFactors = {};

    std::for_each(expandedExponent->args.begin(), expandedExponent->args.end(),
                  [&resultFactors, &expandedBase](const auto& summand) {
                    resultFactors.emplace_back(std::make_unique<Power>(expandedBase->copy(), summand->copy()));
                  });

    return std::make_unique<Product>(std::move(resultFactors));
  }

  if (expandedBase->type == AtomType::PRODUCT) {
    std::vector<std::unique_ptr<Atom>> resultFactors = {};

    std::for_each(expandedBase->args.begin(), expandedBase->args.end(), [&resultFactors, &expandedExponent](const auto& factor) {
      resultFactors.emplace_back(std::make_unique<Power>(factor->copy(), expandedExponent->copy()));
    });

    return std::make_unique<Product>(std::move(resultFactors));
  }

  return std::make_unique<Power>(std::move(expandedBase), std::move(expandedExponent));
}

bool Power::compare(const std::unique_ptr<Atom>& other) const {
  if (other->type == AtomType::POWER) {
    return args.front()->compare(other->args.front()) && args.back()->compare(other->args.back());
  }

  return false;
}

std::unique_ptr<Atom> Power::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}
}  // namespace symbolicAlgebra::implementation

#endif  // POWER_DEFINE
#endif  // DEFINITION