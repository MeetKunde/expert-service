#ifndef EXPERT_SERVICE_POINT_MODEL_H
#define EXPERT_SERVICE_POINT_MODEL_H

#include "../utilities/MathHelper.h"
#include "IModel.h"

namespace expertBackground {
/**
 * @brief Class representing point
 */
class PointModel : public IModel {
 private:
  /**
   * @brief Point ID
   */
  std::string id;

  /**
   * @brief Point X coordinate
   */
  float x;

  /**
   * @brief Point Y coordinate
   */
  float y;

  /**
   * @brief Point name
   */
   std::string name;

 public:
  /**
   * @brief Constructor of a new PointModel object
   *
   * @param identifier ID of point
   * @param xCoordinate X coordinate of point
   * @param yCoordinate Y coordinate of point
   * @param name point name
   */
  explicit PointModel(std::string identifier, float xCoordinate, float yCoordinate, std::string name);

  /**
   * @brief Constructor of a new PointModel object
   *
   * @param pointModel other PointModel object
   */
  PointModel(const PointModel& pointModel);

  /**
   * @brief Override of the assignment operator
   *
   * @param pointModel object to be assigned
   * @return new PointModel object
   */
  PointModel& operator=(const PointModel& pointModel);

  /**
   * @brief Point ID getter
   *
   * @return point ID
   */
  inline const std::string& getId() const { return id; }

  /**
   * @brief Point X coordinate getter
   *
   * @return point X coordinate
   */
  inline float getX() const { return x; }

  /**
   * @brief Point Y coordinate getter
   *
   * @return point Y coordinate
   */
  inline float getY() const { return y; }

  /**
   * @brief Point name getter
   *
   * @return point name
   */
  inline const std::string& getName() const { return name; }

  inline MathHelper::Coordinates getCoordinates() const { return {x, y}; }

  /**
   * @brief Override of equality operator
   *
   * @param pointModel1 first object to compare
   * @param pointModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator==(const PointModel& pointModel1, const PointModel& pointModel2);

  /**
   * @brief Override of inequality operator
   *
   * @param pointModel1 first object to compare
   * @param pointModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator!=(const PointModel& pointModel1, const PointModel& pointModel2);

  /**
   * @brief Override of less operator
   *
   * @param pointModel1 first object to compare
   * @param pointModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<(const PointModel& pointModel1, const PointModel& pointModel2);

  /**
   * @brief Override of greater operator
   *
   * @param pointModel1 first object to compare
   * @param pointModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>(const PointModel& pointModel1, const PointModel& pointModel2);

  /**
   * @brief Override of less or equal operator
   *
   * @param pointModel1 first object to compare
   * @param pointModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<=(const PointModel& pointModel1, const PointModel& pointModel2);

  /**
   * @brief Override of greater or equal operator
   *
   * @param pointModel1 first object to compare
   * @param pointModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>=(const PointModel& pointModel1, const PointModel& pointModel2);

  /**
   * @brief Override of stream insertion operator
   *
   * @param stream object of ostream class
   * @param pointModel object to insert to stream
   * @return object of ostream class
   */
  friend std::ostream& operator<<(std::ostream& stream, const PointModel& pointModel);

  json getJsonObject() const override;

  ~PointModel() override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_POINT_MODEL_H
