#ifndef EXPERT_SERVICE_SHAPES_BANK_H
#define EXPERT_SERVICE_SHAPES_BANK_H

#include "models/PointModel.h"
#include "models/LineModel.h"
#include "models/CircleModel.h"

namespace expertBackground {
/**
 * @brief Class storing all shapes
 */
class ShapesBank {
 private:
  /**
   * @brief Point IDs counter
   */
  unsigned int pointIdCounter;

  /**
   * @brief Line IDs counter
   */
  unsigned int lineIdCounter;

  /**
   * @brief Circle IDs counter
   */
  unsigned int circleIdCounter;

  /**
   * @brief Vector with all point models
   *
   * Index in vector is equal to object ID
   */
  std::vector<PointModel> points;

  /**
   * @brief Vector with all line models
   *
   * Index in vector is equal to object ID
   */
  std::vector<LineModel> lines;

  /**
   * @brief Vector with all circle models
   *
   * Index in vector is equal to object ID
   */
  std::vector<CircleModel> circles;

  /**
   * @brief Vector which converts ID of points given by user to ID set automatically in ShapesBank
   */
  std::vector<unsigned int> pointIdsConverter;

  /**
   * @brief Vector which converts ID of lines given by user to ID set automatically in ShapesBank
   */
  std::vector<unsigned int> lineIdsConverter;

  /**
   * @brief Vector which converts ID of circles given by user to ID set automatically in ShapesBank
   */
  std::vector<unsigned int> circleIdsConverter;

 public:
  /**
   * @brief Constructor of a new ShapesBank object
   */
  explicit ShapesBank();

  /**
   * @brief Constructor of a new ShapesBank object
   *
   * @param maxPointIdCounter maximal value of gived point ID
   * @param maxLineIdCounter maximal value of gived line ID
   * @param maxCircleIdCounter maximal value of gived circle ID
   */
  explicit ShapesBank(unsigned int maxPointIdCounter, unsigned int maxLineIdCounter, unsigned int maxCircleIdCounter);

  /**
   * @brief Adding new PointModel to bank
   *
   * @param id point ID
   * @param x point X coordinate
   * @param y point Y coordinate
   */
  void addPoint(unsigned int identifier, float xCoordinate, float yCoordinate);

  /**
   * @brief PointModel object getter
   *
   * @param identifier ID of point
   * @return reference to PointModel
   * @throws std::out_of_range if point with given ID does not exist
   */
  inline const PointModel& getPoint(unsigned int identifier) const { return points.at(identifier); }

  /**
   * @brief Number of points getter
   *
   * @return number of points
   */
  inline unsigned int getPointsNumber() const { return points.size(); }

  /**
   * @brief All point objects getter
   *
   * @return reference to vector with all points
   */
  inline const std::vector<PointModel>& getPointsVector() const { return points; }

  /**
   * @brief Getting JSON object representing all PointModel objects
   *
   * @return JSON object representing all PointModel objects
   */
  json getPointsAsJsonObjects();

  /**
   * @brief Adding new LineModel to bank
   *
   * @param id line ID
   * @param lineType line type
   * @param lineA coefficient of x in line equation y = ax + b
   * @param lineB constant term in line equation y = ax + b or y = b or x = b
   * @param includedPoints IDs of points inluded in line
   */
  void addLine(unsigned int identifier, LineModel::LineType lineType, float lineA, float lineB,
               std::vector<unsigned int> includedPoints);

  /**
   * @brief LineModel object getter
   *
   * @param id ID of line
   * @return reference to LineModel
   * @throws std::out_of_range if line with given ID does not exist
   */
  inline const LineModel& getLine(unsigned int identifier) const { return lines.at(identifier); }

  unsigned int getLineIdThrowTwoPoints(unsigned int point1Id, unsigned int point2Id) const;

  /**
   * @brief Number of lines getter
   *
   * @return number of lines
   */
  inline unsigned int getLinesNumber() const { return lines.size(); }

  /**
   * @brief All line objects getter
   *
   * @return reference to vector with all lines
   */
  inline const std::vector<LineModel>& getLinesVector() const { return lines; }

  /**
   * @brief Getting JSON object representing all LineModel objects
   *
   * @return JSON object representing all LineModel objects
   */
  json getLinesAsJsonObjects();

  /**
   * @brief Adding new CircleModel to bank
   *
   * @param id circle ID
   * @param centerId circle center point ID
   * @param centerX cirlce center X coordinate
   * @param centerY circle center Y coordinate
   * @param radius circle radius length
   * @param includedPoints IDs of points inluded in circle
   */
  void addCircle(unsigned int identifier, unsigned int centerId, float centerX, float centerY, float radius,
                 std::vector<unsigned int> includedPoints);

  /**
   * @brief CircleModel object getter
   *
   * @param id ID of circle
   * @return reference to CircleModel
   * @throws std::out_of_range if circle with given ID does not exist
   */
  inline const CircleModel& getCircle(unsigned int identifier) const { return circles.at(identifier); }

  /**
   * @brief Number of circles getter
   *
   * @return number of circles
   */
  inline unsigned int getCirclesNumber() const { return circles.size(); }

  /**
   * @brief All circle objects getter
   *
   * @return reference to vector with all circles
   */
  inline const std::vector<CircleModel>& getCirclesVector() const { return circles; }

  /**
   * @brief Getting JSON object representing all CircleModel objects
   *
   * @return JSON object representing all CircleModel objects
   */
  json getCirclesAsJsonObjects();

  /**
   * @brief Translating the given point ID to the ID set in the bank
   *
   * @param id ID of shape to translate
   * @return point ID in the bank
   */
  unsigned int getUnifiedPointId(unsigned int identifier) const { return pointIdsConverter.at(identifier); }

  /**
   * @brief Translating the given line ID to the ID set in the bank
   *
   * @param id ID of shape to translate
   * @return line ID in the bank
   */
  unsigned int getUnifiedLineId(unsigned int identifier) const { return lineIdsConverter.at(identifier); }

  /**
   * @brief Translating the given circle ID to the ID set in the bank
   *
   * @param id ID of shape to translate
   * @return circle ID in the bank
   */
  unsigned int getUnifiedCircleId(unsigned int identifier) const { return circleIdsConverter.at(identifier); }

  /**
   * @brief Translating the given point IDs vector to the vector with IDs set in the bank
   *
   * @param pointIds IDs of point to translate
   * @return vector with IDs set in the bank
   */
  std::vector<unsigned int> getUnifiedPointsVector(std::vector<unsigned int> pointIds);

  /**
   * @brief Comparator used in sorting points counter-clockwise
   *
   * @param point1 first object to compare
   * @param point2 second object to compare
   * @param centerX
   * @param centerY
   * @return true if point1 is greater than point2
   * @return false if point1 is not greater than point2
   */
  static bool counterClockwiseComparator(const PointModel& point1, const PointModel& point2, float centerX, float centerY);
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_SHAPES_BANK_H
