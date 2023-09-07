#include "IDependency.h"

namespace expertBackground {
const std::vector<IDependency::Category> IDependency::dependencyCategories = {
    IDependency::Category::FORMULA,
    IDependency::Category::POLYGON_TYPE,
    IDependency::Category::OF_POINTS_PAIRS,
    IDependency::Category::OF_LINES,
    IDependency::Category::OF_CIRCLES,
    IDependency::Category::OF_ANGLES,
    IDependency::Category::OF_LINE_AND_ANGLE,
    IDependency::Category::OF_LINE_AND_CIRCLE,
    IDependency::Category::OF_LINE_AND_POINTS_PAIR,
    IDependency::Category::OF_CIRCLE_AND_POLYGON,
    IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR,
    IDependency::Category::OF_POLYGONS};

const std::vector<IDependency::Type> IDependency::dependencyTypes = {
    IDependency::Type::SEGMENT_LENGTH,
    IDependency::Type::ANGLE_MEASURE,
    IDependency::Type::EQUATION,
    IDependency::Type::POLYGON_TYPE,
    IDependency::Type::EQUAL_SEGMENTS,
    IDependency::Type::EQUAL_ANGLES,
    IDependency::Type::PERPENDICULAR_LINES,
    IDependency::Type::PARALLEL_LINES,
    IDependency::Type::TANGENT_LINE_TO_CIRCLE,
    IDependency::Type::TANGENT_CIRCLE_TO_CIRCLE,
    IDependency::Type::BISECTOR_LINE,
    IDependency::Type::MID_PERPENDICULAR_LINE,
    IDependency::Type::INSCRIBED_CIRCLE,
    IDependency::Type::CIRCUMSCRIBED_CIRCLE,
    IDependency::Type::ESCRIBED_CIRCLE,
    IDependency::Type::MEDIAN,
    IDependency::Type::ALTITUDE,
    IDependency::Type::MID_SEGMENT,
    IDependency::Type::SIMILAR_TRIANGLES,
    IDependency::Type::CONGRUENT_TRIANGLES};
}  // namespace expertBackground