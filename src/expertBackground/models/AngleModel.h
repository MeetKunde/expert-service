#ifndef EXPERT_SERVICE_ANGLE_MODEL_H
#define EXPERT_SERVICE_ANGLE_MODEL_H

#include "PointsPairModel.h"

#include <nlohmann/json.hpp>
#include <optional>

typedef nlohmann::json json;

namespace expertBackground {
/**
* @brief Class representing angle as triple of point IDs
*/
class AngleModel {
 public:
  /**
   * @brief Enum representing angle type
   */
  enum class Type {
    UNKNOWN,
    CONVEX,
    CONCAVE
  };

  AngleModel() = delete;
  explicit AngleModel(std::string vertexId, std::string point1Id, std::string point2Id, Type type);
  AngleModel(std::string vertexId, std::string point1Id, std::string point2Id, std::string arm1, std::string arm2, Type type);

  AngleModel(const AngleModel& angleModel) = default;
  AngleModel(AngleModel&& angleModel) = default;

  AngleModel& operator=(const AngleModel& angleModel) = default;
  AngleModel& operator=(AngleModel&& angleModel) = default;

  ~AngleModel()  = default;

  inline const std::string& getPoint1Id() const { return pointsOnArms.getPoint1Id(); }
  inline const std::string& getPoint2Id() const { return pointsOnArms.getPoint2Id(); }
  inline const std::string& getVertexId() const { return vertexId; }
  inline const std::optional<std::string>& getArm1() const { return arm1; }
  inline const std::optional<std::string>& getArm2() const { return arm2; }

  inline bool hasTwoArms() const { return arm1.has_value() && arm2.has_value(); }
  inline bool hasOneArm(const std::string& armId) const { return arm1 == armId || arm2 == armId; }
  
  inline Type getType() const { return type; }

  json getJson() const ;

  friend bool operator==(const AngleModel& angleModel1, const AngleModel& angleModel2);
  friend bool operator!=(const AngleModel& angleModel1, const AngleModel& angleModel2);
  friend bool operator<(const AngleModel& angleModel1, const AngleModel& angleModel2);
  friend bool operator>(const AngleModel& angleModel1, const AngleModel& angleModel2);
  friend bool operator<=(const AngleModel& angleModel1, const AngleModel& angleModel2);
  friend bool operator>=(const AngleModel& angleModel1, const AngleModel& angleModel2);

  friend std::ostream& operator<<(std::ostream& stream, const AngleModel& angleModel);
  friend json& operator<<(json& j, const AngleModel& angleModel);

 private:
  std::string vertexId;
  PointsPairModel pointsOnArms;
  std::optional<std::string> arm1;
  std::optional<std::string> arm2;
  Type type;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_ANGLE_MODEL_H
