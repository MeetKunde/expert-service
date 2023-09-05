#include "ExpertBackgroundTestBase.h"

TEST_CASE("Creating empty ShapesBank", "[expertBackground]") {
  const expertBackground::ShapesBank emptyShapesBank;

  const std::string shapeId{"some-id"};

  REQUIRE_THROWS(emptyShapesBank.getPoint(shapeId));
  REQUIRE(emptyShapesBank.getPointsNumber() == 0);
  REQUIRE(emptyShapesBank.getPointsVector().empty());
  REQUIRE(emptyShapesBank.getPointsAsJsonObjects().empty());
  REQUIRE_THROWS(emptyShapesBank.getPointPositionInVector(shapeId));

  REQUIRE_THROWS(emptyShapesBank.getLine(shapeId));
  REQUIRE_THROWS(emptyShapesBank.getLineIdThrowTwoPoints(shapeId, shapeId));
  REQUIRE(emptyShapesBank.getLinesNumber() == 0);
  REQUIRE(emptyShapesBank.getLinesVector().empty());
  REQUIRE(emptyShapesBank.getLinesAsJsonObjects().empty());
  REQUIRE_THROWS(emptyShapesBank.getLinePositionInVector(shapeId));

  REQUIRE_THROWS(emptyShapesBank.getCircle(shapeId));
  REQUIRE_THROWS(emptyShapesBank.getCircleIdWithTwoPoints(shapeId, shapeId));
  REQUIRE(emptyShapesBank.getCirclesNumber() == 0);
  REQUIRE(emptyShapesBank.getCirclesVector().empty());
  REQUIRE(emptyShapesBank.getCirclesAsJsonObjects().empty());
  REQUIRE_THROWS(emptyShapesBank.getCirclePositionInVector(shapeId));
}

TEST_CASE("Adding point to ShapesBank", "[expertBackground]") {
  expertBackground::ShapesBank shapesBank;

  const std::string pointId{"point id"};
  const float pointXCoord{10.0F};
  const float pointYCoord{10.0F};
  const std::string pointName{"P"};
  const size_t pointPositionInPointsVector{0};
  const json pointJson{
      {"object", {{"id", pointId}, {"x", pointXCoord}, {"y", pointYCoord}, {"name", pointName }}},
      {"position", 0}
  };

  shapesBank.addPoint(pointId, pointXCoord, pointYCoord, pointName);

  REQUIRE(shapesBank.getPoint(pointId).getId() == pointId);
  REQUIRE_THAT(shapesBank.getPoint(pointId).getX(), Catch::Matchers::WithinAbs(pointXCoord, EPSILON));
  REQUIRE_THAT(shapesBank.getPoint(pointId).getY(), Catch::Matchers::WithinAbs(pointYCoord, EPSILON));
  REQUIRE(shapesBank.getPoint(pointId).getName() == pointName);
  REQUIRE(shapesBank.getPointsNumber() == 1);
  REQUIRE(shapesBank.getPointsVector().size() == 1);
  REQUIRE(shapesBank.getPointsVector().at(0) == shapesBank.getPoint(pointId));
  REQUIRE(shapesBank.getPointsAsJsonObjects().size() == 1);
  REQUIRE(shapesBank.getPointsAsJsonObjects().at(0) == pointJson);
  REQUIRE(shapesBank.getPointPositionInVector(pointId) == pointPositionInPointsVector);
}

TEST_CASE("Adding line to ShapesBank", "[expertBackground]") {
  expertBackground::ShapesBank shapesBank;

  const std::string point1Id{"point 1 id"};
  const float point1CoordX{3.0F};
  const float point1CoordY{2.0F};
  const std::string point1Name{"A"};

  const std::string point2Id{"point 2 id"};
  const float point2CoordX{-3.0F};
  const float point2CoordY{2.0F};
  const std::string point2Name{"B"};

  const std::string point3Id{"point 3 id"};
  const float point3CoordX{-3.0F};
  const float point3CoordY{-3.0F};
  const std::string point3Name{"C"};

  const std::string point4Id{"point 4 id"};
  const float point4CoordX{-3.0F};
  const float point4CoordY{-1.0F};
  const std::string point4Name{"D"};

  const std::string point5Id{"point 5 id"};
  const float point5CoordX{1.0F};
  const float point5CoordY{2.0F};
  const std::string point5Name{"E"};

  const std::string point6Id{"point 6 id"};
  const float point6CoordX{5.0F};
  const float point6CoordY{5.0F};
  const std::string point6Name{"F"};

  const std::string point7Id{"point 7 id"};
  const float point7CoordX{-7.0F};
  const float point7CoordY{-4.0F};
  const std::string point7Name{"G"};

  const size_t pointsNumber{7};

  const std::string line1Id{"line 1 id"};
  const expertBackground::LineModel::LineType line1Type{expertBackground::LineModel::LineType::SLANTED};
  const float line1A{0.75F};
  const float line1B{1.25F};
  const std::vector<std::string> line1Points{point4Id, point5Id, point6Id, point7Id};
  const std::vector<std::string> line1PointsOrder{point7Id, point4Id, point5Id, point6Id};
  const size_t line1PositionInLinesVector{0};
  const json line1Json{
      {"object", {{"a", line1A}, {"b", line1B}, {"end1Id", point7Id}, {"end2Id", point6Id}, {"id", line1Id}, {"pointsOn", json(line1PointsOrder)}, {"type", line1Type}}},
      {"position", 0}
  };

  const std::string line2Id{"line 2 id"};
  const expertBackground::LineModel::LineType line2Type{expertBackground::LineModel::LineType::HORIZONTAL};
  const float line2A{0.0F};
  const float line2B{2.0F};
  const std::vector<std::string> line2Points{point1Id, point2Id, point5Id};
  const std::vector<std::string> line2PointsOrder{point2Id, point5Id, point1Id};

  const std::string line3Id{"line 3 id"};
  const expertBackground::LineModel::LineType line3Type{expertBackground::LineModel::LineType::VERTICAL};
  const float line3A{0.0F};
  const float line3B{-3.0F};
  const std::vector<std::string> line3Points{point2Id, point3Id, point4Id};
  const std::vector<std::string> line3PointsOrder{point3Id, point4Id, point2Id};

  const size_t linesNumber{3};

  shapesBank.addPoint(point1Id, point1CoordX, point1CoordY, point1Name);
  shapesBank.addPoint(point2Id, point2CoordX, point2CoordY, point2Name);
  shapesBank.addPoint(point3Id, point3CoordX, point3CoordY, point3Name);
  shapesBank.addPoint(point4Id, point4CoordX, point4CoordY, point4Name);
  shapesBank.addPoint(point5Id, point5CoordX, point5CoordY, point5Name);
  shapesBank.addPoint(point6Id, point6CoordX, point6CoordY, point6Name);
  shapesBank.addPoint(point7Id, point7CoordX, point7CoordY, point7Name);

  shapesBank.addLine(line1Id, line1Type, line1A, line1B, line1Points);
  shapesBank.addLine(line2Id, line2Type, line2A, line2B, line2Points);
  shapesBank.addLine(line3Id, line3Type, line3A, line3B, line3Points);

  REQUIRE(shapesBank.getPointsNumber() == pointsNumber);

  REQUIRE(shapesBank.getLine(line1Id).getId() == line1Id);
  REQUIRE(shapesBank.getLine(line1Id).getLineType() == line1Type);
  REQUIRE_THAT(shapesBank.getLine(line1Id).getLineA(), Catch::Matchers::WithinAbs(line1A, EPSILON));
  REQUIRE_THAT(shapesBank.getLine(line1Id).getLineB(), Catch::Matchers::WithinAbs(line1B, EPSILON));
  REQUIRE(shapesBank.getLine(line1Id).getIncludedPoints().size() == line1Points.size());
  REQUIRE(shapesBank.getLinesNumber() == linesNumber);
  REQUIRE(shapesBank.getLinesVector().size() == linesNumber);
  REQUIRE(shapesBank.getLinesVector().at(0) == shapesBank.getLine(line1Id));
  REQUIRE(shapesBank.getLinesAsJsonObjects().size() == linesNumber);
  REQUIRE(shapesBank.getLinesAsJsonObjects().at(0) == line1Json);
  REQUIRE(shapesBank.getLinePositionInVector(line1Id) == line1PositionInLinesVector);
  REQUIRE(shapesBank.getLineIdThrowTwoPoints(point7Id, point5Id) == line1Id);

  for(size_t pointIdx = 0; pointIdx < line1PointsOrder.size(); pointIdx++) {
    REQUIRE(shapesBank.getLine(line1Id).getIncludedPoints().at(pointIdx) == line1PointsOrder.at(pointIdx));
  }

  for(size_t pointIdx = 0; pointIdx < line2PointsOrder.size(); pointIdx++) {
    REQUIRE(shapesBank.getLine(line2Id).getIncludedPoints().at(pointIdx) == line2PointsOrder.at(pointIdx));
  }

  for(size_t pointIdx = 0; pointIdx < line3PointsOrder.size(); pointIdx++) {
    REQUIRE(shapesBank.getLine(line3Id).getIncludedPoints().at(pointIdx) == line3PointsOrder.at(pointIdx));
  }
}

TEST_CASE("Adding circle to ShapesBank", "[expertBackground]") {
  expertBackground::ShapesBank shapesBank;

  const std::string point1Id{"point 1 id"};
  const float point1CoordX{-4.0F};
  const float point1CoordY{1.0F};
  const std::string point1Name{"A"};

  const std::string point2Id{"point 2 id"};
  const float point2CoordX{-4.0F};
  const float point2CoordY{-1.0F};
  const std::string point2Name{"B"};

  const std::string point3Id{"point 3 id"};
  const float point3CoordX{-4.0F};
  const float point3CoordY{3.0F};
  const std::string point3Name{"C"};

  const std::string point4Id{"point 4 id"};
  const float point4CoordX{-2.0F};
  const float point4CoordY{1.0F};
  const std::string point4Name{"D"};

  const std::string point5Id{"point 5 id"};
  const float point5CoordX{-6.0F};
  const float point5CoordY{1.0F};
  const std::string point5Name{"E"};

  const std::string circleId{"circle id"};
  const std::string& circleCenterId{point1Id};
  const float circleCenterCoordX{point1CoordX};
  const float circleCenterCoordY{point1CoordY};
  const std::string& circleCenterName{point1Name};
  const float circleRadius{1};
  const std::vector<std::string> circlePoints{point4Id, point5Id, point2Id, point3Id};
  const std::vector<std::string> circlePointsOrder{point5Id, point2Id, point4Id, point3Id};
  const size_t circlePositionInCirclesVector{0};
  const json circleJson{
      {"object", {{"centerId", circleCenterId}, {"centerX", circleCenterCoordX}, {"centerY", circleCenterCoordY}, {"id", circleId}, {"pointsOn", json(circlePointsOrder)}, {"radius", circleRadius}}},
      {"position", 0}
  };

  const size_t pointsNumber{5};

  shapesBank.addPoint(point1Id, point1CoordX, point1CoordY, point1Name);
  shapesBank.addPoint(point2Id, point2CoordX, point2CoordY, point2Name);
  shapesBank.addPoint(point3Id, point3CoordX, point3CoordY, point3Name);
  shapesBank.addPoint(point4Id, point4CoordX, point4CoordY, point4Name);
  shapesBank.addPoint(point5Id, point5CoordX, point5CoordY, point5Name);

  shapesBank.addCircle(circleId, circleCenterId, circleCenterCoordX, circleCenterCoordY, circleCenterName, circleRadius, circlePoints);

  REQUIRE(shapesBank.getPointsNumber() == pointsNumber);

  REQUIRE(shapesBank.getCircle(circleId).getId() == circleId);
  REQUIRE(shapesBank.getCircle(circleId).getCenterId() == circleCenterId);
  REQUIRE_THAT(shapesBank.getCircle(circleId).getCenterX(), Catch::Matchers::WithinAbs(circleCenterCoordX, EPSILON));
  REQUIRE_THAT(shapesBank.getCircle(circleId).getCenterY(), Catch::Matchers::WithinAbs(circleCenterCoordY, EPSILON));
  REQUIRE(shapesBank.getCircle(circleId).getCenterName() == circleCenterName);
  REQUIRE_THAT(shapesBank.getCircle(circleId).getRadius(), Catch::Matchers::WithinAbs(circleRadius, EPSILON));
  REQUIRE(shapesBank.getCircle(circleId).getIncludedPoints().size() == circlePoints.size());
  REQUIRE(shapesBank.getCirclesNumber() == 1);
  REQUIRE(shapesBank.getCirclesVector().size() == 1);
  REQUIRE(shapesBank.getCirclesVector().at(0) == shapesBank.getCircle(circleId));
  REQUIRE(shapesBank.getCirclesAsJsonObjects().size() == 1);
  REQUIRE(shapesBank.getCirclesAsJsonObjects().at(0) == circleJson);
  REQUIRE(shapesBank.getCirclePositionInVector(circleId) == circlePositionInCirclesVector);
  REQUIRE(shapesBank.getCircleIdWithTwoPoints(circleCenterId, circlePoints.back()) == circleId);

  for(size_t pointIdx = 0; pointIdx < circlePointsOrder.size(); pointIdx++) {
    REQUIRE(shapesBank.getCircle(circleId).getIncludedPoints().at(pointIdx) == circlePointsOrder.at(pointIdx));
  }
}