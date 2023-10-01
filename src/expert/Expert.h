#ifndef EXPERT_SERVICE_EXPERT_H
#define EXPERT_SERVICE_EXPERT_H

#include <expertBackground.h>

namespace expert {

typedef expertBackground::Int Integer;
typedef expertBackground::Rat Rational;
typedef expertBackground::Var Variable;
typedef expertBackground::Const Constant;

typedef expertBackground::ShapesBank ShapesBank;
typedef expertBackground::DependenciesBank DependenciesBank;

typedef expertBackground::MathHelper MathHelper;

typedef expertBackground::PointModel PointModel;
typedef expertBackground::LineModel LineModel;
typedef expertBackground::CircleModel CircleModel;
typedef expertBackground::PolygonModel PolygonModel;
typedef expertBackground::AngleModel AngleModel;
typedef expertBackground::ExpressionModel ExpressionModel;
typedef expertBackground::IdentifierModel IdentifierModel;

typedef expertBackground::LineModel::LineType LineType;
typedef expertBackground::AngleModel::AngleType AngleType;
typedef expertBackground::PolygonModel::PolygonType PolygonType;

typedef expertBackground::IDependency IDependency;

typedef expertBackground::EquationDependency EquationDependency;
typedef expertBackground::DependenciesBank::EquationDependencies EquationDependencies;
typedef expertBackground::LinesDependency LinesDependency;
typedef expertBackground::DependenciesBank::LinesDependencies LinesDependencies;
typedef expertBackground::CirclesDependency CirclesDependency;
typedef expertBackground::DependenciesBank::CirclesDependencies CirclesDependencies;
typedef expertBackground::LineCircleDependency LineCircleDependency;
typedef expertBackground::DependenciesBank::LineCircleDependencies LineCircleDependencies;
typedef expertBackground::CirclePolygonDependency CirclePolygonDependency;
typedef expertBackground::DependenciesBank::CirclePolygonDependencies CirclePolygonDependencies;
typedef expertBackground::PolygonTypeDependency PolygonTypeDependency;
typedef expertBackground::DependenciesBank::PolygonTypeDependencies PolygonTypeDependencies;
typedef expertBackground::PointsPairsDependency PointsPairsDependency;
typedef expertBackground::DependenciesBank::PointsPairsDependencies PointsPairsDependencies;
typedef expertBackground::AnglesDependency AnglesDependency;
typedef expertBackground::DependenciesBank::AnglesDependencies AnglesDependencies;
typedef expertBackground::PolygonsDependency PolygonsDependency;
typedef expertBackground::DependenciesBank::PolygonsDependencies PolygonsDependencies;
typedef expertBackground::LinePointsPairDependency LinePointsPairDependency;
typedef expertBackground::DependenciesBank::LinePointsPairDependencies LinePointsPairDependencies;
typedef expertBackground::LineAngleDependency LineAngleDependency;
typedef expertBackground::DependenciesBank::LineAngleDependencies LineAngleDependencies;
typedef expertBackground::PointsPairPairPointsPairDependency PointsPairPairPointsPairDependency;
typedef expertBackground::DependenciesBank::PointsPairPairPointsPairDependencies PointsPairPairPointsPairDependencies;
typedef expertBackground::PolygonPointsPairDependency PolygonPointsPairDependency;
typedef expertBackground::DependenciesBank::PolygonPointsPairDependencies PolygonPointsPairDependencies;
typedef expertBackground::PolygonExpressionDependency PolygonExpressionDependency;
typedef expertBackground::DependenciesBank::PolygonExpressionDependencies PolygonExpressionDependencies;


class Expert {
 private:
  static constexpr int VERTICAL_LINE_TYPE_ID = 0;
  static constexpr int HORIZONTAL_LINE_TYPE_ID = 1;
  static constexpr int SLANTED_LINE_TYPE_ID = 2;

  static constexpr int UNKNOWN_ANGLE_TYPE_ID = 0;
  static constexpr int CONVEX_ANGLE_TYPE_ID = 1;
  static constexpr int CONCAVE_ANGLE_TYPE_ID = 2;

  static constexpr int ISOSCELES_ACUTE_TRIANGLE_POLYGON_TYPE_ID = 1;
  static constexpr int EQUILATERAL_TRIANGLE_POLYGON_TYPE_ID = 2;
  static constexpr int SCALENE_RIGHT_TRIANGLE_POLYGON_TYPE_ID = 3;
  static constexpr int ISOSCELES_RIGHT_TRIANGLE_POLYGON_TYPE_ID = 4;
  static constexpr int OBTUSE_ISOSCELES_TRIANGLE_POLYGON_TYPE_ID = 5;
  static constexpr int SQUARE_POLYGON_TYPE_ID = 6;
  static constexpr int RECTANGLE_POLYGON_TYPE_ID = 7;
  static constexpr int REGULAR_POLYGON_TYPE_ID = 8;
  static constexpr int PARALLELOGRAM_POLYGON_TYPE_ID = 9;
  static constexpr int KITE_POLYGON_TYPE_ID = 10;
  static constexpr int RHOMBUS_POLYGON_TYPE_ID = 11;
  static constexpr int SCALENE_TRAPEZOID_POLYGON_TYPE_ID = 12;
  static constexpr int ISOSCELES_TRAPEZOID_POLYGON_TYPE_ID = 13;
  static constexpr int RIGHT_TRAPEZOID_POLYGON_TYPE_ID = 14;

  static constexpr size_t EXERCISE_DESCRIPTION_ID = 0;

  expertBackground::ShapesBank shapesBank;
  expertBackground::DependenciesBank dependenciesBank;

  expertBackground::Graph schemeGraph;

  std::vector<std::vector<std::vector<size_t>>> intersectionPointsOfLines;
  std::vector<std::vector<std::vector<size_t>>> intersectionPointsOfCircles;
  std::vector<std::vector<std::vector<size_t>>> intersectionPointsOfLinesAndCircles;

  std::vector<std::vector<std::pair<size_t, size_t>>> pointsOnLinesIntersections;
  std::vector<std::vector<std::pair<size_t, size_t>>> pointsOnCirclesIntersections;
  std::vector<std::vector<std::pair<size_t, size_t>>> pointsOnLineAndCircleIntersections;

  std::vector<std::vector<bool>> pointsOnShapes;

  expertBackground::LocalTimer localTimer;

 public:
  Expert();

  void importTask(json task, std::ostream& stream);
  json exportSolution(std::ostream& stream);
  void useKnowledge(std::ostream& stream);

 private:
  void addValues(json lengths, json angleValues, json formulas, json perimeters, json areas);
  void addLinesDependencies(json perpendicular, json parallel);
  void addEqualismDependencies(json segments, json angles);
  void addTangentLinesAndCircles(json lines, json circles);
  void addCirclePolygonDependencies(json inscribed, json circumscribed, json escribed);
  void addBisectors(json bisectorLines);
  void addMidPerpendiculars(json midPerpendicularLines);
  void addPolygonTypes(json types);
  void addSpecialSegments(json medians, json altitudes, json midSegments);

  static LineType parseLineType(unsigned int lineType);
  static AngleType parseAngleType(unsigned int angleType);
  static PolygonType parsePolygonType(unsigned int polygonType);

  void findIntersectionPointsOfLines();
  void findIntersectionPointsOfCircles();
  void findIntersectionPointsOfLinesCircles();
  void checkPointsOnShapes();
  json getIntersectionPointsAsJson();

  // Tools.cpp
  bool pointsLiesOnOneLine(const std::vector<std::string>& pointIds) const;
  unsigned int setEqualSides(const std::string& segment1End1Id, const std::string& segment1End2Id,
                             const std::string& segment2End1Id, const std::string& segment2End2Id,
                             IDependency::Reason reason, std::vector<size_t> dependentDependencies,
                             IDependency::ImportanceLevel importanceLevel);
  unsigned int setEqualAngles(const std::string& angle1Point1, const std::string& angle1Vertex, const std::string& angle1Point2,
                              const std::string& angle2Point1, const std::string& angle2Vertex, const std::string& angle2Point2,
                              AngleType anglesType,
                              IDependency::Reason reason, std::vector<size_t> dependentDependencies,
                              IDependency::ImportanceLevel importanceLevel);
  unsigned int setSidesParallelism(const std::string& segment1End1Id, const std::string& segment1End2Id,
                                   const std::string& segment2End1Id, const std::string& segment2End2Id,
                                   IDependency::Reason reason, std::vector<size_t> dependentDependencies,
                                   IDependency::ImportanceLevel importanceLevel);
  unsigned int setSidesPerpendicularity(const std::string& segment1End1Id, const std::string& segment1End2Id,
                                        const std::string& segment2End1Id, const std::string&segment2End2Id,
                                        IDependency::Reason reason, std::vector<size_t> dependentDependencies,
                                        IDependency::ImportanceLevel importanceLevel);

  // LinesTheorems.cpp
  unsigned int exploreLineBasedDependencies();
  unsigned int setRightAnglesBasedOnPerpendicularLines();
  unsigned int findParallelLinesBasedOnParallelLines();
  unsigned int findParallelLinesBasedOnPerpendicularLines();
  unsigned int findPerpendicularLinesBasedOnLines();
  unsigned int findPerpendicularLinesBasedOnRightAngles();

  // AnglesTheorems.cpp
  unsigned int exploreAngleBasedTheorems();
  unsigned int findVerticalAngles();
  unsigned int findSupplementaryAngles();
  unsigned int findAlternateAngles();
  unsigned int findCorrespondingAngles();

  // SpecificPropertiesTheorems.cpp
  unsigned int explorePolygonTypeBasedDependencies();
  unsigned int setIsoscelesAcuteTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setEquilateralTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setScaleneRightTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setIsoscelesRightTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setObtuseIsoscelesTriangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setSquareDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setRectangleDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setRegularPolygonDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setParallelogramDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setKiteDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setRhombusDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setScaleneTrapezoidDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setIsoscelesTrapezoidDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);
  unsigned int setRightTrapezoidDependencies(const std::shared_ptr<PolygonTypeDependency>& dependency);

  unsigned int exploreSpecificSegmentsBasedDependencies();
  unsigned int setMedianDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency);
  unsigned int setAltitudeDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency);
  unsigned int setMidSegmentDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency);
  unsigned int setMidSegmentInTriangleDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency);
  unsigned int setMidSegmentInTrapezoidDependencies(const std::shared_ptr<PolygonPointsPairDependency>& dependency);

  unsigned int exploreTangentLineAndCircleBasedDependencies();
  unsigned int setTangentLineCircleDependencies(const std::shared_ptr<LineCircleDependency>& dependency);
  unsigned int setTangentCircleCircleDependencies(const std::shared_ptr<CirclesDependency>& dependency);

  unsigned int explorePolygonCircleBasedDependencies();
  unsigned int setInscribedCircleDependencies(const std::shared_ptr<CirclePolygonDependency>& dependency);
  unsigned int setCircumscribedCircleDependencies(const std::shared_ptr<CirclePolygonDependency>& dependency);
  unsigned int setEscribedCircleDependencies(const std::shared_ptr<CirclePolygonDependency>& dependency);

  unsigned int exploreSpecificLineBasedDependencies();
  unsigned int setMidPerpendicularLineDependencies(const std::shared_ptr<LinePointsPairDependency>& dependency);
  unsigned int setBisectorLineDependencies(const std::shared_ptr<LineAngleDependency>& dependency);
};

}  // namespace expert

#endif  //EXPERT_SERVICE_EXPERT_H
