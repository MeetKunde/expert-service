#ifndef EXPERT_SERVICE_I_DEPENDENCY_H
#define EXPERT_SERVICE_I_DEPENDENCY_H

#include <nlohmann/json.hpp>

typedef nlohmann::json json;

namespace expertBackground {
/**
 * @brief Interface for dependency
 */
class IDependency {
 public:
  /**
   * @brief Enum representing category of dependency
   */
  enum class Category {
    FORMULA,
    POLYGON_TYPE,
    ANGLE_TYPE,
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

  static constexpr Category CategoryBegin = Category::FORMULA;
  static constexpr Category CategoryEnd = static_cast<Category>(static_cast<int>(Category::OF_POLYGON_AND_EXPRESSION) + 1);

  /**
   * @brief Enum representing type of dependency
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

  static constexpr Type TypeBegin = Type::SEGMENT_LENGTH;
  static constexpr Type TypeEnd = static_cast<Type>(static_cast<int>(Type::CONGRUENT_TRIANGLES) + 1);

  /**
   * @brief Enum representing reason of dependency
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

  static constexpr Reason ReasonBegin = Reason::NONE;
  static constexpr Reason ReasonEnd = static_cast<Reason>(static_cast<int>(Reason::EQUATION_EXTRACTION) + 1);

  /**
   * @brief Enum representing importance of dependency
   */
  enum class Importance {
    LOW,
    MEDIUM,
    HIGH
  };

  static constexpr Importance ImportanceBegin = Importance::LOW;
  static constexpr Importance ImportanceEnd = static_cast<Importance>(static_cast<int>(Importance::HIGH) + 1);

  virtual inline size_t getId() const = 0;
  virtual inline Category getCategory() const = 0;
  virtual inline Type getType() const = 0;
  virtual inline Reason getReason() const = 0;
  virtual inline const std::vector<size_t>& getDependentDependencies() const = 0;
  virtual inline Importance getImportance() const = 0;
  virtual json getJson() const = 0;
};


void operator++(IDependency::Category& category, int) {
  if (category == IDependency::CategoryEnd) {
    throw std::out_of_range("Category out of range");
  }

  category = static_cast<IDependency::Category>(static_cast<int>(category) + 1);
}

void operator++(IDependency::Category& category) {
  if (category == IDependency::CategoryEnd) {
    throw std::out_of_range("Category out of range");
  }

  category = static_cast<IDependency::Category>(static_cast<int>(category) + 1);
}

void operator++(IDependency::Type& type, int) {
  if (type == IDependency::TypeEnd) {
    throw std::out_of_range("Type out of range");
  }

  type = static_cast<IDependency::Type>(static_cast<int>(type) + 1);
}

void operator++(IDependency::Type& type) {
  if (type == IDependency::TypeEnd) {
    throw std::out_of_range("Type out of range");
  }

  type = static_cast<IDependency::Type>(static_cast<int>(type) + 1);
}

void operator++(IDependency::Reason& reason, int) {
  if (reason == IDependency::ReasonEnd) {
    throw std::out_of_range("Reason out of range");
  }

  reason = static_cast<IDependency::Reason>(static_cast<int>(reason) + 1);
}

void operator++(IDependency::Reason& reason) {
  if (reason == IDependency::ReasonEnd) {
    throw std::out_of_range("Reason out of range");
  }

  reason = static_cast<IDependency::Reason>(static_cast<int>(reason) + 1);
}

void operator++(IDependency::Importance& importance, int) {
  if (importance == IDependency::ImportanceEnd) {
    throw std::out_of_range("Importance out of range");
  }

  importance = static_cast<IDependency::Importance>(static_cast<int>(importance) + 1);
}

void operator++(IDependency::Importance& importance) {
  if (importance == IDependency::ImportanceEnd) {
    throw std::out_of_range("Importance out of range");
  }

  importance = static_cast<IDependency::Importance>(static_cast<int>(importance) + 1);
}
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_I_DEPENDENCY_H
