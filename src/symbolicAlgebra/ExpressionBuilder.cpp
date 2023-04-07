#include "ExpressionBuilder.h"

namespace symbolicAlgebra {
const std::map<char, std::pair<int, bool>> ExpressionBuilder::operators = {{'+', {2, 0}},  // binary +
                                                                           {'-', {2, 0}},  // binary -
                                                                           {'p', {4, 1}},  // unary +
                                                                           {'m', {4, 1}},  // unary -
                                                                           {'*', {3, 0}}, {'/', {3, 0}}, {'^', {4, 1}}};

const std::pair<char, char> ExpressionBuilder::parenthesis = {'(', ')'};

std::string ExpressionBuilder::toLowerCase(const std::string& str) {
  std::string lowerCase(str);

  transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);

  return lowerCase;
}

bool ExpressionBuilder::isSpecialFunction(const std::string& str) {
  const std::string strLowerCase = ExpressionBuilder::toLowerCase(str);

  return strLowerCase == "sin" || strLowerCase == "cos" || strLowerCase == "tan" || strLowerCase == "cot" ||
         strLowerCase == "log" || strLowerCase == "ln" || strLowerCase == "sqrt" || strLowerCase == "exp";
}

bool ExpressionBuilder::isSpecialConstant(const std::string& str) {
  const std::string strLowerCase = ExpressionBuilder::toLowerCase(str);

  return strLowerCase == "e" || strLowerCase == "pi";
}

// Parsing Method = Shunting Yard Algorithm
std::unique_ptr<implementation::Atom> ExpressionBuilder::fromString(const std::string& expression) {
  std::stack<char> operatorsStack;
  std::stack<char> functionsStack;
  std::vector<std::pair<ParsingTypes, std::string>> postFixVector;
  std::istringstream iss(expression);
  char tmp;
  char last = '!';
  int num;

  while (static_cast<int>(iss.tellg()) != EOF) {
    char token = static_cast<char>(iss.peek());
    if (isLetter(token)) {
      std::string stringTmp;
      stringTmp += token;
      iss >> tmp;

      while (static_cast<int>(iss.tellg()) != EOF && isLetter(static_cast<char>(iss.peek()))) {
        token = static_cast<char>(iss.peek());
        stringTmp += token;
        iss >> tmp;
      }

      if (ExpressionBuilder::isSpecialFunction(stringTmp)) {
        const std::string lowercase = ExpressionBuilder::toLowerCase(stringTmp);
        if (lowercase == "sin") {
          functionsStack.push('s');
        }
        else if (lowercase == "cos") {
          functionsStack.push('c');
        }
        else if (lowercase == "tan") {
          functionsStack.push('t');
        }
        else if (lowercase == "cot") {
          functionsStack.push('C');
        }
        else if (lowercase == "log") {
          functionsStack.push('l');
        }
        else if (lowercase == "ln") {
          functionsStack.push('L');
        }
        else if (lowercase == "sqrt") {
          functionsStack.push('S');
        }
        else if (lowercase == "exp") {
          functionsStack.push('e');
        }
      }
      else if (ExpressionBuilder::isSpecialConstant(stringTmp)) {
        postFixVector.push_back({ParsingTypes::CONSTANT, ExpressionBuilder::toLowerCase(stringTmp)});
      }
      else {
        postFixVector.push_back({ParsingTypes::VARIABLE, stringTmp});
      }

      //last = stringTmp.back();
      last = ' ';
    }
    else if (isDigit(token)) {
      iss >> num;
      const std::string strNum = std::to_string(num);
      postFixVector.push_back({ParsingTypes::NUMBER, strNum});
      //last = strNum.back();
      last = ' ';
    }
    else if (isOperator(token)) {
      if (last == '!' || isOperator(last) || last == parenthesis.first) {
        if (token == '-') {
          token = 'm';
        }
        if (token == '+') {
          token = 'p';
        }
      }

      char offset = 0;
      if (!ExpressionBuilder::operators.at(token).second) {
        offset = 1;
      }
      if (!operatorsStack.empty()) {
        char top = operatorsStack.top();
        while (isOperator(top)) {
          if (ExpressionBuilder::operators.at(token).first < ExpressionBuilder::operators.at(top).first + offset) {
            std::string stringTmp;
            stringTmp += top;
            postFixVector.push_back({ParsingTypes::OPERATOR, stringTmp});
            operatorsStack.pop();
          }
          else {
            break;
          }

          if (operatorsStack.empty()) {
            break;
          }

          top = operatorsStack.top();
        }
      }
      operatorsStack.push(token);
      iss >> tmp;
      last = token;
    }
    else if (token == ExpressionBuilder::parenthesis.first) {
      operatorsStack.push(token);
      functionsStack.push(token);
      iss >> tmp;
      last = token;
    }
    else if (token == ExpressionBuilder::parenthesis.second) {
      while (operatorsStack.top() != ExpressionBuilder::parenthesis.first) {
        std::string stringTmp;
        stringTmp += operatorsStack.top();
        postFixVector.push_back({ParsingTypes::OPERATOR, stringTmp});
        operatorsStack.pop();
      }
      operatorsStack.pop();
      iss >> tmp;
      last = token;

      if (!functionsStack.empty()) {
        functionsStack.pop();
      }

      if (!functionsStack.empty()) {
        std::string stringTmp;
        stringTmp += functionsStack.top();
        functionsStack.pop();
        if (stringTmp.front() != parenthesis.first) {
          postFixVector.push_back({ParsingTypes::OPERATOR, stringTmp});
        }
        else {
          functionsStack.push(parenthesis.first);
        }
      }
    }
    else {
      iss.get(tmp);
    }
  }

  while (!operatorsStack.empty()) {
    std::string stringTmp;
    stringTmp += operatorsStack.top();
    postFixVector.push_back({ParsingTypes::OPERATOR, stringTmp});
    operatorsStack.pop();
  }

  std::unique_ptr<implementation::Atom> lhs;
  std::unique_ptr<implementation::Atom> rhs;

  std::stack<std::unique_ptr<implementation::Atom>> treeNodes;

  for (const std::pair<ParsingTypes, std::string>& item : postFixVector) {
    if (item.first == ParsingTypes::OPERATOR) {
      if (item.second == "+") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        rhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Sum>(std::move(rhs), std::move(lhs)));
      }
      else if (item.second == "-") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        rhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Sum>(
            std::move(rhs),
            std::make_unique<implementation::Product>(std::make_unique<implementation::Number>(-1), std::move(lhs))));
      }
      else if (item.second == "m") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Product>(std::make_unique<implementation::Number>(-1), std::move(lhs)));
      }
      else if (item.second == "*") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        rhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Product>(std::move(rhs), std::move(lhs)));
      }
      else if (item.second == "/") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        rhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Product>(
            std::move(rhs),
            std::make_unique<implementation::Power>(std::move(lhs), std::make_unique<implementation::Number>(-1))));
      }
      else if (item.second == "^") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        rhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Power>(std::move(rhs), std::move(lhs)));
      }
      else if (item.second == "s") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Sin>(std::move(lhs)));
      }
      else if (item.second == "c") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Cos>(std::move(lhs)));
      }
      else if (item.second == "t") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Tan>(std::move(lhs)));
      }
      else if (item.second == "C") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Cot>(std::move(lhs)));
      }
      else if (item.second == "l") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        rhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Log>(std::move(rhs), std::move(lhs)));
      }
      else if (item.second == "L") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Log>(std::make_unique<implementation::Const>("e"), std::move(lhs)));
      }
      else if (item.second == "S") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Sqrt>(std::move(lhs)));
      }
      else if (item.second == "e") {
        lhs = std::move(treeNodes.top());
        treeNodes.pop();
        treeNodes.push(std::make_unique<implementation::Power>(std::make_unique<implementation::Const>("e"), std::move(lhs)));
      }
    }
    else if (item.first == ParsingTypes::NUMBER) {
      treeNodes.push(std::make_unique<implementation::Number>(std::stoi(item.second)));
    }
    else if (item.first == ParsingTypes::VARIABLE) {
      treeNodes.push(std::make_unique<implementation::Var>(item.second));
    }
    else if (item.first == ParsingTypes::CONSTANT) {
      treeNodes.push(std::make_unique<implementation::Const>(item.second));
    }
  }

  if (treeNodes.empty()) {
    return nullptr;
  }

  return std::move(treeNodes.top());
}
}  // namespace symbolicAlgebra