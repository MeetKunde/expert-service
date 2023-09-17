#ifndef EXPERT_SERVICE_POLYGON_MODEL_H
#define EXPERT_SERVICE_POLYGON_MODEL_H

#include <vector>
#include "IModel.h"

namespace expertBackground {

class PolygonModel : public IModel {
 public:
  enum class PolygonType {
    UNKNOWN,
    ISOSCELES_ACUTE_TRIANGLE,
    EQUILATERAL_TRIANGLE,
    SCALENE_RIGHT_TRIANGLE,
    ISOSCELES_RIGHT_TRIANGLE,
    OBTUSE_ISOSCELES_TRIANGLE,
    SQUARE,
    RECTANGLE,
    REGULAR_POLYGON,
    PARALLELOGRAM,
    KITE,
    RHOMBUS,
    SCALENE_TRAPEZOID,
    ISOSCELES_TRAPEZOID,
    RIGHT_TRAPEZOID
  };

 private:
  std::vector<std::string> verticesIds;

  bool fixedPointsOrder;

 public:
  explicit PolygonModel(std::vector<std::string> vertices, bool fixedPointsOrder);
  PolygonModel(const PolygonModel& polygonModel);

  PolygonModel& operator=(const PolygonModel& polygonModel);

  const std::vector<std::string>& getVerticesIds() const { return verticesIds; }

  friend bool operator==(const PolygonModel& lhs, const PolygonModel& rhs);
  friend bool operator!=(const PolygonModel& lhs, const PolygonModel& rhs);

  friend std::ostream& operator<<(std::ostream& stream, const PolygonModel& polygonModel);

  json getJsonObject() const override;
  ~PolygonModel() override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_POLYGON_MODEL_H
