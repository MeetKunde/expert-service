#include "../Utilities.h"

#ifdef DEFINITION
#ifndef PRODUCT_DEFINE
#define PRODUCT_DEFINE
#define ATOM_PRODUCT

namespace symbolicAlgebra::implementation {
Product::Product() : Atom(AtomType::PRODUCT) {}

Product::Product(std::unique_ptr<Atom> factor) : Atom(AtomType::PRODUCT, std::move(factor)) {}

Product::Product(std::unique_ptr<Atom> factor1, std::unique_ptr<Atom> factor2) : Atom(AtomType::PRODUCT) {
  if (factor1->type == AtomType::PRODUCT) {
    std::move(std::begin(factor1->args), std::end(factor1->args), std::back_inserter(args));
  }
  else {
    args.push_back(std::move(factor1));
  }

  if (factor2->type == AtomType::PRODUCT) {
    std::move(std::begin(factor2->args), std::end(factor2->args), std::back_inserter(args));
  }
  else {
    args.push_back(std::move(factor2));
  }

  preprocessConstructorArgs();
}

Product::Product(std::vector<std::unique_ptr<Atom>> factors) : Atom(AtomType::PRODUCT, std::move(factors)) {
  preprocessConstructorArgs();
}

std::unique_ptr<Atom> Product::copy() const {
  std::vector<std::unique_ptr<Atom>> resultFactors = {};
  resultFactors.reserve(args.size());

  std::for_each(args.begin(), args.end(), [&resultFactors](const auto& factor) { resultFactors.emplace_back(factor->copy()); });

  return std::make_unique<Product>(std::move(resultFactors));
}

void Product::print(std::ostream& stream) const {
  if (args.empty()) {
    stream << 1;
  }
  else if (args.size() == 1) {
    args.front()->print(stream);
  }
  else {
    for (auto iter = args.cbegin(); iter != args.cend(); ++iter) {
      if (iter != args.begin()) {
        stream << "*";
      }

      if (Atom::isNumber(*iter) && iter->get()->evaluate() == -1) {
        stream << "-";
        ++iter;
        if (iter == args.cend()) {
          break;
        }
      }

      if (iter->get()->type == AtomType::SUM) {
        stream << "(";
        iter->get()->print(stream);
        stream << ")";
      }
      else {
        iter->get()->print(stream);
      }
    }
  }
}

inline bool Product::canBeEvaluated() const {
  return std::all_of(args.cbegin(), args.cend(), [](const std::unique_ptr<Atom>& factor) { return factor->canBeEvaluated(); });
}

double Product::evaluate() const {
  return std::accumulate(args.cbegin(), args.cend(), 1.0,
                         [](float result, const std::unique_ptr<Atom>& factor) { return result * factor->evaluate(); });
}

std::unique_ptr<Atom> Product::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  std::vector<std::unique_ptr<Atom>> resultFactors = {};
  resultFactors.reserve(args.size());

  std::for_each(args.begin(), args.end(), [&resultFactors, &variableName, &node](const auto& factor) {
    resultFactors.emplace_back(factor->substitute(variableName, node));
  });

  return std::make_unique<Product>(std::move(resultFactors))->simplify();
}

void Product::getVariablesNames(std::set<std::string>& names) const {
  for (const std::unique_ptr<Atom>& factor : args) {
    factor->getVariablesNames(names);
  }
}

std::unique_ptr<Atom> Product::differentiate(const std::string& variable) const {
  std::vector<std::unique_ptr<Atom>> resultSummands = {};

  for (auto iter1 = args.begin(); iter1 != args.end(); ++iter1) {
    std::vector<std::unique_ptr<Atom>> factors = {};
    for (auto iter2 = args.begin(); iter2 != args.end(); ++iter2) {
      if (iter1 == iter2) {
        factors.push_back((*iter2)->differentiate(variable));
      }
      else {
        factors.push_back((*iter2)->copy());
      }
    }
    resultSummands.push_back(std::make_unique<Product>(std::move(factors)));
  }

  return std::make_unique<Sum>(std::move(resultSummands));
}

std::unique_ptr<Atom> Product::simplify() const {
  if (canBeEvaluated() && Utilities::isInt(evaluate())) {

    return std::make_unique<Number>(static_cast<int>(evaluate()));
  }

  if (args.empty()) {  // () converts to 1

    return std::make_unique<Number>(1);
  }

  if (args.size() == 1) {  // (a) converts to a

    return args.front()->simplify();
  }

  std::vector<std::unique_ptr<Atom>> result;

  for (const std::unique_ptr<Atom>& factor : args) {  // a * (b * c) * d converts to a * b * c * d
    std::unique_ptr<Atom> simplifiedFactor = factor->simplify();
    if (simplifiedFactor->type == AtomType::PRODUCT) {
      std::move(std::begin(simplifiedFactor->args), std::end(simplifiedFactor->args), std::back_inserter(result));
    }
    else {
      result.emplace_back(std::move(simplifiedFactor));
    }
  }

  for (std::vector<std::unique_ptr<Atom>>::iterator iter1 = result.begin(); iter1 != result.end(); ++iter1) {
    if (Atom::isNumber(*iter1)) {  // number will be grouped on the end of function
      if (Utilities::isZero((*iter1)->evaluate())) {

        return std::make_unique<Number>(0);
      }
      continue;
    }

    if ((*iter1)->type == AtomType::POWER && Atom::isNumber((*iter1)->args.front()) && Atom::isNumber((*iter1)->args.back())) {
      continue;
    }

    std::unique_ptr<Atom> power1Exponent = std::make_unique<Number>(1);
    std::unique_ptr<Atom> element1 = (*iter1)->copy();

    if (element1->type == AtomType::POWER) {
      power1Exponent = std::move(element1->args.back());
      element1 = std::move(element1->args.front());
    }

    std::vector<std::unique_ptr<Atom>>::iterator iter2 = iter1;
    ++iter2;

    for (; iter2 != result.end();) {
      if (Atom::isNumber(*iter2)) {  // number will be grouped on the end of function
        ++iter2;
        continue;
      }

      if ((*iter2)->type == AtomType::POWER && Atom::isNumber((*iter2)->args.front()) && Atom::isNumber((*iter2)->args.back())) {
        ++iter2;
        continue;
      }

      std::unique_ptr<Atom> element2 = (*iter2)->copy();
      std::unique_ptr<Atom> power2Exponent = std::make_unique<Number>(1);

      if (element2->type == AtomType::POWER) {
        power2Exponent = std::move(element2->args.back());
        element2 = std::move(element2->args.front());
      }

      if (element1->compare(element2)) {
        power1Exponent = std::make_unique<Sum>(power1Exponent->copy(), power2Exponent->copy());
        iter2 = result.erase(iter2);
      }
      else {
        ++iter2;
      }
    }

    if (power1Exponent->canBeEvaluated() && power1Exponent->evaluate() == 0.0) {
      (*iter1) = std::make_unique<Number>(1);
    }
    else if (power1Exponent->canBeEvaluated() && power1Exponent->evaluate() == 1.0) {
      (*iter1) = std::move(element1);
    }
    else {
      (*iter1) = std::make_unique<Power>(element1->copy(), power1Exponent->copy())->simplify();
    }
  }

  Number numbersProduct(1);
  for (std::vector<std::unique_ptr<Atom>>::iterator iter = result.begin(); iter != result.end();) {
    if (Atom::isNumber(*iter)) {
      Number* casted = static_cast<Number*>(iter->get());
      numbersProduct *= (*casted);

      iter = result.erase(iter);
    }
    else if ((*iter)->type == AtomType::POWER && Atom::isNumber((*iter)->args.front()) && Atom::isNumber((*iter)->args.back())) {
      Number* casted1 = static_cast<Number*>((*iter)->args.front().get());
      Number* casted2 = static_cast<Number*>((*iter)->args.back().get());
      numbersProduct *= ((*casted1) ^ (*casted2));

      iter = result.erase(iter);
    }
    else {
      ++iter;
    }
  }

  if (Utilities::isZero(numbersProduct.evaluate())) {

    return std::make_unique<Number>(0);
  }

  if (!Utilities::isOne(numbersProduct.evaluate())) {
    result.insert(result.begin(), std::make_unique<Number>(numbersProduct));
  }

  if (result.empty()) {

    return std::make_unique<Number>(1);
  }

  if (result.size() == 1) {

    return std::move(result.front());
  }

  if (result.size() == 2 && result.front()->canBeEvaluated() && result.front()->evaluate() == -1 &&
      result.back()->type == AtomType::SUM) {
    std::vector<std::unique_ptr<Atom>> negativeSummands;
    for (std::vector<std::unique_ptr<Atom>>::iterator iter = result.back()->args.begin(); iter != result.back()->args.end();
         ++iter) {
      negativeSummands.push_back(std::make_unique<Product>(std::make_unique<Number>(-1), (*iter)->copy())->simplify());
    }

    return std::make_unique<Sum>(std::move(negativeSummands));
  }

  return std::make_unique<Product>(std::move(result));
}

std::unique_ptr<Atom> Product::expand() const {
  std::unique_ptr<Atom> thisCopy = copy();
  std::vector<std::unique_ptr<Atom>> resultFactors = {};

  std::vector<std::unique_ptr<Atom>>::iterator iter;
  std::unique_ptr<Atom> expandedFactor;
  for (iter = thisCopy->args.begin(); iter != thisCopy->args.end(); ++iter) {
    expandedFactor = (*iter)->expand();
    if (expandedFactor->type == AtomType::SUM) {
      break;
    }

    resultFactors.emplace_back(expandedFactor->copy());
  }

  if (iter != thisCopy->args.end()) {
    thisCopy->args.erase(iter);
    std::vector<std::unique_ptr<Atom>> resultSummands = {};
    std::for_each(expandedFactor->args.begin(), expandedFactor->args.end(),
                  [&thisCopy, &resultSummands](const std::unique_ptr<Atom>& arg) {
                    resultSummands.push_back(std::make_unique<Product>(arg->copy(), thisCopy->copy())->expand());
                  });

    return std::make_unique<Sum>(std::move(resultSummands));
  }

  return std::make_unique<Product>(std::move(resultFactors));
}

bool Product::compare(const std::unique_ptr<Atom>& other) const {
  const std::unique_ptr<Atom> expSimOther = other->expand()->simplify();

  if (expSimOther->type != Atom::AtomType::PRODUCT) {
    return false;
  }

  std::unique_ptr<Atom> result = copy();
  std::for_each(expSimOther->args.begin(), expSimOther->args.end(), [&result](const std::unique_ptr<Atom>& arg) {
    result->args.push_back(std::make_unique<Power>(arg->copy(), std::make_unique<Number>(-1)));
  });

  const std::unique_ptr<Atom> zero = result->expand()->simplify();

  return zero->canBeEvaluated() && zero->evaluate() == 1.0;
}

std::unique_ptr<Atom> Product::coefficient(const std::unique_ptr<Atom>& node) const {
  if (node->type == Atom::AtomType::PRODUCT) {
    const std::unique_ptr<Atom> thisCopy = copy();

    for (std::vector<std::unique_ptr<Atom>>::iterator iter1 = node->args.begin(); iter1 != node->args.end(); ++iter1) {
      const std::vector<std::unique_ptr<Atom>>::iterator iterToElementToDelete = std::find_if(
          thisCopy->args.begin(), thisCopy->args.end(), [&iter1](const auto& element) { return (*iter1)->compare((element)); });

      if (iterToElementToDelete == thisCopy->args.end()) {

        return std::make_unique<Number>(0);
      }

      thisCopy->args.erase(iterToElementToDelete);
    }

    if (thisCopy->args.empty()) {

      return std::make_unique<Number>(1);
    }

    return thisCopy->simplify();
  }

  for (std::vector<std::unique_ptr<Atom>>::const_iterator iter1 = args.begin(); iter1 != args.end(); ++iter1) {
    const std::unique_ptr<Atom> coeff = (*iter1)->coefficient(node);
    coeff->simplify();
    if (!coeff->canBeEvaluated() || coeff->evaluate() != 0.0) {
      std::vector<std::unique_ptr<Atom>> resultFactors = {};
      resultFactors.emplace_back(coeff->copy());
      for (std::vector<std::unique_ptr<Atom>>::const_iterator iter2 = args.begin(); iter2 != args.end(); ++iter2) {
        if (iter1 != iter2) {
          resultFactors.push_back((*iter2)->copy());
        }
      }

      return std::make_unique<Product>(std::move(resultFactors))->simplify();
    }
  }

  return std::make_unique<Number>(0);
}

void Product::preprocessConstructorArgs() {
  // if more than one factor has minus then additional minuses are simplified

  unsigned int minusesCount = 0;
  std::vector<std::unique_ptr<Atom>*> negativeNumbers;
  std::for_each(args.begin(), args.end(), [&minusesCount, &negativeNumbers](auto& arg) {
    if (Atom::isNumber(arg)) {
      if (arg->evaluate() < 0) {
        minusesCount++;
        negativeNumbers.emplace_back(&arg);
      }
    }
    else if (arg->type == AtomType::PRODUCT && arg->args.size() > 0 && Atom::isNumber(arg->args.front())) {
      if (arg->args.front()->evaluate() < 0) {
        minusesCount++;
        negativeNumbers.emplace_back(&arg->args.front());
      }
    }
  });

  if (minusesCount > 1) {
    std::vector<std::unique_ptr<Atom>*>::iterator beginIt = negativeNumbers.begin();
    if (minusesCount % 2 == 1) {
      beginIt = std::next(beginIt);
    }

    std::for_each(beginIt, negativeNumbers.end(), [](const auto& num) {
      Number* numCasted = static_cast<Number*>(num->get());

      numCasted->rational *= -1;
      numCasted->nominator *= -1;
    });
  }
}
}  // namespace symbolicAlgebra::implementation

#endif  // PRODUCT_DEFINE
#endif  // DEFINITION