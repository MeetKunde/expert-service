#ifndef INTEGER_H
#define INTEGER_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Integer : public Expression {
 public:
  Integer() = delete;
  explicit Integer(int value);

  Integer(const Integer& other) = default;
  Integer(Integer&& other) = default;

  Integer& operator=(const Integer& other) = default;
  Integer& operator=(Integer&& other) = default;

  ~Integer() = default;
};
}  // namespace symbolicAlgebra

#endif /* INTEGER_H */