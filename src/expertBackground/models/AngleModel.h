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
  explicit AngleModel(std::string arm1Point1Id, std::string arm1Point2Id, std::string arm2Point1Id, std::string arm2Point2Id, Type type);

  AngleModel(const AngleModel& angleModel) = default;
  AngleModel(AngleModel&& angleModel) = default;

  AngleModel& operator=(const AngleModel& angleModel) = default;
  AngleModel& operator=(AngleModel&& angleModel) = default;

  ~AngleModel()  = default;

  inline const PointsPairModel& getArm1() const { return arm1; }
  inline const PointsPairModel& getArm2() const { return arm2; }

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
  PointsPairModel arm1;
  PointsPairModel arm2;
  Type type;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_ANGLE_MODEL_H
