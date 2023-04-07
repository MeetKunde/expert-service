#ifndef INTEGER_H
#define INTEGER_H

#include "Expression.h"
#include "implementation/Utilities.h"

namespace symbolicAlgebra {
class Integer : public Expression {
 public:
  explicit Integer(int value);
};
}  // namespace symbolicAlgebra

#endif /* INTEGER_H */