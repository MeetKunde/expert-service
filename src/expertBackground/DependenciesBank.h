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

  enum class CirlePolygonDependencies {
    CIRCUMSCRIBED_CIRCLE = static_cast<unsigned int>(IDependency::Type::CIRCUMSCRIBED_CIRCLE),
    INSCRIBED_CIRCLE = static_cast<unsigned int>(IDependency::Type::INSCRIBED_CIRCLE),
    ESCRIBED_CIRCLE = static_cast<unsigned int>(IDependency::Type::ESCRIBED_CIRCLE)
  };

  enum class PointsPairsDependencies {
    MEDIAN = static_cast<unsigned int>(IDependency::Type::MEDIAN),
    EQUAL_SEGMENTS = static_cast<unsigned int>(IDependency::Type::EQUAL_SEGMENTS)
  };

  enum class AnglesDependencies { EQUAL_ANGLES = static_cast<unsigned int>(IDependency::Type::EQUAL_ANGLES) };

  enum class PolygonsDependencies {
    SIMILAR_TRIANGLES = static_cast<unsigned int>(IDependency::Type::SIMILAR_TRIANGLES),
    CONGRUENT_TRIANGLES = static_cast<unsigned int>(IDependency::Type::CONGRUENT_TRIANGLES)
  };

  enum class LinePointPairDependencies {
    MID_PERPENDICULAR_LINE = static_cast<unsigned int>(IDependency::Type::MID_PERPENDICULAR_LINE),
    ALTITUDE = static_cast<unsigned int>(IDependency::Type::ALTITUDE)
  };

  enum class LineAngleDependencies { BISECTOR_LINE = static_cast<unsigned int>(IDependency::Type::BISECTOR_LINE) };

  enum class PointsPairsPairPointsPairDependencies { MID_SEGMENT = static_cast<unsigned int>(IDependency::Type::MID_SEGMENT) };

 private:
  typedef Dependency<IdentifierModel, IdentifierModel> IdentifiersDependency;
  typedef Dependency<PointsPairModel, PointsPairModel> PointPairsDependency;
  typedef Dependency<AngleModel, AngleModel> AnglesDependency;
  typedef Dependency<PolygonModel, PolygonModel> PolygonsDependency;
  typedef Dependency<IdentifierModel, PointsPairModel> IdentifierAndPointsPairDependency;
  typedef Dependency<IdentifierModel, AngleModel> IdentifierAndAngleDependency;
  typedef Dependency<IdentifierModel, PolygonModel> IdentifierAndPolygonDependency;
  typedef Dependency<ModelsPairModel<PointsPairModel>, PointsPairModel> PairOfPointsPairPointsPairDependency;

  static const std::string capitalLetters;

  unsigned int dependencyIdCounter;
  unsigned int valuesBankDimension;

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
  std::map<IDependency::Type, std::vector<unsigned int>> dependenciesMap;

  // key is variable name, value is vector with points which represents it and/or angle type, eg.: {|AB|, {1, 2}} or {|<ABC|, {1, 2, 3, true}}
  std::map<std::string, std::vector<unsigned int>> variableToIndexes;

  // IDs of dependencies which are equations
  std::vector<unsigned int> formulas;

  // it is initialized by all available length and angle names
  // it does not include variables entered from external
  std::map<std::string, std::set<unsigned int>> formulasIncludingVariable;

  // key is single variable, value is vector of available values of this variable
  std::map<std::string, std::vector<ExpressionModel>> extractedEquations;

 public:
  enum EvaluationResult { SUCCESS, FAILURE, UNDEFINED };

  explicit DependenciesBank();
  explicit DependenciesBank(const ShapesBank* generatedShapesBank);

  inline unsigned int getValuesBankDimension() const { return valuesBankDimension; }

  unsigned int addLength(unsigned int id1, unsigned int id2, const symbolicAlgebra::Expression& length,
                         IDependency::Reason reason, std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness);
  static Var getLengthVariable(unsigned int id1, unsigned int id2);

  unsigned int addConvexAngle(unsigned int id1, unsigned int vertexId, unsigned int id2, const symbolicAlgebra::Expression& value,
                              IDependency::Reason reason, std::vector<unsigned int> basedOn,
                              IDependency::UsefulnessLevel usefulness);
  Var getConvexAngleVariable(unsigned int id1, unsigned int vertexId, unsigned int id2);

  unsigned int addConcaveAngle(unsigned int id1, unsigned int vertexId, unsigned int id2,
                               const symbolicAlgebra::Expression& value, IDependency::Reason reason,
                               std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness);
  Var getConcaveAngleVariable(unsigned int id1, unsigned int vertexId, unsigned int id2);

  unsigned int addLinesDependency(unsigned int id1, unsigned int id2, IDependency::Type type, IDependency::Reason reason,
                                  std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LinesDependency>> getLinesDependencies(LinesDependencies type) {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addCirclesDependency(unsigned int id1, unsigned int id2, IDependency::Type type, IDependency::Reason reason,
                                    std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<CirclesDependency>> getCirclesDependencies(CirclesDependencies type) {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPointPairsDependency(unsigned int pair1End1, unsigned int pair1End2, unsigned int pair2End1,
                                       unsigned int pair2End2, IDependency::Type type, IDependency::Reason reason,
                                       std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PointsPairsDependency>> getPointPairsDependencies(PointsPairsDependencies type) {
    return getDependenciesWithType<PointsPairModel, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addAnglesDependency(unsigned int angle1Point1Id, unsigned int angle1VertexId, unsigned int angle1Point2Id,
                                   AngleModel::AngleType angle1Type, unsigned int angle2Point1Id, unsigned int angle2VertexId,
                                   unsigned int angle2Point2Id, AngleModel::AngleType angle2Type, IDependency::Type type,
                                   IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                   IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<AnglesDependency>> getAnglesDependencies(AnglesDependencies type) {
    return getDependenciesWithType<AngleModel, AngleModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPolygonsDependency(std::vector<unsigned int> polygon1VerticesIds, std::vector<unsigned int> polygon2VerticesIds,
                                     IDependency::Type type, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                     IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PolygonsDependency>> getPolygonDependencies(PolygonsDependencies type) {
    return getDependenciesWithType<PolygonModel, PolygonModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLineCircleDependency(unsigned int lineId, unsigned int circleId, IDependency::Type type,
                                       IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                       IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LineCircleDependency>> getLineCircleDependencies(LineCircleDependencies type) {
    return getDependenciesWithType<IdentifierModel, IdentifierModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLinePointPairDependency(unsigned int lineId, unsigned int pairEnd1Id, unsigned pairEnd2Id,
                                          IDependency::Type type, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                          IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LinePointPairDependency>> getLinePointPairDependencies(LinePointPairDependencies type) {
    return getDependenciesWithType<IdentifierModel, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addLineAngleDependency(unsigned int lineId, unsigned int anglePoint1Id, unsigned int angleVertexId,
                                      unsigned int anglePoint2Id, AngleModel::AngleType angleType, IDependency::Type type,
                                      IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                      IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<LineAngleDependency>> getLineAngleDependencies(LineAngleDependencies type) {
    return getDependenciesWithType<IdentifierModel, AngleModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addCirclePolygonDependency(unsigned int circleId, std::vector<unsigned int> polygonVerticesIds,
                                          IDependency::Type type, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                          IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<CirclePolygonDependency>> getCirclePolygonDependencies(CirlePolygonDependencies type) {
    return getDependenciesWithType<IdentifierModel, PolygonModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addPolygonTypeDependency(std::vector<unsigned int> polygonVerticesIds, PolygonModel::PolygonType polygonType,
                                        IDependency::Type type, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                        IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PolygonTypeDependency>> getPolygonTypeDependencies() {
    return getDependenciesWithType<IdentifierModel, PolygonModel>(IDependency::Type::POLYGON_TYPE);
  }

  unsigned int addPointsPairsPairPointsPairDependency(unsigned int arm1End1, unsigned int arm1End2, unsigned int arm2End1,
                                                      unsigned int arm2End2, unsigned int segmentEnd1, unsigned int segmentEnd2,
                                                      IDependency::Type type, IDependency::Reason reason,
                                                      std::vector<unsigned int> basedOn, IDependency::UsefulnessLevel usefulness);
  std::vector<std::shared_ptr<PointsPairsPairPointsPairDependency>> getPointsPairsPairPointsPairDependencies(
      PointsPairsPairPointsPairDependencies type) {
    return getDependenciesWithType<ModelsPairModel<PointsPairModel>, PointsPairModel>(static_cast<IDependency::Type>(type));
  }

  unsigned int addEquation(const symbolicAlgebra::Expression& leftSide, const symbolicAlgebra::Expression& rightSide,
                           IDependency::Reason reason, std::vector<unsigned int> basedOn,
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
    std::vector<unsigned int>::iterator vIt;
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
    std::vector<unsigned int> dependenciesIds = dependenciesMap.at(type);
    std::vector<unsigned int>::iterator vIt;

    for (vIt = dependenciesIds.begin(); vIt != dependenciesIds.end(); ++vIt) {
      result.push_back(std::dynamic_pointer_cast<Dependency<T, U>>(dependenciesVector[*vIt]));
    }

    return result;
  }

  static std::string getPointName(unsigned int identifier);
  static std::string getLengthName(unsigned int end1Id, unsigned int end2Id);
  static std::string getAngleName(unsigned int end1Id, unsigned int vertexId, unsigned int end2Id, bool isConvex);
  void changeAngleEnds(unsigned int* id1, unsigned int* vertexId, unsigned int* id2);
  void add(std::shared_ptr<IDependency> dependencyModel);
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCIES_BANK_H
