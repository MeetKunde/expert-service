#ifndef EXPERT_SERVICE_POINTS_PAIR_MODEL_H
#define EXPERT_SERVICE_POINTS_PAIR_MODEL_H

#include <nlohmann/json.hpp>
#include <string>

typedef nlohmann::json json;

namespace expertBackground {
/**
* @brief Class representing pair of points
*/
class PointsPairModel {
 public:
  PointsPairModel() = delete;
  explicit PointsPairModel(std::string point1, std::string point2);

  PointsPairModel(const PointsPairModel& pointsPair) = default;
  PointsPairModel(PointsPairModel&& pointsPair) = default;

  PointsPairModel& operator=(const PointsPairModel& pointsPair) = default;
  PointsPairModel& operator=(PointsPairModel&& pointsPair) = default;

  ~PointsPairModel() = default;

  inline const std::string& getPoint1Id() const { return point1Id; };
  inline const std::string& getPoint2Id() const { return point2Id; };

  json getJson() const ;

  friend bool operator== (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);
  friend bool operator!= (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);
  friend bool operator< (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);
  friend bool operator> (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);
  friend bool operator<= (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);
  friend bool operator>= (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  friend std::ostream& operator<< (std::ostream& stream, const PointsPairModel& pointsPair);
  friend json& operator<< (json& j, const PointsPairModel& pointsPair);

 private:
  std::string point1Id;
  std::string point2Id;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_POINTS_PAIR_MODEL_H
