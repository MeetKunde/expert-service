#ifndef EXPERT_SERVICE_CIRCLE_MODEL_H
#define EXPERT_SERVICE_CIRCLE_MODEL_H

#include "../utilities/MathHelper.h"

#include <nlohmann/json.hpp>
#include <string>

typedef nlohmann::json json;

namespace expertBackground {
/**
 * @brief Class representing circle
 */
class CircleModel {
 public:
  CircleModel() = delete;
  explicit CircleModel(std::string identifier, std::string centerId, float radius, std::vector<std::string> includedPoints);

  CircleModel(const CircleModel& circleModel) = default;
  CircleModel(CircleModel&& circleModel) = default;

  CircleModel& operator=(const CircleModel& circleModel) = default;
  CircleModel& operator=(CircleModel&& circleModel) = default;

  ~CircleModel() = default;

  inline const std::string& getId() const { return id; }
  inline const std::string& getCenterId() const { return centerId; }
  inline float getRadius() const { return radius; }
  inline const std::vector<std::string>& getIncludedPoints() const { return includedPointIds; }

  json getJson() const;

  friend bool operator==(const CircleModel& circleModel1, const CircleModel& circleModel2);
  friend bool operator!=(const CircleModel& circleModel1, const CircleModel& circleModel2);
  friend bool operator<(const CircleModel& circleModel1, const CircleModel& circleModel2);
  friend bool operator>(const CircleModel& circleModel1, const CircleModel& circleModel2);
  friend bool operator<=(const CircleModel& circleModel1, const CircleModel& circleModel2);
  friend bool operator>=(const CircleModel& circleModel1, const CircleModel& circleModel2);

  friend std::ostream& operator<<(std::ostream& stream, const CircleModel& circleModel);
  friend json& operator<<(json& j, const CircleModel& circleModel);

 private:
  std::string id;
  std::string centerId;
  float radius;
  std::vector<std::string> includedPointIds;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_CIRCLE_MODEL_H
