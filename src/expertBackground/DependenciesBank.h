#ifndef EXPERT_SERVICE_DEPENDENCIES_BANK_H
#define EXPERT_SERVICE_DEPENDENCIES_BANK_H

#include "ShapesBank.h"
#include "dependency/Dependency.h"
#include "models/AngleModel.h"
#include "models/ExpressionModel.h"
#include "models/IdentifierModel.h"
#include "models/ModelsPairModel.h"
#include "models/PointsPairModel.h"
#include "models/PolygonModel.h"

#include <memory>
#include <string>

namespace expertBackground {

typedef Dependency<ExpressionModel, ExpressionModel> EquationDependency;
typedef Dependency<IdentifierModel, IdentifierModel> LinesDependency;
typedef Dependency<IdentifierModel, IdentifierModel> CirclesDependency;
typedef Dependency<IdentifierModel, IdentifierModel> LineCircleDependency;
typedef Dependency<IdentifierModel, PolygonModel> PolygonTypeDependency;
typedef Dependency<IdentifierModel, PolygonModel> CirclePolygonDependency;
typedef Dependency<PointsPairModel, PointsPairModel> PointsPairsDependency;
typedef Dependency<IdentifierModel, PointsPairModel> LinePointPairDependency;
typedef Dependency<IdentifierModel, AngleModel> LineAngleDependency;
typedef Dependency<ModelsPairModel<PointsPairModel>, PointsPairModel> PointsPairsPairPointsPairDependency;

/**
 * @brief Class storing all dependencies
 */
class DependenciesBank {
 public:
  enum class FormulaDependencies {
    SEGMENT_LENGTH = static_cast<unsigned int>(IDependency::Type::SEGMENT_LENGTH),
    ANGLE_VALUE = static_cast<unsigned int>(IDependency::Type::ANGLE_VALUE),
    EQUATION = static_cast<unsigned int>(IDependency::Type::EQUATION)
  };

  enum class LinesDependencies {
    PERPENDICULAR_LINES = static_cast<unsigned int>(IDependency::Type::PERPENDICULAR_LINES),
    PARALLEL_LINES = static_cast<unsigned int>(IDependency::Type::PARALLEL_LINES)
  };

  enum class CirclesDependencies {
    TANGENT_CIRCLE_TO_CIRCLE = static_cast<unsigned int>(IDependency::Type::TANGENT_CIRCLE_TO_CIRCLE)
  };

  enum class LineCircleDependencies {
    TANGENT_LINE_TO_CIRCLE = static_cast<unsigned int>(IDependency::Type::TANGENT_LINE_TO_CIRCLE)
  };

  enum class CirclePolygonDependencies {
    CIRCUMSCRIBED_CIRCLE = static_cast<unsigned int>(IDependency::Type::CIRCUMSCRIBED_CIRCLE),
    INSCRIBED_CIRCLE = static_cast<unsigned int>(IDependency::Type::INSCRIBED_CIRCLE),
    ESCRIBED_CIRCLE = static_cast<unsigned int>(IDependency::Type::ESCRIBED_CIRCLE)
  };

  enum class PointsPairsDependencies {
    MEDIAN = static_cast<unsigned int>(IDependency::Type::MEDIAN),
    EQUAL_SEGMENTS = static_cast<unsigned int>(IDependency::Type::EQUAL_SEGMENTS)
  };

  enum class AnglesDependencies {
    EQUAL_ANGLES = static_cast<unsigned int>(IDependency::Type::EQUAL_ANGLES)
  };

  enum class PolygonsDependencies {
    SIMILAR_TRIANGLES = static_cast<unsigned int>(IDependency::Type::SIMILAR_TRIANGLES),
    CONGRUENT_TRIANGLES = static_cast<unsigned int>(IDependency::Type::CONGRUENT_TRIANGLES)
  };

  enum class LinePointPairDependencies {
    MID_PERPENDICULAR_LINE = static_cast<unsigned int>(IDependency::Type::MID_PERPENDICULAR_LINE),
    ALTITUDE = static_cast<unsigned int>(IDependency::Type::ALTITUDE)
  };

  enum class LineAngleDependencies {
    BISECTOR_LINE = static_cast<unsigned int>(IDependency::Type::BISECTOR_LINE)
  };

  enum class PointsPairsPairPointsPairDependencies {
    MID_SEGMENT = static_cast<unsigned int>(IDependency::Type::MID_SEGMENT)
  };

 private:
  typedef Dependency<IdentifierModel, IdentifierModel> IdentifiersDependency;
  typedef Dependency<PointsPairModel, PointsPairModel> PointPairsDependency;
  typedef Dependency<AngleModel, AngleModel> AnglesDependency;
  typedef Dependency<PolygonModel, PolygonModel> PolygonsDependency;
  typedef Dependency<IdentifierModel, PointsPairModel> IdentifierAndPointsPairDependency;
  typedef Dependency<IdentifierModel, AngleModel> IdentifierAndAngleDependency;
  typedef Dependency<IdentifierModel, PolygonModel> IdentifierAndPolygonDependency;
  typedef Dependency<ModelsPairModel<PointsPairModel>, PointsPairModel> PairOfPointsPairPointsPairDependency;

  size_t dependencyIdCounter;

  const ShapesBank* shapesBank;

  /**
   * @brief Vector with pointers to all dependency objects
   *
   * Index in vector is equal to object ID
   */
  std::vector<std::shared_ptr<IDependency>> dependenciesVector;

  /**
   * @brief Map with dependencies divided to types.
   *
   * Keys represent available dependency types. Values are vectors with dependency IDs
   */
  std::map<IDependency::Type, std::vector<size_t>> dependenciesMap;

  // key is variable name, value is vector with points which represents it and/or angle type, eg.: {|AB|, {1, 2}} or {|<ABC|, {1, 2, 3, true}}
  std::map<std::string, std::vector<size_t>> variableToIndexes;

  // IDs of dependencies which are equations
  std::vector<size_t> formulas;

  // it is initialized by all available length and angle names
  // it does not include variables entered from external
  std::map<std::string, std::set<size_t>> formulasIncludingVariable;

  // key is single variable, value is vector of available values of this variable
  std::map<std::string, std::vector<ExpressionModel>> extractedEquations;

 public:
  enum EvaluationResult { SUCCESS, FAILURE, UNDEFINED };

  explicit DependenciesBank();
  explicit DependenciesBank(const ShapesBank* generatedShapesBank);

  unsigned int addLength(std::string point1Id, std::string point2Id, const symbolicAlgebra::Expression& length,
                         IDependency::Reason reason, std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness);

  unsigned int addConvexAngle(std::string point1Id, std::string vertexId, std::string point2Id, const symbolicAlgebra::Expression& value,
                              IDependency::Reason reason, std::vector<size_t> basedOn,
                              IDependency::UsefulnessLevel usefulness);

  unsigned int addConcaveAngle(std::string point1Id, std::string vertexId, std::string point2Id,
                               const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                               std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness);

  unsigned int addLinesDependency(std::string id1, std::string id2, IDependency::Type type, IDependency::Reason reason,
                                  std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LinesDependency>> getLinesDependencies(LinesDependencies type) {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addCirclesDependency(std::string id1, std::string id2, IDependency::Type type, IDependency::Reason reason,
                                    std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<CirclesDependency>> getCirclesDependencies(CirclesDependencies type) {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPointPairsDependency(std::string pair1End1, std::string pair1End2, std::string pair2End1,
                                       std::string pair2End2, IDependency::Type type, IDependency::Reason reason,
                                       std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PointsPairsDependency>> getPointPairsDependencies(PointsPairsDependencies type) {
    return getDependenciesWithType<PointsPairModel, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addAnglesDependency(std::string angle1Point1Id, std::string angle1VertexId, std::string angle1Point2Id,
                                   AngleModel::AngleType angle1Type, std::string angle2Point1Id, std::string angle2VertexId,
                                   std::string angle2Point2Id, AngleModel::AngleType angle2Type, IDependency::Type type,
                                   IDependency::Reason reason, std::vector<size_t> basedOn,
                                   IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<AnglesDependency>> getAnglesDependencies(AnglesDependencies type) {
    return getDependenciesWithType<AngleModel, AngleModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPolygonsDependency(std::vector<std::string> polygon1VerticesIds, std::vector<std::string> polygon2VerticesIds,
                                     IDependency::Type type, IDependency::Reason reason, std::vector<size_t> basedOn,
                                     IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PolygonsDependency>> getPolygonDependencies(PolygonsDependencies type) {
    return getDependenciesWithType<PolygonModel, PolygonModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLineCircleDependency(std::string lineId, std::string circleId, IDependency::Type type,
                                       IDependency::Reason reason, std::vector<size_t> basedOn,
                                       IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LineCircleDependency>> getLineCircleDependencies(LineCircleDependencies type) {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLinePointPairDependency(std::string lineId, std::string pairEnd1Id, std::string pairEnd2Id,
                                          IDependency::Type type, IDependency::Reason reason, std::vector<size_t> basedOn,
                                          IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LinePointPairDependency>> getLinePointPairDependencies(LinePointPairDependencies type) {
    return getDependenciesWithType<IdentifierModel, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLineAngleDependency(std::string lineId, std::string anglePoint1Id, std::string angleVertexId,
                                      std::string anglePoint2Id, AngleModel::AngleType angleType, IDependency::Type type,
                                      IDependency::Reason reason, std::vector<size_t> basedOn,
                                      IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LineAngleDependency>> getLineAngleDependencies(LineAngleDependencies type) {
    return getDependenciesWithType<IdentifierModel, AngleModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addCirclePolygonDependency(std::string circleId, std::vector<std::string> polygonVerticesIds,
                                          IDependency::Type type, IDependency::Reason reason, std::vector<size_t> basedOn,
                                          IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<CirclePolygonDependency>> getCirclePolygonDependencies(CirclePolygonDependencies type) {
    return getDependenciesWithType<IdentifierModel, PolygonModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPolygonTypeDependency(std::vector<std::string> polygonVerticesIds, PolygonModel::PolygonType polygonType,
                                        IDependency::Type type, IDependency::Reason reason, std::vector<size_t> basedOn,
                                        IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PolygonTypeDependency>> getPolygonTypeDependencies() {
    return getDependenciesWithType<IdentifierModel, PolygonModel>(IDependency::Type::POLYGON_TYPE);
  }

  unsigned int addPointsPairsPairPointsPairDependency(std::string arm1End1, std::string arm1End2, std::string arm2End1,
                                                      std::string arm2End2, std::string segmentEnd1, std::string segmentEnd2,
                                                      IDependency::Type type, IDependency::Reason reason,
                                                      std::vector<size_t> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PointsPairsPairPointsPairDependency>> getPointsPairsPairPointsPairDependencies(
      PointsPairsPairPointsPairDependencies type) {
    return getDependenciesWithType<ModelsPairModel<PointsPairModel>, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addEquation(const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide,
                           IDependency::Reason reason, std::vector<size_t> basedOn,
                           IDependency::UsefulnessLevel usefulness);
  Equ getEquation(unsigned int dependencyId) const;

  void extractEquations();

  std::pair<DependenciesBank::EvaluationResult, std::vector<unsigned int>> evaluateEquation(
      const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide);
  std::pair<DependenciesBank::EvaluationResult, std::vector<unsigned int>> evaluateEquation(
      const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide,
      std::set<std::string>& variables);

  json getDependenciesAsJsonObjects();
  json getVariablesIndexesAsJsonObject();

 private:
  template <typename T, typename U>
  bool checkIfDependencyExist(std::shared_ptr<Dependency<T, U>> dependency) {
    std::vector<size_t>::iterator vIt;
    for (vIt = dependenciesMap.at(dependency->getType()).begin(); vIt != dependenciesMap.at(dependency->getType()).end(); ++vIt) {
      std::shared_ptr<Dependency<T, U>> castedDependency =
          std::dynamic_pointer_cast<Dependency<T, U>>(dependenciesVector.at(*vIt));
      if (*castedDependency == *dependency) {
        return true;
      }
    }

    return false;
  }

  template <typename T, typename U>
  std::vector<std::shared_ptr<Dependency<T, U>>> getDependenciesWithType(IDependency::Type type) const {
    std::vector<std::shared_ptr<Dependency<T, U>>> result;
    std::vector<size_t> dependenciesIds = dependenciesMap.at(type);

    for (std::vector<size_t>::iterator vIt = dependenciesIds.begin(); vIt != dependenciesIds.end(); ++vIt) {
      result.push_back(std::dynamic_pointer_cast<Dependency<T, U>>(dependenciesVector[*vIt]));
    }

    return result;
  }

  static std::string getLengthName(const PointModel& point1, const PointModel& point2);
  static std::string getAngleName(const PointModel& point1, const PointModel& point2, const PointModel& point3, bool angleIsConvex);
  void changeAngleEnds(std::string& point1Id, std::string& vertexId, std::string& point2Id);
  void add(std::shared_ptr<IDependency> dependencyModel);
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCIES_BANK_H
