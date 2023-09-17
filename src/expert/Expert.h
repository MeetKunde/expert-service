#ifndef EXPERT_SERVICE_EXPERT_H
#define EXPERT_SERVICE_EXPERT_H

#include <expertBackground.h>

namespace expert {

typedef expertBackground::Int Integer;
typedef expertBackground::Rat Rational;
typedef expertBackground::Var Variable;
typedef expertBackground::Const Constant;

typedef expertBackground::MathHelper MathHelper;
typedef expertBackground::IDependency IDependency;

typedef expertBackground::LineModel::LineType LineType;
typedef expertBackground::AngleModel::AngleType AngleType;
typedef expertBackground::PolygonModel::PolygonType PolygonType;

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

  void importTask(json task);
  json exportSolution();
  void useKnowledge();

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

  /*
  //Tools.cpp
  static void printUnsignedIntVector(const std::vector<unsigned int>& vec);
  bool pointsLiesOnOneLine(const std::vector<unsigned int>& points);
  unsigned int setEqualSides(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                             unsigned int segment2End2, IDependency::Reason reason, IDependency::UsefulnessLevel usefulness);
  unsigned int setEqualSides(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                             unsigned int segment2End2, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                             IDependency::UsefulnessLevel usefulness);
  unsigned int setEqualAngles(unsigned int angle1Point1, unsigned int angle1Vertex, unsigned int angle1Point2,
                              unsigned int angle2Point1, unsigned int angle2Vertex, unsigned int angle2Point2,
                              AngleType anglesType, IDependency::Reason reason, IDependency::UsefulnessLevel usefulness);
  unsigned int setEqualAngles(unsigned int angle1Point1, unsigned int angle1Vertex, unsigned int angle1Point2,
                              unsigned int angle2Point1, unsigned int angle2Vertex, unsigned int angle2Point2,
                              AngleType anglesType, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                              IDependency::UsefulnessLevel usefulness);
  unsigned int setSidesParallelism(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                   unsigned int segment2End2, IDependency::Reason reason,
                                   IDependency::UsefulnessLevel usefulness);
  unsigned int setSidesParallelism(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                   unsigned int segment2End2, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                   IDependency::UsefulnessLevel usefulness);
  unsigned int setSidesPerpendicularity(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                        unsigned int segment2End2, IDependency::Reason reason,
                                        IDependency::UsefulnessLevel usefulness);
  unsigned int setSidesPerpendicularity(unsigned int segment1End1, unsigned int segment1End2, unsigned int segment2End1,
                                        unsigned int segment2End2, IDependency::Reason reason, std::vector<unsigned int> basedOn,
                                        IDependency::UsefulnessLevel usefulness);

  // AnglesTheorems.cpp
  unsigned int findVerticalAngles();
  unsigned int findSupplementaryAngles();
  unsigned int findAlternateAngles();
  unsigned int findCorrespondingAngles();

  // PolygonsTheorems.cpp
  unsigned int findSpecificPolygonDependencies(const expertBackground::PolygonModel& polygon, PolygonType type,
                                               unsigned int dependencyId);
  unsigned int findMidPerpendicularDependencies(const expertBackground::PointsPairModel& segment, unsigned int lineId,
                                                unsigned int dependencyId);
  unsigned int findBisectorDependencies(const expertBackground::AngleModel& angle, unsigned int lineId,
                                        unsigned int dependencyId);
  unsigned int findTangentLineDependencies(unsigned int lineId, unsigned int circleId, unsigned int dependencyId);
  unsigned int findMedianDependencies(const expertBackground::PointsPairModel& base,
                                      const expertBackground::PointsPairModel& median, unsigned int dependencyId);
  unsigned int findAltitudeDependencies(unsigned int baseLineId, const expertBackground::PointsPairModel& altitude,
                                        unsigned int dependencyId);
  unsigned int findMidSegmentDependencies(const expertBackground::PointsPairModel& arm1,
                                          const expertBackground::PointsPairModel& arm2,
                                          const expertBackground::PointsPairModel& midSegment, unsigned int dependencyId);
  unsigned int findInscribedCircleDependencies(const expertBackground::PolygonModel& polygon, unsigned int circleId,
                                               unsigned int dependencyId);
  unsigned int findEscribedCircleDependencies(const expertBackground::PolygonModel& polygon, unsigned int circleId,
                                              unsigned int dependencyId);
  unsigned int findSpecificShapesDependencies();

  // LinesTheorems.cpp
  unsigned int setRightAnglesBasedOnPerpendicularities();
  unsigned int findParallelLinesBasedOnParallelLines();
  unsigned int findParallelLinesBasedOnPerpendicularLines();
  unsigned int findPerpendicularLinesBasedOnLines();
  unsigned int findPerpendicularLinesBasedOnRightAngles();
   */
};

}  // namespace expert

#endif  //EXPERT_SERVICE_EXPERT_H
