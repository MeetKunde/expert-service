#ifndef EXPERT_SERVICE_POINTS_PAIR_MODEL_H
#define EXPERT_SERVICE_POINTS_PAIR_MODEL_H

#include "IModel.h"

namespace expertBackground {
/**
* @brief Class representing ordered pair of usigned integers
*/
class PointsPairModel : public IModel {
 private:
  /**
   * @brief Value of first element
   */
  std::string point1Id;

  /**
   * @brief Value of second element
   */
  std::string point2Id;

 public:
  /**
   * @brief Constructor of a new PointsPairModel object
   *
   * @param point1Id first point ID
   * @param point2Id second point ID
   */
  explicit PointsPairModel(std::string point1, std::string point2);

  /**
   * @brief Constructor of a new PointsPairModel object
   *
   * @param pointsPair other PointsPairModel object
   */
  PointsPairModel(const PointsPairModel& pointsPair);

  /**
   * @brief Override of the assignment operator
   *
   * @param pointsPair object to be assigned
   * @return new PointsPairModel object
   */
  PointsPairModel& operator=(const PointsPairModel& pointsPair);

  /**
   * @brief First point ID getter
   *
   * @return first point ID
   */
  inline const std::string& getPoint1Id() const { return point1Id; };

  /**
   * @brief Second point ID getter
   *
   * @return second point ID
   */
  inline const std::string& getPoint2Id() const { return point2Id; };

  /**
   * @brief Override of equality operator
   *
   * @param pointsPair1 first object to compare
   * @param pointsPair2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator== (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  /**
   * @brief Override of inequality operator
   *
   * @param pointsPair1 first object to compare
   * @param pointsPair2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator!= (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  /**
   * @brief Override of less operator
   *
   * @param pointsPair1 first object to compare
   * @param pointsPair2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator< (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  /**
   * @brief Override of greater operator
   *
   * @param pointsPair1 first object to compare
   * @param pointsPair2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator> (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  /**
   * @brief Override of less or equal operator
   *
   * @param pointsPair1 first object to compare
   * @param pointsPair2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<= (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  /**
   * @brief Override of greater or equal operator
   *
   * @param pointsPair1 first object to compare
   * @param pointsPair2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>= (const PointsPairModel& pointsPair1, const PointsPairModel& pointsPair2);

  /**
   * @brief Override of stream insertion operator
   *
   * @param stream object of ostream class
   * @param pointsPair object to insert to stream
   * @return object of ostream class
   */
  friend std::ostream& operator<< (std::ostream& stream, const PointsPairModel& pointsPair);

  json getJsonObject() const override;

  ~PointsPairModel() override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_POINTS_PAIR_MODEL_H
