#ifndef EXPERT_SERVICE_POINT_MODEL_H
#define EXPERT_SERVICE_POINT_MODEL_H

#include "../utilities/MathHelper.h"

#include <nlohmann/json.hpp>
#include <string>

typedef nlohmann::json json;

namespace expertBackground {
/**
 * @brief Class representing point
 */
class PointModel {
 public:
  PointModel() = delete;
  explicit PointModel(std::string identifier, float xCoordinate, float yCoordinate, std::string name);

  PointModel(const PointModel&) = default;
  PointModel(PointModel&&) = default;

  PointModel& operator=(const PointModel&) = default;
  PointModel& operator=(PointModel&&) = default;

  ~PointModel() = default;

  inline const std::string& getId() const { return id; }
  inline float getX() const { return x; }
  inline float getY() const { return y; }
  inline const std::string& getName() const { return name; }
  inline MathHelper::Coordinates getCoordinates() const { return {x, y}; }

  json getJson() const;

  friend bool operator==(const PointModel& pointModel1, const PointModel& pointModel2);
  friend bool operator!=(const PointModel& pointModel1, const PointModel& pointModel2);
  friend bool operator<(const PointModel& pointModel1, const PointModel& pointModel2);
  friend bool operator>(const PointModel& pointModel1, const PointModel& pointModel2);
  friend bool operator<=(const PointModel& pointModel1, const PointModel& pointModel2);
  friend bool operator>=(const PointModel& pointModel1, const PointModel& pointModel2);

  friend std::ostream& operator<<(std::ostream& stream, const PointModel& pointModel);
  friend json& operator<<(json& j, const PointModel& pointModel); 

 private:
  std::string id;
  float x;
  float y;
  std::string name;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_POINT_MODEL_H
