#ifndef EXPERT_SERVICE_CIRCLE_MODEL_H
#define EXPERT_SERVICE_CIRCLE_MODEL_H

#include "../utilities/MathHelper.h"
#include "IModel.h"
#include "PointModel.h"

namespace expertBackground {
/**
 * @brief Class representing circle
 */
class CircleModel : public IModel {
 private:
  /**
   * @brief Circle ID getter
   */
  std::string id;

  /**
   * @brief Circle center point
   */
  PointModel centerPoint;

  /**
   * @brief Circle radius length
   */
  float radius;

  /**
   * @brief IDs of included in circle points
   */
  std::vector<std::string> includedPointIds;

 public:
  /**
   * @brief Constructor of a new CircleModel object
   *
   * @param identifier ID of circle
   * @param centerId ID of circle center point
   * @param centerX X coordinate of circle center point
   * @param centerY Y coordinate of circle center point
   * @param radius radius of circle
   * @param includedPoints IDs of points inluded in circle
   */
  explicit CircleModel(std::string identifier, std::string centerId, float centerX, float centerY, std::string centerName, float radius,
                       std::vector<std::string> includedPoints);

  /**
   * @brief Constructor of a new CircleModel object
   *
   * @param circleModel other CircleModel object
   */
  CircleModel(const CircleModel& circleModel);

  /**
   * @brief Override of the assignment operator
   *
   * @param circleModel object to be assigned
   * @return new CircleModel object
   */
  CircleModel& operator=(const CircleModel& circleModel);

  /**
   * @brief Circle ID getter
   *
   * @return circle ID
   */
  inline const std::string& getId() const { return id; }

  /**
   * @brief Circle center ID getter
   *
   * @return circle center ID
   */
  inline const std::string& getCenterId() const { return centerPoint.getId(); }

  /**
   * @brief Circle center X coordinate getter
   *
   * @return circle center X coordinate
   */
  inline float getCenterX() const { return centerPoint.getX(); }

  /**
   * @brief Circle center Y coordinate getter
   *
   * @return circle center Y coordinate
   */
  inline float getCenterY() const { return centerPoint.getY(); }

  /**
   * @brief Circle center name getter
   *
   * @return circle center name
   */
  inline const std::string& getCenterName() const { return centerPoint.getName(); }

  /**
   * @brief Circle radius length getter
   *
   * @return circle radius length
   */
  inline float getRadius() const { return radius; }

  /**
   * @brief Reference to set of included points IDs getter
   *
   * @return reference to set of included points IDs
   */
  inline const std::vector<std::string>& getIncludedPoints() const { return includedPointIds; }

  /**
   * @brief Override of equality operator
   *
   * @param circleModel1 first object to compare
   * @param circleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator==(const CircleModel& circleModel1, const CircleModel& circleModel2);

  /**
   * @brief Override of inequality operator
   *
   * @param circleModel1 first object to compare
   * @param circleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator!=(const CircleModel& circleModel1, const CircleModel& circleModel2);

  /**
   * @brief Override of less operator
   *
   * @param circleModel1 first object to compare
   * @param circleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<(const CircleModel& circleModel1, const CircleModel& circleModel2);

  /**
   * @brief Override of greater operator
   *
   * @param circleModel1 first object to compare
   * @param circleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>(const CircleModel& circleModel1, const CircleModel& circleModel2);

  /**
   * @brief Override of less or equal operator
   *
   * @param circleModel1 first object to compare
   * @param circleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<=(const CircleModel& circleModel1, const CircleModel& circleModel2);

  /**
   * @brief Override of greater or equal operator
   *
   * @param circleModel1 first object to compare
   * @param circleModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>=(const CircleModel& circleModel1, const CircleModel& circleModel2);

  /**
   * @brief Override of stream insertion operator
   *
   * @param stream object of ostream class
   * @param circleModel object to insert to stream
   * @return object of ostream class
   */
  friend std::ostream& operator<<(std::ostream& stream, const CircleModel& circleModel);

  json getJsonObject() const override;

  ~CircleModel() override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_CIRCLE_MODEL_H
