#include "Number.h"
#include "../Utilities.h"

#ifdef DEFINITION
#ifndef NUMBER_DEFINE
#define NUMBER_DEFINE
#define ATOM_NUMBER

namespace symbolicAlgebra::implementation {
Number::Number(int value) : Atom(AtomType::NUMBER_INT), nominator(value), denominator(1), rational(1.0 * value) {}

Number::Number(double value)
    : Atom(AtomType::NUMBER_RAT), nominator(static_cast<int>(value)), denominator(1), rational(1.0 * value) {}

Number::Number(int nominatorValue, int denominatorValue)
    : Atom(AtomType::NUMBER_FRAC),
      nominator(nominatorValue),
      denominator(denominatorValue),
      rational(1.0 * nominatorValue / denominatorValue) {
  if ((nominatorValue < 0 && denominatorValue < 0) || (nominatorValue > 0 && denominatorValue < 0)) {
    nominator *= -1;
    denominator *= -1;
  }
}

std::unique_ptr<Atom> Number::copy() const {
  if (type == AtomType::NUMBER_INT) {
    return std::make_unique<Number>(nominator);
  }

  if (type == AtomType::NUMBER_RAT) {
    return std::make_unique<Number>(rational);
  }

  // type == AtomType::NUMBER_FRAC
  return std::make_unique<Number>(nominator, denominator);
}

void Number::print(std::ostream& stream) const {
  if (type == AtomType::NUMBER_INT) {
    stream << nominator;

    return;
  }

  if (type == AtomType::NUMBER_RAT) {
    stream << rational;

    return;
  }

  // type == AtomType::NUMBER_FRAC
  stream << nominator << "/" << denominator;
}

bool Number::canBeEvaluated() const {
  return true;
}

double Number::evaluate() const {
  return rational;
}

std::unique_ptr<Atom> Number::substitute(const std::string& /*variableName*/, const std::unique_ptr<Atom>& /*val*/) const {
  return copy();
}

void Number::getVariablesNames(std::set<std::string>& /*names*/) const {}

std::unique_ptr<Atom> Number::differentiate(const std::string& /*variable*/) const {
  return std::make_unique<Number>(0);
}

std::unique_ptr<Atom> Number::simplify() const {
  if (type == AtomType::NUMBER_FRAC && nominator <= 0 && denominator < 0) {
    return std::make_unique<Number>(nominator * -1, denominator * -1);
  }

  return copy();
}

std::unique_ptr<Atom> Number::expand() const {
  return copy();
}

bool Number::compare(const std::unique_ptr<Atom>& other) const {
  if (Atom::isNumber(other)) {
    Number* castedNumber = static_cast<Number*>(other.get());
    return Utilities::areEqual(rational, castedNumber->rational);
  }

  return false;
}

std::unique_ptr<Atom> Number::coefficient(const std::unique_ptr<Atom>& node) const {
  if (compare(node)) {
    return std::make_unique<Number>(1);
  }

  return std::make_unique<Number>(0);
}

Number& Number::operator+=(const Number& rhs) {
  if (type == AtomType::NUMBER_RAT || rhs.type == AtomType::NUMBER_RAT) {
    *this = Number(rational + rhs.rational);

    return *this;
  }

  if (type == AtomType::NUMBER_FRAC || rhs.type == AtomType::NUMBER_FRAC) {
    const int resultDenominator = std::lcm(denominator, rhs.denominator);
    const int resultNominator = nominator * resultDenominator / denominator + rhs.nominator * resultDenominator / rhs.denominator;

    const int ndGcd = std::gcd(resultNominator, resultDenominator);

    *this = Number(resultNominator / ndGcd, resultDenominator / ndGcd);

    return *this;
  }

  // type == AtomType::NUMBER_INT && rhs.type == AtomType::NUMBER_INT
  *this = Number(nominator + rhs.nominator);

  return *this;
}

Number operator+(Number lhs, const Number& rhs) {
  lhs += rhs;
  return lhs;
}

Number& Number::operator-=(const Number& rhs) {
  if (type == AtomType::NUMBER_RAT || rhs.type == AtomType::NUMBER_RAT) {
    *this = Number(rational - rhs.rational);
  }
  else if (type == AtomType::NUMBER_FRAC || rhs.type == AtomType::NUMBER_FRAC) {
    const int resultDenominator = std::lcm(denominator, rhs.denominator);
    const int resultNominator = nominator * resultDenominator / denominator - rhs.nominator * resultDenominator / rhs.denominator;

    const int ndGcd = std::gcd(resultNominator, resultDenominator);

    *this = Number(resultNominator / ndGcd, resultDenominator / ndGcd);
  }
  else {  // type == AtomType::NUMBER_INT && rhs.type == AtomType::NUMBER_INT
    *this = Number(nominator - rhs.nominator);
  }

  return *this;
}

Number operator-(Number lhs, const Number& rhs) {
  lhs -= rhs;
  return lhs;
}

Number& Number::operator*=(const Number& rhs) {
  if (type == AtomType::NUMBER_RAT || rhs.type == AtomType::NUMBER_RAT) {
    *this = Number(rational * rhs.rational);
  }
  else if (type == AtomType::NUMBER_FRAC || rhs.type == AtomType::NUMBER_FRAC) {
    const int resultNominator = nominator * rhs.nominator;
    const int resultDenominator = denominator * rhs.denominator;

    const int ndGcd = std::gcd(resultNominator, resultDenominator);

    *this = Number(resultNominator / ndGcd, resultDenominator / ndGcd);
  }
  else {  // type == AtomType::NUMBER_INT && rhs.type == AtomType::NUMBER_INT
    *this = Number(nominator * rhs.nominator);
  }

  return *this;
}

Number operator*(Number lhs, const Number& rhs) {
  lhs *= rhs;
  return lhs;
}

Number& Number::operator/=(const Number& rhs) {
  if (type == AtomType::NUMBER_RAT || rhs.type == AtomType::NUMBER_RAT) {
    *this = Number(rational / rhs.rational);
  }
  else if (type == AtomType::NUMBER_FRAC || rhs.type == AtomType::NUMBER_FRAC) {
    const int resultNominator = nominator * rhs.denominator;
    const int resultDenominator = denominator * rhs.nominator;

    const int ndGcd = std::gcd(resultNominator, resultDenominator);

    if (resultDenominator == ndGcd) {
      *this = Number(resultNominator / ndGcd);
    }
    else if (resultDenominator == -ndGcd) {
      *this = Number(-resultNominator / ndGcd);
    }
    else {
      *this = Number(resultNominator / ndGcd, resultDenominator / ndGcd);
    }
  }
  else {  // type == AtomType::NUMBER_INT && rhs.type == AtomType::NUMBER_INT
    const double result = 1.0 * nominator / rhs.nominator;

    if (Utilities::isInt(result)) {
      *this = Number(static_cast<int>(result));
    }
    else {
      const int ndGcd = std::gcd(nominator, rhs.nominator);

      *this = Number(nominator / ndGcd, rhs.nominator / ndGcd);
    }
  }

  return *this;
}

Number operator/(Number lhs, const Number& rhs) {
  lhs /= rhs;
  return lhs;
}

Number& Number::operator^=(const Number& rhs) {
  if (type == AtomType::NUMBER_RAT || rhs.type == AtomType::NUMBER_RAT) {
    *this = Number(pow(rational, rhs.rational));

    return *this;
  }

  if (rhs.type == AtomType::NUMBER_FRAC) {
    *this = Number(pow(rational, rhs.rational));

    return *this;
  }

  if (type == AtomType::NUMBER_FRAC) {
    if (rhs.nominator == 0) {
      *this = Number(1);
    }
    else if (rhs.nominator > 0) {
      *this = Number(static_cast<int>(pow(nominator, rhs.nominator)), static_cast<int>(pow(denominator, rhs.nominator)));
    }
    else {
      *this = Number(static_cast<int>(pow(denominator, -rhs.nominator)), static_cast<int>(pow(nominator, -rhs.nominator)));
    }

    return *this;
  }

  // type == AtomType::NUMBER_INT && rhs.type == AtomType::NUMBER_INT
  if (rhs.nominator == 0) {
    *this = Number(1);
  }
  else if (rhs.nominator > 0) {
    *this = Number(static_cast<int>(pow(rational, rhs.rational)));
  }
  else {
    *this = Number(1, static_cast<int>(pow(rational, -rhs.rational)));
  }

  return *this;
}

Number operator^(Number lhs, const Number& rhs) {
  lhs ^= rhs;
  return lhs;
}

bool operator==(const Number& lhs, const Number& rhs) {
  return Utilities::areEqual(lhs.rational, rhs.rational);
}

bool operator!=(const Number& lhs, const Number& rhs) {
  return !(lhs == rhs);
}

}  // namespace symbolicAlgebra::implementation

#endif  // NUMBER_DEFINE
#endif  // DEFINITION