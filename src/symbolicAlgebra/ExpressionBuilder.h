#ifndef EXPRESSION_BUILDER_H
#define EXPRESSION_BUILDER_H

#include "Expression.h"
#include <vector>
#include <stack>
#include <map>

namespace symbolicAlgebra {
class ExpressionBuilder {
 public:
  static std::unique_ptr<implementation::Atom> fromString(const std::string& expression);

 private:
  enum class ParsingTypes { VARIABLE, NUMBER, CONSTANT, OPERATOR };

  static inline bool isDigit(char chr) { return std::isdigit(chr) != 0; }
  static inline bool isLetter(char chr) {
    return (std::isalpha(chr) != 0) || chr == '|' || chr == '<' || chr == '>' || chr == '\'';
  }
  static inline bool isOperator(char chr) { return operators.find(chr) != operators.end(); }
  static std::string toLowerCase(const std::string& str);
  static bool isSpecialFunction(const std::string& str);
  static bool isSpecialConstant(const std::string& str);

  static const std::map<char, std::pair<int, bool>> operators;
  static const std::pair<char, char> parenthesis;

};
}  // namespace symbolicAlgebra

#endif /* EXPRESSION_BUILDER_H */