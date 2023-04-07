#ifndef EXPERT_SERVICE_MATH_HELPER_H
#define EXPERT_SERVICE_MATH_HELPER_H

#include <math.h>

namespace expertBackground {

class MathHelper {
 public:
  struct Coordinates {
    float x;
    float y;
  };

  static const float COMPARISON_EPSILON;
  static const int ROUND_ANGLE_VALUE;
  static const int STRAIGHT_ANGLE_VALUE;
  static const int RIGHT_ANGLE_VALUE;
  static const int ZERO_ANGLE_VALUE;
  static const int EQUILATERAL_TRIANGLE_INTERNAL_ANGLE_VALUE;
  static const int SQUARE_INTERNAL_ANGLE_VALUE;
  static const int RECTANGLE_INTERNAL_ANGLE_VALUE;

  static double getDistance(Coordinates point1, Coordinates point2);
  static double getAngleCosine(Coordinates point1, Coordinates point2, Coordinates point3);
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_MATH_HELPER_H
