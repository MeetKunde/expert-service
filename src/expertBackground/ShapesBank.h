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
  size_t pointIdCounter;

  /**
   * @brief Line IDs counter
   */
  size_t lineIdCounter;

  /**
   * @brief Circle IDs counter
   */
  size_t circleIdCounter;

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
   * @brief Vector which converts ID of points given by user to position at points vector
   */
  std::map<std::string, size_t> pointIdsConverter;

  /**
   * @brief Vector which converts ID of lines given by user to position at lines vector
   */
  std::map<std::string, size_t> lineIdsConverter;

  /**
   * @brief Vector which converts ID of circles given by user to position at circles vector
   */
  std::map<std::string, size_t> circleIdsConverter;

 public:
  /**
   * @brief Constructor of a new ShapesBank object
   */
  explicit ShapesBank();

  /**
   * @brief Adding new PointModel to bank
   *
   * @param id point ID
   * @param x point X coordinate
   * @param y point Y coordinate
   * @param name point name
   */
  void addPoint(std::string identifier, float xCoordinate, float yCoordinate, std::string name);

  /**
   * @brief PointModel object getter
   *
   * @param identifier ID of point
   * @return reference to PointModel
   * @throws std::out_of_range if point with given ID does not exist
   */
  inline const PointModel& getPoint(const std::string& identifier) const { return points.at(pointIdsConverter.at(identifier)); }

  /**
   * @brief Number of points getter
   *
   * @return number of points
   */
  inline size_t getPointsNumber() const { return points.size(); }

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
  json getPointsAsJsonObjects() const;

  /**
   * Getting point position in vector
   * @param pointId point ID
   * @return position of point
   * @throws std::out_of_range if point with given ID does not exist
   */
  inline size_t getPointPositionInVector(const std::string& pointId) const { return pointIdsConverter.at(pointId); }

  /**
   * @brief Adding new LineModel to bank
   *
   * @param identifier line ID
   * @param lineType line type
   * @param lineA coefficient of x in line equation y = ax + b
   * @param lineB constant term in line equation y = ax + b or y = b or x = b
   * @param includedPoints IDs of points included in line
   */
  void addLine(std::string identifier, LineModel::LineType lineType, float lineA, float lineB,
               std::vector<std::string> includedPoints);

  /**
   * @brief LineModel object getter
   *
   * @param id ID of line
   * @return reference to LineModel
   * @throws std::out_of_range if line with given ID does not exist
   */
  inline const LineModel& getLine(const std::string& identifier) const { return lines.at(lineIdsConverter.at(identifier)); }

  /**
   * @brief ID of LineModel object which pass throw given points
   *
   * @param point1Id ID of first point on line
   * @param point2Id ID of second point on line
   * @return id of LineModel which pass throw given points
   * @throws std::out_of_range if line does not exist
   */
  std::string getLineIdThrowTwoPoints(const std::string& point1Id, const std::string& point2Id) const;

  /**
   * @brief Number of lines getter
   *
   * @return number of lines
   */
  inline size_t getLinesNumber() const { return lines.size(); }

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
  json getLinesAsJsonObjects() const;

  /**
   * Getting line position in vector
   * @param lineId line ID
   * @return position of line
   * @throws std::out_of_range if point with given ID does not exist
   */
  inline size_t getLinePositionInVector(const std::string& lineId) const { return lineIdsConverter.at(lineId); }

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
  void addCircle(std::string identifier, std::string centerId, float centerX, float centerY, std::string centerName, float radius,
                 std::vector<std::string> includedPoints);

  /**
   * @brief CircleModel object getter
   *
   * @param id ID of circle
   * @return reference to CircleModel
   * @throws std::out_of_range if circle with given ID does not exist
   */
  inline const CircleModel& getCircle(const std::string& identifier) const { return circles.at(circleIdsConverter.at(identifier)); }

  /**
   * @brief ID of CircleModel object with given center id and point on id
   *
   * @param centerPointId ID of first point on line
   * @param pointOnCircleId ID of second point on line
   * @return id of CircleModel with given points
   * @throws std::out_of_range if circle does not exist
   */
  std::string getCircleIdWithTwoPoints(const std::string& centerPointId, const std::string& pointOnCircleId) const;

  /**
   * @brief Number of circles getter
   *
   * @return number of circles
   */
  inline size_t getCirclesNumber() const { return circles.size(); }

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
  json getCirclesAsJsonObjects() const;

  /**
   * Getting circle position in vector
   * @param circleId circle ID
   * @return position of circle
   * @throws std::out_of_range if point with given ID does not exist
   */
  inline size_t getCirclePositionInVector(const std::string& circleId) const { return circleIdsConverter.at(circleId); }

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
