#ifndef EXPERT_SERVICE_I_DEPENDENCY_H
#define EXPERT_SERVICE_I_DEPENDENCY_H

#include <nlohmann/json.hpp>

typedef nlohmann::json json;

namespace expertBackground {

class IDependency {
 public:
  /**
   * @brief Representing category of dependency
   */
  enum class Category {
    FORMULA,
    POLYGON_TYPE,
    OF_POINTS_PAIRS,
    OF_LINES,
    OF_CIRCLES,
    OF_ANGLES,
    OF_LINE_AND_ANGLE,
    OF_LINE_AND_CIRCLE,
    OF_LINE_AND_POINTS_PAIR,
    OF_CIRCLE_AND_POLYGON,
    OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR,
    OF_POLYGONS,
    OF_POLYGON_AND_POINTS_PAIRS,
    OF_POLYGON_AND_EXPRESSION
  };

  static const std::vector<Category> dependencyCategories;

  /**
   * @brief Representing type of dependency
   */
  enum class Type {
    SEGMENT_LENGTH,
    ANGLE_MEASURE,
    EQUATION,
    POLYGON_TYPE,
    POLYGON_PERIMETER,
    POLYGON_AREA,
    EQUAL_SEGMENTS,
    EQUAL_ANGLES,
    PERPENDICULAR_LINES,
    PARALLEL_LINES,
    TANGENT_LINE_TO_CIRCLE,
    TANGENT_CIRCLE_TO_CIRCLE,
    BISECTOR_LINE,
    MID_PERPENDICULAR_LINE,
    INSCRIBED_CIRCLE,
    CIRCUMSCRIBED_CIRCLE,
    ESCRIBED_CIRCLE,
    MEDIAN,
    ALTITUDE,
    MID_SEGMENT,
    SIMILAR_TRIANGLES,
    CONGRUENT_TRIANGLES
  };

  static const std::vector<Type> dependencyTypes;

  /**
   * @brief Representing reason of dependency
   */
  enum class Reason {
    NONE,
    USER_DEFINED,
    DIVIDED_SEGMENT,
    MID_PERPENDICULAR,
    TANGENT_TO_CIRCLE,
    TANGENT_TO_LINE,
    ALTITUDE,
    MEDIAN,
    MID_SEGMENT,
    BISECTOR,
    INSCRIBED_CIRCLE,
    CIRCUMSCRIBED_CIRCLE,
    ESCRIBED_CIRCLE,
    SQUARE,
    SQUARE_DIAGONAL,
    RECTANGLE,
    RECTANGLE_DIAGONAL,
    REGULAR_POLYGON,
    PARALLELOGRAM,
    PARALLELOGRAM_DIAGONAL,
    KITE,
    KITE_DIAGONAL,
    RHOMBUS,
    RHOMBUS_DIAGONAL,
    ISOSCELES_ACUTE_TRIANGLE,
    EQUILATERAL_TRIANGLE,
    SCALENE_RIGHT_TRIANGLE,
    ISOSCELES_RIGHT_TRIANGLE,
    OBTUSE_ISOSCELES_TRIANGLE,
    SCALENE_TRAPEZOID,
    ISOSCELES_TRAPEZOID,
    ISOSCELES_TRAPEZOID_DIAGONAL,
    RIGHT_TRAPEZOID,
    POINTS_ARE_THE_SAME,
    ARMS_ARE_THE_SAME,
    VERTICAL_ANGLES,
    SUPPLEMENTARY_ANGLES,
    ALTERNATE_ANGLES,
    CORRESPONDING_ANGLES,
    PERPENDICULAR_LINES,
    PARALLEL_LINES,
    PARALLELISM_TRANSITIVITY,
    PERPENDICULARITY_COMPOSITION,
    PERPENDICULARITY_AND_PARALLELISM_COMPOSITION,
    RIGHT_ANGLE,
    SUM_OF_ANGLES_IN_TRIANGLE,
    SIMILAR_TRIANGLES,
    SIDE_SIDE_SIDE,
    ANGLE_ANGLE_ANGLE,
    CONGRUENT_TRIANGLES,
    SIDE_ANGLE_SIDE,
    ANGLE_SIDE_ANGLE,
    SAME_ARMS,
    FUNDAMENTAL_THEORY_OF_GEOMETRY,
    LINE_TANGENT_TO_CIRCLE,
    EQUATION_EXTRACTION
  };

  enum class ImportanceLevel {
    LOW,
    MEDIUM,
    HIGH
  };

  virtual json getObjectAsJson() const = 0;
  virtual inline size_t getId() const = 0;
  virtual inline Category getCategory() const = 0;
  virtual inline Type getType() const = 0;
  virtual inline Reason getReason() const = 0;
  virtual inline const std::vector<size_t>& getDependentDependencies() const = 0;
  virtual inline ImportanceLevel getImportanceLevel() const = 0;

  virtual ~IDependency() {}
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_I_DEPENDENCY_H
