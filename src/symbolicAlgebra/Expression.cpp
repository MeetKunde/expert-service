#include "Expression.h"

namespace symbolicAlgebra {
Expression::Expression() : root(std::make_unique<implementation::Number>(0)) {}

Expression::Expression(std::unique_ptr<implementation::Atom> root) : root(std::move(root)) {}

Expression::Expression(const std::string& expression) : Expression(ExpressionBuilder::fromString(expression)) {}

Expression::Expression(const Expression& other) {
  if (other.root == nullptr) {
    root = nullptr;
  }
  else {
    root = other.root->copy();
  }
}

Expression::Expression(Expression&& other) {
  root = std::move(other.root);
}

Expression& Expression::operator=(const Expression& other) {
  if (this != &other) {
    if (other.root == nullptr) {
      root = nullptr;
    }
    else {
      root = other.root->copy();
    }
  }

  return *this;
}

Expression& Expression::operator=(Expression&& other) {
  if(this != &other) {
    root = std::move(other.root);
  }

  return *this;
}

Expression::~Expression() {}

std::string Expression::getString() const {
  if (root == nullptr) {
    return "";
  }

  std::stringstream streamOfString;
  root->print(streamOfString);

  return streamOfString.str();
}

bool Expression::canBeFullEvaluated() const {
  if (root == nullptr) {
    return false;
  }

  return root->canBeEvaluated();
}

double Expression::evaluate() const {
  if (root == nullptr) {
    return 0;
  }

  return root->evaluate();
}

void Expression::simplify() {
  if (root == nullptr) {
    return;
  }

  root = root->simplify();
}

void Expression::expand() {
  if (root == nullptr) {
    return;
  }

  root = root->expand();
}

void Expression::substitute(const std::string& variableName, const Expression& subExpression) {
  if (root == nullptr) {
    return;
  }

  root = root->substitute(variableName, subExpression.root);
}

Expression Expression::getCoefficient(const Expression& subExpression) {
  if (root == nullptr) {
    return Expression();
  }

  return Expression(root->coefficient(subExpression.root));
}

void Expression::getIncludedVariables(std::set<std::string>& names) const {
  if (root == nullptr) {
    return;
  }

  root->getVariablesNames(names);
}

void Expression::differentiate(const std::string& variable) {
  if (root == nullptr) {
    return;
  }

  root = root->differentiate(variable);
}

void Expression::differentiate(const std::string& variable, unsigned int times) {
  if (root == nullptr) {
    return;
  }

  for (unsigned int i = 0; i < times; i++) {
    root = root->differentiate(variable);
  }
}

Expression& Expression::operator+=(const Expression& rhs) {
  root = std::make_unique<implementation::Sum>(root->copy(), rhs.root->copy());

  return *this;
}

Expression operator+(Expression lhs, const Expression& rhs) {
  lhs += rhs;
  return lhs;
}

Expression& Expression::operator-=(const Expression& rhs) {
  root = std::make_unique<implementation::Sum>(
      root->copy(), std::make_unique<implementation::Product>(std::make_unique<implementation::Number>(-1), rhs.root->copy()));

  return *this;
}

Expression operator-(Expression lhs, const Expression& rhs) {
  lhs -= rhs;
  return lhs;
}

Expression& Expression::operator*=(const Expression& rhs) {
  root = std::make_unique<implementation::Product>(root->copy(), rhs.root->copy());

  return *this;
}

Expression operator*(Expression lhs, const Expression& rhs) {
  lhs *= rhs;
  return lhs;
}

Expression& Expression::operator/=(const Expression& rhs) {
  root = std::make_unique<implementation::Product>(
      root->copy(), std::make_unique<implementation::Power>(rhs.root->copy(), std::make_unique<implementation::Number>(-1)));

  return *this;
}

Expression operator/(Expression lhs, const Expression& rhs) {
  lhs /= rhs;
  return lhs;
}

Expression& Expression::operator^=(const Expression& rhs) {
  root = std::make_unique<implementation::Power>(root->copy(), rhs.root->copy());

  return *this;
}

Expression operator^(Expression lhs, const Expression& rhs) {
  lhs ^= rhs;
  return lhs;
}
bool operator==(const Expression& lhs, const Expression& rhs) {
  return lhs.root->compare(rhs.root);
}

bool operator!=(const Expression& lhs, const Expression& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const Expression& expression) {
  if (expression.root != nullptr) {
    expression.root->print(stream);
  }

  return stream;
}

}  // namespace symbolicAlgebra