#ifndef EXPERT_SERVICE_ANGLE_MODEL_H
#define EXPERT_SERVICE_ANGLE_MODEL_H

#include "IModel.h"
#include "PointsPairModel.h"

namespace expertBackground {
/**
* @brief Class representing angle as triple of point IDs
*/
class AngleModel : public IModel {
 public:
  /**
   * @brief Representing type of angle
   */
  enum class AngleType {
    /**
     * @brief Angle type is unknown - convex or concave
     */
    UNKNOWN,

    /**
     * @brief Angle is convex
     */
    CONVEX,

    /**
     * @brief Angle is concave
     */
    CONCAVE
  };

 private:
  /**
   * @brief Angle vertex ID
   */
  unsigned int vertexId;

  /**
   * @brief IDs of points which lies on ends of angle arms
   */
  PointsPairModel pointsOnArms;

  /**
   * @brief Type of angle
   */
  AngleType angleType;

 public:
  /**
   * @brief Constructor of a new AngleModel object
   *
   * @param point1Id ID of point on first angle arm
   * @param vertexId ID of angle vertex
   * @param point2Id ID of point on second angle arm
   * @param type type of angle
   */
  explicit AngleModel(unsigned int vertexId, unsigned int point1Id, unsigned int point2Id, AngleType type);

  /**
   * @brief Constructor of a new AngleModel object
   *
   * @param angleModel other AngleModel object
   */
  AngleModel(const AngleModel& angleModel);

  /**
   * @brief Override of the assignment operator
   *
   * @param angleModel object to be assigned
   * @return new AngleModel object
   */
  AngleModel& operator=(const AngleModel& angleModel);

  /**
   * @brief ID of point on first arm getter
   *
   * @return ID of point on first arm
   */
  inline unsigned int getPoint1Id() const { return pointsOnArms.getPoint1Id(); }

  /**
   * @brief ID of point on second arm getter
   *
   * @return ID of point on second arm
   */
  inline unsigned int getPoint2Id() const { return pointsOnArms.getPoint2Id(); }

  /**
   * @brief ID of angle vertex getter
   *
   * @return ID of angle vertex
   */
  inline unsigned int getVertexId() const { return vertexId; }

  /**
   * @brief Angle type getter
   *
   * @return angle type
   */
  inline AngleType getAngleType() const { return angleType; }

  /**
   * @brief Override of equality operator
   *
   * @param angleModel1 first object to compare
   * @param angleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator==(const AngleModel& angleModel1, const AngleModel& angleModel2);

  /**
   * @brief Override of inequality operator
   *
   * @param angleModel1 first object to compare
   * @param angleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator!=(const AngleModel& angleModel1, const AngleModel& angleModel2);

  /**
   * @brief Override of less operator
   *
   * @param angleModel1 first object to compare
   * @param angleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<(const AngleModel& angleModel1, const AngleModel& angleModel2);

  /**
   * @brief Override of greater operator
   *
   * @param angleModel1 first object to compare
   * @param angleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>(const AngleModel& angleModel1, const AngleModel& angleModel2);

  /**
   * @brief Override of less or equal operator
   *
   * @param angleModel1 first object to compare
   * @param angleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<=(const AngleModel& angleModel1, const AngleModel& angleModel2);

  /**
   * @brief Override of greater or equal operator
   *
   * @param angleModel1 first object to compare
   * @param angleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>=(const AngleModel& angleModel1, const AngleModel& angleModel2);

  /**
   * @brief Override of stream insertion operator
   *
   * @param stream object of ostream class
   * @param angleModel object to insert to stream
   * @return object of ostream class
   */
  friend std::ostream& operator<<(std::ostream& stream, const AngleModel& angleModel);

  json getJsonObject() const override;

  ~AngleModel() override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_ANGLE_MODEL_H
