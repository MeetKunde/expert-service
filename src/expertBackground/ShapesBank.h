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
 public:
  explicit ShapesBank();

  ShapesBank(const ShapesBank&) = default;
  ShapesBank(ShapesBank&&) = default;

  ShapesBank& operator=(const ShapesBank&) = default;
  ShapesBank& operator=(ShapesBank&&) = default;

  ~ShapesBank() = default;

  void addPoint(const std::string& identifier, float xCoordinate, float yCoordinate, const std::string& name);
  
  inline const PointModel& getPoint(const std::string& identifier) const { return points.at(pointIdsConverter.at(identifier)); }
  inline size_t getPointsNumber() const { return points.size(); }
  inline const std::vector<PointModel>& getPointsVector() const { return points; }
  inline size_t getPointPositionInVector(const std::string& pointId) const { return pointIdsConverter.at(pointId); }

  json getPointsJson() const;

  void addLine(const std::string& identifier, LineModel::Type lineType, float lineA, float lineB,
               const std::vector<std::string>& includedPoints);

  inline const LineModel& getLine(const std::string& identifier) const { return lines.at(lineIdsConverter.at(identifier)); }
  std::string getLineIdThrowTwoPoints(const std::string& point1Id, const std::string& point2Id) const;
  inline size_t getLinesNumber() const { return lines.size(); }
  inline const std::vector<LineModel>& getLinesVector() const { return lines; }
  inline size_t getLinePositionInVector(const std::string& lineId) const { return lineIdsConverter.at(lineId); }

  json getLinesJson() const;

  void addCircle(const std::string& identifier, const std::string& centerId, float radius, 
                 const std::vector<std::string>& includedPoints);

  inline const CircleModel& getCircle(const std::string& identifier) const { return circles.at(circleIdsConverter.at(identifier)); }
  std::string getCircleIdWithTwoPoints(const std::string& centerPointId, const std::string& pointOnCircleId) const;
  inline size_t getCirclesNumber() const { return circles.size(); }
  inline const std::vector<CircleModel>& getCirclesVector() const { return circles; }
  inline size_t getCirclePositionInVector(const std::string& circleId) const { return circleIdsConverter.at(circleId); }

  json getCirclesJson() const;

 private:
  static bool counterClockwiseComparator(const PointModel& point1, const PointModel& point2, const PointModel& center);

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
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_SHAPES_BANK_H
