#ifndef EXPERT_SERVICE_LINE_MODEL_H
#define EXPERT_SERVICE_LINE_MODEL_H

#include "../utilities/MathHelper.h"

#include <nlohmann/json.hpp>
#include <string>

typedef nlohmann::json json;

namespace expertBackground {
/**
 * @brief Class representing line
 */
class LineModel {
 public:
 /**
  * @brief Enum representing type of line
 */
  enum class Type {
    VERTICAL,
    HORIZONTAL,
    SLANTED
  };

  LineModel() = delete;
  explicit LineModel(std::string identifier, Type type, float coeffA, float coeffB, std::vector<std::string> includedPoints);

  LineModel(const LineModel& lineModel) = default;
  LineModel(LineModel&& lineModel) = default;

  LineModel& operator=(const LineModel& lineModel) = default;
  LineModel& operator=(LineModel&& lineModel) = default;

  ~LineModel() = default;

  inline std::string getId() const { return id; }
  inline Type getType() const { return type; }
  inline float getCoeffA() const { return coeffA; }
  inline float getCoeffB() const { return coeffB; }
  inline const std::vector<std::string>& getIncludedPoints() const { return includedPointIds; }

  json getJson() const;

  friend bool operator== (const LineModel& lineModel1, const LineModel& lineModel2);
  friend bool operator!= (const LineModel& lineModel1, const LineModel& lineModel2);
  friend bool operator< (const LineModel& lineModel1, const LineModel& lineModel2);
  friend bool operator> (const LineModel& lineModel1, const LineModel& lineModel2);
  friend bool operator<= (const LineModel& lineModel1, const LineModel& lineModel2);
  friend bool operator>= (const LineModel& lineModel1, const LineModel& lineModel2);

  friend std::ostream& operator<< (std::ostream& stream, const LineModel& lineModel);
  friend json& operator<< (json& j, const LineModel& lineModel);

 private:
  std::string id;
  Type type;
  float coeffA; // Coefficient of x in line equation: y = Ax + B
  float coeffB; // Constant term in line equation: y = Ax + B or y = B or x = B
  std::vector<std::string> includedPointIds;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_LINE_MODEL_H
