#include "Solve.h"
#include "Integer.h"
#include "Variable.h"

namespace symbolicAlgebra {
bool Solve::compare(const Expression& lhs, const Expression& rhs) {
  return lhs == rhs;
}

std::vector<Equation> Solve::findZeros(const Expression& equation, const std::string& variable) {
  Expression copyOfEquation = equation;
  copyOfEquation.simplify();

  Expression diff = equation;

  diff.differentiate(variable);
  diff.simplify();
  if (diff.canBeFullEvaluated() && diff.evaluate() == 0) {
    if (copyOfEquation.canBeFullEvaluated() && copyOfEquation.evaluate() == 0.0) {
      return {Equation(Variable(variable), Variable(variable))};
    }

    return {};
  }

  diff.differentiate(variable);
  diff.simplify();
  if (diff.canBeFullEvaluated() && diff.evaluate() == 0) {
    const Expression aCoeff = copyOfEquation.getCoefficient(Variable(variable));
    Expression bCoeff = copyOfEquation;
    bCoeff.substitute(variable, Expression("0"));
    bCoeff.simplify();

    const Expression left = Variable(variable);
    Expression right = Integer(-1) * bCoeff / aCoeff;
    right.simplify();

    return {Equation(left, right)};
  }

  diff.differentiate(variable);
  diff.simplify();
  if (diff.canBeFullEvaluated() && diff.evaluate() == 0) {
    const Expression aCoeff = copyOfEquation.getCoefficient(Variable(variable) ^ Integer(2));
    const Expression bCoeff = copyOfEquation.getCoefficient(Variable(variable));
    Expression cCoeff = copyOfEquation;
    cCoeff.substitute(variable, Expression("0"));
    cCoeff.simplify();

    const Expression left1 = Variable(variable);
    const Expression left2 = Variable(variable);
    const Expression delta = (bCoeff ^ Integer(2)) - (Integer(4) * aCoeff * cCoeff);
    const Expression half = Integer(1) / Integer(2);
    const Expression deltaSqrt = delta ^ half;
    Expression right1 = (Integer(-1) * bCoeff + deltaSqrt) / (Integer(2) * aCoeff);
    right1.simplify();
    Expression right2 = (Integer(-1) * bCoeff - deltaSqrt) / (Integer(2) * aCoeff);
    right2.simplify();

    return {Equation(left1, right1), Equation(left2, right2)};
  }

  return {};
}

std::vector<Equation> Solve::equation(const Equation& equation, const std::string& variable) {
  const Expression equ = equation.lhs - equation.rhs;
  return findZeros(equ, variable);
}

std::vector<Equation> Solve::systemOfEquations(const std::vector<Equation>& equations) {
  std::vector<Equation> result;
  std::stack<Equation> justUsedEquations;
  std::vector<Equation> notUsed;
  systemOfEquations(equations, justUsedEquations, notUsed, result);

  while (!justUsedEquations.empty()) {
    Equation equ = justUsedEquations.top();
    justUsedEquations.pop();

    for (auto iter = result.begin(); iter != result.end(); ++iter) {
      equ.rhs.substitute(iter->lhs.getString(), iter->rhs);
    }

    result.push_back(equ);
  }

  result.insert(result.end(), notUsed.begin(), notUsed.end());

  std::set<std::string> variables;
  std::vector<Equation> singles;
  for (auto iter = result.begin(); iter != result.end();) {
    variables.clear();
    iter->lhs.getIncludedVariables(variables);
    iter->rhs.getIncludedVariables(variables);

    if (variables.size() > 1) {
      for (auto var = variables.begin(); var != variables.end(); ++var) {
        std::vector<Equation> solutions = equation(*iter, *var);
        for (auto sol = solutions.begin(); sol != solutions.end(); ++sol) {
          singles.push_back(*sol);
        }
      }

      iter = result.erase(iter);
    }
    else {
      ++iter;
    }
  }

  result.insert(result.end(), singles.begin(), singles.end());

  return result;
}

void Solve::systemOfEquations(const std::vector<Equation>& equations, std::stack<Equation>& justUsedEquations,
                              std::vector<Equation>& notUsed, std::vector<Equation>& results) {
  bool somethingNew = false;

  std::vector<Equation> others;
  std::vector<Equation> news;

  for (auto iter = equations.cbegin(); iter != equations.cend(); ++iter) {
    std::set<std::string> variables = {};
    iter->lhs.getIncludedVariables(variables);
    iter->rhs.getIncludedVariables(variables);

    if (variables.size() == 1) {
      std::vector<Equation> res = equation(*iter, *variables.begin());
      results.push_back(res.front());
      news.push_back(res.front());
      somethingNew = true;
    }
    else if (!variables.empty()) {
      others.push_back(*iter);
    }
  }

  if (somethingNew) {
    for (auto iter1 = others.begin(); iter1 != others.end(); ++iter1) {
      for (auto iter2 = news.begin(); iter2 != news.end(); ++iter2) {
        Expression newLhs = iter1->lhs;
        Expression newRhs = iter1->rhs;
        newLhs.substitute(iter2->lhs.getString(), iter2->rhs);
        newRhs.substitute(iter2->lhs.getString(), iter2->rhs);
        *iter1 = Equation(newLhs, newRhs);
      }
    }

    systemOfEquations(others, justUsedEquations, notUsed, results);
  }
  else {
    notUsed.clear();
    for (auto iter = others.begin(); iter != others.end(); ++iter) {
      notUsed.push_back(*iter);
    }

    for (auto iter1 = others.cbegin(); iter1 != others.cend(); ++iter1) {
      std::set<std::string> vars1;
      iter1->lhs.getIncludedVariables(vars1);
      iter1->rhs.getIncludedVariables(vars1);

      std::set<std::string> vars2;
      for (auto iter2 = others.cbegin(); iter2 != others.cend(); ++iter2) {
        //if(iter1 != iter2) {
        iter2->lhs.getIncludedVariables(vars2);
        iter2->rhs.getIncludedVariables(vars2);
        //}
      }

      for (auto iter2 = vars1.cbegin(); iter2 != vars1.cend(); ++iter2) {
        if (vars2.find(*iter2) != vars2.cend()) {
          std::vector<Equation> solution = equation(*iter1, *iter2);
          for (auto iter3 = solution.begin(); iter3 != solution.end(); ++iter3) {
            std::vector<Equation> newEquations;
            justUsedEquations.push(*iter3);
            for (auto iter4 = others.cbegin(); iter4 != others.cend(); ++iter4) {
              if (iter1 != iter4) {
                Expression newLhs = iter4->lhs;
                newLhs.substitute(iter3->lhs.getString(), iter3->rhs);
                Expression newRhs = iter4->rhs;
                newRhs.substitute(iter3->lhs.getString(), iter3->rhs);
                newEquations.emplace_back(newLhs, newRhs);
              }
            }

            systemOfEquations(newEquations, justUsedEquations, notUsed, results);
            return;
          }
        }
      }
    }
  }
}

}  // namespace symbolicAlgebra
