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
typedef Dependency<AngleModel, AngleModel> AnglesDependency;
typedef Dependency<PolygonModel, PolygonModel> PolygonsDependency;
typedef Dependency<IdentifierModel, IdentifierModel> LineCircleDependency;
typedef Dependency<IdentifierModel, PolygonModel> PolygonTypeDependency;
typedef Dependency<IdentifierModel, PolygonModel> CirclePolygonDependency;
typedef Dependency<PointsPairModel, PointsPairModel> PointsPairsDependency;
typedef Dependency<IdentifierModel, PointsPairModel> LinePointPairDependency;
typedef Dependency<IdentifierModel, AngleModel> LineAngleDependency;
typedef Dependency<ModelsPairModel<PointsPairModel>, PointsPairModel> PointsPairPairPointsPairDependency;

/**
 * @brief Class storing all dependencies
 */
class DependenciesBank {
 public:
  enum class EquationDependencies {
    SEGMENT_LENGTH = static_cast<unsigned int>(IDependency::Type::SEGMENT_LENGTH),
    ANGLE_MEASURE = static_cast<unsigned int>(IDependency::Type::ANGLE_MEASURE),
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

  enum class PolygonTypeDependencies {
    POLYGON_TYPE = static_cast<unsigned int>(IDependency::Type::POLYGON_TYPE)
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

  enum class PointsPairPairPointsPairDependencies {
    MID_SEGMENT = static_cast<unsigned int>(IDependency::Type::MID_SEGMENT)
  };

 private:
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

 public:
  explicit DependenciesBank();
  explicit DependenciesBank(const ShapesBank* generatedShapesBank);

  inline std::size_t getDependenciesNumber() const { return dependencyIdCounter; }

  inline const std::shared_ptr<IDependency>& getDependencyById(size_t dependencyId) const { return dependenciesVector.at(dependencyId); }

  unsigned int addEquation(const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide,
                           IDependency::Reason reason, std::vector<size_t> basedOn,
                           IDependency::ImportanceLevel importanceLevel);

  unsigned int addLength(const std::string& point1Id, const std::string& point2Id, const symbolicAlgebra::Expression& length,
                         IDependency::Reason reason, std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  unsigned int addConvexAngle(const std::string& point1Id, const std::string& vertexId, const std::string& point2Id,
                              const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                              std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  unsigned int addConcaveAngle(const std::string& point1Id, const std::string& vertexId, const std::string& point2Id,
                               const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                               std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<EquationDependency>> getEquationDependencies(EquationDependencies type) const {
    return getDependenciesWithType<ExpressionModel, ExpressionModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLinesDependency(const std::string& id1, const std::string& id2, LinesDependencies type,
                                  IDependency::Reason reason, std::vector<size_t> basedOn,
                                  IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<LinesDependency>> getLinesDependencies(LinesDependencies type) const {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addCirclesDependency(const std::string& id1, const std::string& id2, CirclesDependencies type,
                                    IDependency::Reason reason, std::vector<size_t> basedOn,
                                    IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<CirclesDependency>> getCirclesDependencies(CirclesDependencies type) const {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPointsPairsDependency(const std::string& pair1End1, const std::string& pair1End2,
                                       const std::string& pair2End1, const std::string& pair2End2,
                                       PointsPairsDependencies type, IDependency::Reason reason,
                                       std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<PointsPairsDependency>> getPointsPairsDependencies(PointsPairsDependencies type) const {
    return getDependenciesWithType<PointsPairModel, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addAnglesDependency(const std::string& angle1Point1Id, const std::string& angle1VertexId,
                                   const std::string& angle1Point2Id, AngleModel::AngleType angle1Type,
                                   const std::string& angle2Point1Id, const std::string& angle2VertexId,
                                   const std::string& angle2Point2Id, AngleModel::AngleType angle2Type,
                                   AnglesDependencies type, IDependency::Reason reason,
                                   std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<AnglesDependency>> getAnglesDependencies(AnglesDependencies type) const {
    return getDependenciesWithType<AngleModel, AngleModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPolygonsDependency(const std::vector<std::string>& polygon1VerticesIds,
                                     const std::vector<std::string>& polygon2VerticesIds,
                                     PolygonsDependencies type, IDependency::Reason reason,
                                     std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<PolygonsDependency>> getPolygonDependencies(PolygonsDependencies type) const {
    return getDependenciesWithType<PolygonModel, PolygonModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLineCircleDependency(const std::string& lineId, const std::string& circleId,
                                       LineCircleDependencies type, IDependency::Reason reason,
                                       std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<LineCircleDependency>> getLineCircleDependencies(LineCircleDependencies type) const {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLinePointPairDependency(const std::string&lineId, const std::string& pairEnd1Id,
                                          const std::string& pairEnd2Id, LinePointPairDependencies type,
                                          IDependency::Reason reason, std::vector<size_t> basedOn,
                                          IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<LinePointPairDependency>> getLinePointPairDependencies(LinePointPairDependencies type) const {
    return getDependenciesWithType<IdentifierModel, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLineAngleDependency(const std::string& lineId, const std::string& anglePoint1Id,
                                      const std::string& angleVertexId, const std::string& anglePoint2Id,
                                      AngleModel::AngleType angleType, LineAngleDependencies type,
                                      IDependency::Reason reason, std::vector<size_t> basedOn,
                                      IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<LineAngleDependency>> getLineAngleDependencies(LineAngleDependencies type) const {
    return getDependenciesWithType<IdentifierModel, AngleModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addCirclePolygonDependency(const std::string& circleId,
                                          const std::vector<std::string>& polygonVerticesIds,
                                          CirclePolygonDependencies type, IDependency::Reason reason,
                                          std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<CirclePolygonDependency>> getCirclePolygonDependencies(CirclePolygonDependencies type) const {
    return getDependenciesWithType<IdentifierModel, PolygonModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPolygonTypeDependency(const std::vector<std::string>& polygonVerticesIds,
                                        PolygonModel::PolygonType polygonType, PolygonTypeDependencies type,
                                        IDependency::Reason reason, std::vector<size_t> basedOn,
                                        IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<PolygonTypeDependency>> getPolygonTypeDependencies(PolygonTypeDependencies /*type*/) const {
    return getDependenciesWithType<IdentifierModel, PolygonModel>(IDependency::Type::POLYGON_TYPE);
  }

  unsigned int addPointsPairPairPointsPairDependency(const std::string& arm1End1, const std::string& arm1End2,
                                                     const std::string& arm2End1, const std::string& arm2End2,
                                                     const std::string& segmentEnd1, const std::string& segmentEnd2,
                                                     PointsPairPairPointsPairDependencies type,
                                                     IDependency::Reason reason,std::vector<size_t> basedOn,
                                                     IDependency::ImportanceLevel importanceLevel);

  std::vector<std::shared_ptr<PointsPairPairPointsPairDependency>> getPointsPairPairPointsPairDependencies(PointsPairPairPointsPairDependencies type) const {
    return getDependenciesWithType<ModelsPairModel<PointsPairModel>, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  json getDependenciesAsJsonObjects() const;
  json getVariablesIndexesAsJsonObject() const;

 private:
  template <class T1, class T2>
  std::vector<std::shared_ptr<Dependency<T1, T2>>> getDependenciesWithType(IDependency::Type type) const {
    std::vector<std::shared_ptr<Dependency<T1, T2>>> result;
    std::vector<size_t> dependenciesIds = dependenciesMap.at(type);

    for (std::vector<size_t>::const_iterator vIt = dependenciesIds.begin(); vIt != dependenciesIds.end(); ++vIt) {
      result.push_back(std::dynamic_pointer_cast<Dependency<T1, T2>>(dependenciesVector[*vIt]));
    }

    return result;
  }

  template <class T1, class T2>
  bool checkIfDependencyExist(std::shared_ptr<Dependency<T1, T2>> dependency) const {
    std::vector<size_t>::const_iterator vIt;
    for (vIt = dependenciesMap.at(dependency->getType()).begin(); vIt != dependenciesMap.at(dependency->getType()).end(); ++vIt) {
      const std::shared_ptr<Dependency<T1, T2>> castedDependency =
          std::dynamic_pointer_cast<Dependency<T1, T2>>(dependenciesVector.at(*vIt));

      if (*castedDependency == *dependency) {
        return true;
      }

    }

    return false;
  }

  template <>
  bool checkIfDependencyExist<ExpressionModel, ExpressionModel>(std::shared_ptr<EquationDependency> dependency) const {
    const std::vector<IDependency::Type> typesToCheck = {
        IDependency::Type::EQUATION,
        IDependency::Type::SEGMENT_LENGTH,
        IDependency::Type::ANGLE_MEASURE
    };

    for(const IDependency::Type& type: typesToCheck) {
      std::vector<size_t>::const_iterator vIt;
      for (vIt = dependenciesMap.at(type).begin(); vIt != dependenciesMap.at(type).end(); ++vIt) {
        const std::shared_ptr<EquationDependency> castedDependency =
            std::dynamic_pointer_cast<EquationDependency>(dependenciesVector.at(*vIt));

        if (*castedDependency == *dependency) {
          return true;
        }
      }
    }

    return false;
  }


  static std::pair<std::string, std::vector<std::string>> getLengthName(const PointModel& point1,
                                                                        const PointModel& point2);

  static std::pair<std::string, std::vector<std::string>> getAngleName(const PointModel& point1,
                                                                       const PointModel& point2,
                                                                       const PointModel& point3,
                                                                       bool angleIsConvex);

  std::vector<std::string> changeAngleEnds(const std::string& point1Id, const std::string& vertexId, const std::string& point2Id);
  void add(std::shared_ptr<IDependency> dependencyModel);
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCIES_BANK_H
