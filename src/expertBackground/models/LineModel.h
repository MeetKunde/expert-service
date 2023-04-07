#ifndef EXPERT_SERVICE_LINE_MODEL_H
#define EXPERT_SERVICE_LINE_MODEL_H

#include "../utilities/MathHelper.h"
#include "IModel.h"

namespace expertBackground {
/**
 * @brief Class representing line
 */
class LineModel : public IModel {
 public:
  /**
   * @brief Representing type of line
   */
  enum class LineType{
    /**
     * @brief The equation is of the form x = b
     */
    VERTICAL,

    /**
     * @brief The equation is of the form y = b
     */
    HORIZONTAL,

    /**
     * @brief The equation is of the form y = ax + b
     */
    SLANTED
  };

 private:
  /**
   * @brief Line ID
   */
  unsigned int id;

  /**
   * @brief Type of line
   */
  LineType lineType;

  /**
   * @brief Coefficient of x in line equation: y = Ax + B
   */
  float lineA;

  /**
   * @brief Constant term in line equation: y = Ax + B or y = B or x = B
   */
  float lineB;

  /**
   * @brief IDs of included in line points
   */
  std::vector<unsigned int> includedPointIds;

 public:
  /**
   * @brief Constructor a new LineModel object
   *
   * @param identifier ID of line
   * @param type type of line
   * @param lineA A coefficient in line equation
   * @param lineB B coefficient in line equation
   * @param includedPoints IDs of points inluded in line
   */
  explicit LineModel(unsigned int identifier, LineType type, float lineA, float lineB, std::vector<unsigned int> includedPoints);

  /**
   * @brief Constructor of a new LineModel object
   *
   * @param lineModel other LineModel object
   */
  LineModel(const LineModel& lineModel);

  /**
   * @brief Override of the assignment operator
   *
   * @param lineModel object to be assigned
   * @return new LineModel object
   */
  LineModel& operator=(const LineModel& lineModel);

  /**
   * @brief Line ID getter
   *
   * @return line ID
   */
  inline unsigned int getId() const { return id; }

  /**
   * @brief Line type getter
   *
   * @return line type
   */
  inline LineType getLineType() const { return lineType; }

  /**
   * @brief A coefficient in line equation getter
   *
   * @return A coefficient in line equation
   */
  inline float getLineA() const { return lineA; }

  /**
   * @brief B coefficient in line equation getter
   *
   * @return B coefficient in line equation
   */
  inline float getLineB() const { return lineB; }

  /**
   * @brief Reference to set of included points IDs getter
   *
   * @return reference to set of included points IDs
   */
  inline const std::vector<unsigned int>& getIncludedPoints() const { return includedPointIds; }

  /**
   * @brief Override of equality operator
   *
   * @param lineModel1 first object to compare
   * @param lineModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator== (const LineModel& lineModel1, const LineModel& lineModel2);

  /**
   * @brief Override of inequality operator
   *
   * @param lineModel1 first object to compare
   * @param lineModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator!= (const LineModel& lineModel1, const LineModel& lineModel2);

  /**
   * @brief Override of less operator
   *
   * @param lineModel1 first object to compare
   * @param lineModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator< (const LineModel& lineModel1, const LineModel& lineModel2);

  /**
   * @brief Override of greater operator
   *
   * @param lineModel1 first object to compare
   * @param lineModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator> (const LineModel& lineModel1, const LineModel& lineModel2);

  /**
   * @brief Override of less or equal operator
   *
   * @param lineModel1 first object to compare
   * @param lineModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator<= (const LineModel& lineModel1, const LineModel& lineModel2);

  /**
   * @brief Override of greater or equal operator
   *
   * @param lineModel1 first object to compare
   * @param lineModel2 second object to compare
   * @return true if given angles are equal
   * @return false if given angles are not equal
   */
  friend bool operator>= (const LineModel& lineModel1, const LineModel& lineModel2);

  /**
   * @brief Override of stream insertion operator
   *
   * @param stream object of ostream class
   * @param lineModel object to insert to stream
   * @return object of ostream class
   */
  friend std::ostream& operator<< (std::ostream& stream, const LineModel& lineModel);

  json getJsonObject() const override;

  ~LineModel() override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_LINE_MODEL_H
