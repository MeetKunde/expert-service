#ifndef EXPERT_SERVICE_POLYGON_MODEL_H
#define EXPERT_SERVICE_POLYGON_MODEL_H

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

typedef nlohmann::json json;

namespace expertBackground {
/**
 * @brief Class representing polygon model
*/
class PolygonModel {
 public:
  /**
   * @brief Enum representing polygon type
  */
  enum class Type {
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

  PolygonModel() = delete;
  explicit PolygonModel(Type type, bool fixedPointsOrder, std::vector<std::string> vertices);
  
  PolygonModel(const PolygonModel& polygonModel) = default;
  PolygonModel(PolygonModel&& polygonModel) = default;

  PolygonModel& operator=(const PolygonModel& polygonModel) = default;
  PolygonModel& operator=(PolygonModel&& polygonModel) = default;

  ~PolygonModel()  = default;

  inline Type getType() const { return type; }
  inline bool getFixedPointsOrder() const { return fixedPointsOrder; }
  inline const std::vector<std::string>& getVerticesIds() const { return verticesIds; }

  json getJson() const ;

  friend bool operator==(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2);
  friend bool operator!=(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2);
  friend bool operator<(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2);
  friend bool operator>(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2);
  friend bool operator<=(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2);
  friend bool operator>=(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2);

  friend std::ostream& operator<<(std::ostream& stream, const PolygonModel& polygonModel);
  friend json& operator<<(json& j, const PolygonModel& polygonModel);
 
 private:
  Type type;
  bool fixedPointsOrder;
  std::vector<std::string> verticesIds;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_POLYGON_MODEL_H
