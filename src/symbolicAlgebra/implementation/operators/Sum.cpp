#include "../Utilities.h"

#ifdef DEFINITION
#ifndef SUM_DEFINE
#define SUM_DEFINE
#define ATOM_SUM

namespace symbolicAlgebra::implementation {
Sum::Sum() : Atom(AtomType::SUM) {}

Sum::Sum(std::unique_ptr<Atom> addend) : Atom(AtomType::SUM, std::move(addend)) {}

Sum::Sum(std::unique_ptr<Atom> addend1, std::unique_ptr<Atom> addend2) : Atom(Atom::AtomType::SUM) {
  if (addend1->getType() == AtomType::SUM) {
    std::move(std::begin(addend1->getEditableArgs()), std::end(addend1->getEditableArgs()), std::back_inserter(args));
  }
  else {
    args.push_back(std::move(addend1));
  }

  if (addend2->getType() == AtomType::SUM) {
    std::move(std::begin(addend2->getEditableArgs()), std::end(addend2->getEditableArgs()), std::back_inserter(args));
  }
  else {
    args.push_back(std::move(addend2));
  }
}

Sum::Sum(std::vector<std::unique_ptr<Atom>> summands) : Atom(Atom::AtomType::SUM, std::move(summands)) {}

std::unique_ptr<Atom> Sum::copy() const {
  std::vector<std::unique_ptr<Atom>> resultSummands = {};
  resultSummands.reserve(args.size());

  std::for_each(args.begin(), args.end(),
                [&resultSummands](const auto& summand) { resultSummands.emplace_back(summand->copy()); });

  return std::make_unique<Sum>(std::move(resultSummands));
}

void Sum::print(std::ostream& stream) const {
  if (args.empty()) {
    stream << 0;
  }

  for (auto iter = args.cbegin(); iter != args.cend(); ++iter) {
    const bool cond1 = (Atom::isNumber(*iter) && iter->get()->evaluate() < 0);
    const bool cond2 = iter->get()->getType() == AtomType::PRODUCT && !iter->get()->getArgs().empty() &&
                       Atom::isNumber(iter->get()->getArgs().front()) && (iter->get()->getArgs().front()->evaluate() < 0);

    const bool printHasMinusAtBegin = cond1 || cond2;

    if (!printHasMinusAtBegin && iter != args.begin()) {
      stream << "+";
    }

    iter->get()->print(stream);
  }
}

bool Sum::canBeEvaluated() const {
  return std::all_of(args.cbegin(), args.cend(), [](const std::unique_ptr<Atom>& summand) { return summand->canBeEvaluated(); });
}

double Sum::evaluate() const {
  return std::accumulate(args.cbegin(), args.cend(), 0.0,
                         [](float result, const std::unique_ptr<Atom>& summand) { return result + summand->evaluate(); });
}

std::unique_ptr<Atom> Sum::substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const {
  std::vector<std::unique_ptr<Atom>> resultSummands = {};
  resultSummands.reserve(args.size());

  std::for_each(args.begin(), args.end(), [&resultSummands, &variableName, &node](const auto& summand) {
    resultSummands.emplace_back(summand->substitute(variableName, node));
  });

  return std::make_unique<Sum>(std::move(resultSummands))->simplify();
}

void Sum::getVariablesNames(std::set<std::string>& names) const {
  for (const std::unique_ptr<Atom>& summand : args) {
    summand->getVariablesNames(names);
  }
}

std::unique_ptr<Atom> Sum::differentiate(const std::string& variable) const {
  std::vector<std::unique_ptr<Atom>> resultSummands = {};
  resultSummands.reserve(args.size());

  std::for_each(args.begin(), args.end(), [&resultSummands, &variable](const auto& summand) {
    resultSummands.emplace_back(summand->differentiate(variable));
  });

  return std::make_unique<Sum>(std::move(resultSummands));
}

std::unique_ptr<Atom> Sum::simplify() const {
  if (canBeEvaluated() && Utilities::isInt(evaluate())) {

    return std::make_unique<Number>(static_cast<int>(evaluate()));
  }

  if (args.empty()) {  // () converts to 0

    return std::make_unique<Number>(0);
  }

  if (args.size() == 1) {  // (a) converts to a

    return args.front()->simplify();
  }

  std::vector<std::unique_ptr<Atom>> result;

  for (const std::unique_ptr<Atom>& summand : args) {  // a + (b + c) + d converts to a + b + c + d
    std::unique_ptr<Atom> simplifiedSummand = summand->simplify();
    if (simplifiedSummand->getType() == AtomType::SUM) {
      std::move(std::begin(simplifiedSummand->getEditableArgs()), std::end(simplifiedSummand->getEditableArgs()), std::back_inserter(result));
    }
    else {
      result.emplace_back(std::move(simplifiedSummand));
    }
  }

  for (auto iter1 = result.begin(); iter1 != result.end();) {
    if (Atom::isNumber(*iter1)) {  // number will be grouped on the end of function
      ++iter1;
      continue;
    }

    if ((*iter1)->getType() == AtomType::POWER && Atom::isNumber((*iter1)->getArgs().front()) && Atom::isNumber((*iter1)->getArgs().back())) {
      ++iter1;
      continue;
    }

    std::unique_ptr<Atom> product1Coeff = std::make_unique<Number>(1);
    std::unique_ptr<Atom> element1 = (*iter1)->copy();

    if (element1->getType() == AtomType::PRODUCT) {  // if first factor of product is number then save it and divide product by it
      if (!element1->getArgs().empty() && element1->getArgs().front()->canBeEvaluated()) {
        product1Coeff = std::move(element1->getEditableArgs().front());
        element1->getEditableArgs().erase(element1->getEditableArgs().begin());
        element1 = element1->simplify();
      }
    }

    auto iter2 = iter1;
    ++iter2;

    for (; iter2 != result.end();) {
      if (Atom::isNumber(*iter2)) {  // number will be grouped on the end of function
        ++iter2;
        continue;
      }

      if ((*iter2)->getType() == AtomType::POWER && Atom::isNumber((*iter2)->getArgs().front()) && Atom::isNumber((*iter2)->getArgs().back())) {
        ++iter2;
        continue;
      }

      std::unique_ptr<Atom> product2Coeff = std::make_unique<Number>(1);
      std::unique_ptr<Atom> element2 = (*iter2)->copy();

      if (element2->getType() == AtomType::PRODUCT) {  // if first factor of product is number then save it and divide product by it
        if (!element2->getArgs().empty() && element2->getArgs().front()->canBeEvaluated() &&
            (Atom::isNumber(element2->getArgs().front()) || element2->getArgs().front()->getType() == AtomType::POWER)) {
          product2Coeff = std::move(element2->getEditableArgs().front());
          element2->getEditableArgs().erase(element2->getEditableArgs().begin());
          element2 = element2->simplify();
        }
      }

      if (element1->compare(element2)) {
        product1Coeff = std::make_unique<Sum>(product1Coeff->copy(), product2Coeff->copy());
        iter2 = result.erase(iter2);
      }
      else {
        ++iter2;
      }
    }

    if (product1Coeff->canBeEvaluated() && product1Coeff->evaluate() == 0.0) {
      iter1 = result.erase(iter1);
    }
    else {
      *(iter1) = std::make_unique<Product>(product1Coeff->copy(), element1->copy())->simplify();
      ++iter1;
    }
  }

  Number numbersSum(0);
  for (auto iter = result.begin(); iter != result.end();) {
    if (Atom::isNumber(*iter)) {
      Number* casted = static_cast<Number*>(iter->get());
      numbersSum += (*casted);

      iter = result.erase(iter);
    }
    else if ((*iter)->getType() == AtomType::POWER && Atom::isNumber((*iter)->getArgs().front()) && Atom::isNumber((*iter)->getArgs().back())) {
      Number* casted1 = static_cast<Number*>((*iter)->getArgs().front().get());
      Number* casted2 = static_cast<Number*>((*iter)->getArgs().back().get());
      numbersSum += ((*casted1) ^ (*casted2));

      iter = result.erase(iter);
    }
    else {
      ++iter;
    }
  }

  if (!Utilities::isZero(numbersSum.evaluate())) {
    result.insert(result.begin(), std::make_unique<Number>(numbersSum));
  }

  if (result.empty()) {

    return std::make_unique<Number>(0);
  }

  if (result.size() == 1) {

    return std::move(result.front());
  }

  return std::make_unique<Sum>(std::move(result));
}

std::unique_ptr<Atom> Sum::expand() const {
  std::vector<std::unique_ptr<Atom>> resultSummands = {};
  resultSummands.reserve(args.size());

  std::for_each(args.begin(), args.end(),
                [&resultSummands](const auto& summand) { resultSummands.emplace_back(summand->expand()); });

  return std::make_unique<Sum>(std::move(resultSummands));
}

bool Sum::compare(const std::unique_ptr<Atom>& other) const {
  const std::unique_ptr<Atom> expSimOther = other->expand()->simplify();

  if (expSimOther->getType() != Atom::AtomType::SUM) {
    return false;
  }

  std::unique_ptr<Atom> result = copy()->expand()->simplify();
  std::for_each(expSimOther->getArgs().begin(), expSimOther->getArgs().end(), [&result](const std::unique_ptr<Atom>& arg) {
    result->getEditableArgs().push_back(std::make_unique<Product>(std::make_unique<Number>(-1), arg->copy()));
  });

  const std::unique_ptr<Atom> zero = result->expand()->simplify();

  return zero->canBeEvaluated() && zero->evaluate() == 0.0;
}

std::unique_ptr<Atom> Sum::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  std::vector<std::unique_ptr<Atom>> resultSummands = {};
  resultSummands.reserve(args.size());

  std::for_each(args.begin(), args.end(),
                [&resultSummands, &node](const auto& summand) { resultSummands.emplace_back(summand->coefficient(node)); });

  return std::make_unique<Sum>(std::move(resultSummands))->simplify();
}
}  // namespace symbolicAlgebra::implementation

#endif  // SUM_DEFINE
#endif  // DEFINITION