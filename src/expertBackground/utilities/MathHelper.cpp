#include "MathHelper.h"

namespace expertBackground {
const float MathHelper::COMPARISON_EPSILON = 0.001F;
const int MathHelper::ROUND_ANGLE_VALUE = 360;
const int MathHelper::STRAIGHT_ANGLE_VALUE = 180;
const int MathHelper::RIGHT_ANGLE_VALUE = 90;
const int MathHelper::ZERO_ANGLE_VALUE = 0;
const int MathHelper::EQUILATERAL_TRIANGLE_INTERNAL_ANGLE_VALUE = 60;
const int MathHelper::SQUARE_INTERNAL_ANGLE_VALUE = 90;
const int MathHelper::RECTANGLE_INTERNAL_ANGLE_VALUE = 90;

double MathHelper::getDistance(Coordinates point1, Coordinates point2) {
  const double diffX = point1.x - point2.x;
  const double diffY = point1.y - point2.y;

  return sqrt(diffX * diffX + diffY * diffY);
}

double MathHelper::getAngleCosine(Coordinates point1, Coordinates point2, Coordinates point3) {
  // law of Cosines: c^2 = a^2 + b^2 - 2ab*cos(angle)
  // cos(angle) = (a^2 + b^2 - c^2) / 2ab

  const double side1 = MathHelper::getDistance(point1, point2);
  const double side2 = MathHelper::getDistance(point3, point2);
  const double side3 = MathHelper::getDistance(point1, point3);

  constexpr float multiplier = 2.0F;

  return (side1 * side1 + side2 * side2 - side3 * side3) / (multiplier * side1 * side2);
}
}  // namespace expertBackground