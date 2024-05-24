#include "Atom.h"

#include "operands/Number.h"
#include "operands/Var.h"
#include "operands/Const.h"
#include "operators/Sum.h"
#include "operators/Product.h"
#include "operators/Power.h"
#include "functions/Sin.h"
#include "functions/Cos.h"
#include "functions/Tan.h"
#include "functions/Cot.h"
#include "functions/Sqrt.h"
#include "functions/Log.h"

#include <math.h>
#include <vector>
#include <set>
#include <map>
#include <numeric>

#ifndef UTILITIES
#define UTILITIES

#define FORWARD_DECLARATION
#include "Utilities.h"
#undef FORWARD_DECLARATION

#define DECLARATION
#include "Utilities.h"
#undef DECLARATION

#define DEFINITION
#include "Utilities.h"

namespace symbolicAlgebra::implementation {
class Utilities {
 public:
  static constexpr double EPSILON = 0.00001;

  Utilities() = delete;

  inline static bool isInt(double val) { return fabs(val - static_cast<int>(val)) < Utilities::EPSILON; }
  inline static bool isZero(double val) { return fabs(val) < Utilities::EPSILON; }
  inline static bool isOne(double val) { return fabs(val - 1.0) < Utilities::EPSILON; }
  inline static bool areEqual(double val1, double val2) { return fabs(val1 - val2) < Utilities::EPSILON; }
  static std::pair<int, int> addFractions(const std::vector<std::pair<int, int>>& fractions);
  static std::pair<int, int> mulFractions(const std::vector<std::pair<int, int>>& fractions);
};

}  // namespace symbolicAlgebra::implementation

#endif  // UTILITIES