#include "ExpertBackgroundTestBase.h"

TEST_CASE("Creating empty DependenciesBank", "[expertBackground]") {
  const DependenciesBank emptyDependenciesBank;

  json emptyDependenciesJson{};
  for(IDependency::Type type = IDependency::TypeBegin; type != IDependency::TypeEnd; ++type) {
    emptyDependenciesJson.push_back({{"dependencies", std::vector<IDependency::Type>()}, {"type", type}});
  }

  const json emptyVariablesIndexesJson{};

  const size_t equationId = 11;

  REQUIRE(emptyDependenciesBank.getDependenciesNumber() == 0);
  REQUIRE_THROWS(emptyDependenciesBank.getDependencyById(equationId));

  REQUIRE(emptyDependenciesBank.getDependenciesJson() == emptyDependenciesJson);
  REQUIRE(emptyDependenciesBank.getVariablesIndexesJson() == emptyVariablesIndexesJson);

  REQUIRE(emptyDependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::EQUATION).empty());
  REQUIRE(emptyDependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::SEGMENT_LENGTH).empty());
  REQUIRE(emptyDependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::ANGLE_MEASURE).empty());

  REQUIRE(emptyDependenciesBank.getAnglesDependencies(DependenciesBank::AnglesDependencies::EQUAL_ANGLES).empty());

  REQUIRE(emptyDependenciesBank.getCirclePolygonDependencies(DependenciesBank::CirclePolygonDependencies::INSCRIBED_CIRCLE).empty());
  REQUIRE(emptyDependenciesBank.getCirclePolygonDependencies(DependenciesBank::CirclePolygonDependencies::CIRCUMSCRIBED_CIRCLE).empty());
  REQUIRE(emptyDependenciesBank.getCirclePolygonDependencies(DependenciesBank::CirclePolygonDependencies::ESCRIBED_CIRCLE).empty());

  REQUIRE(emptyDependenciesBank.getCirclesDependencies(DependenciesBank::CirclesDependencies::TANGENT_CIRCLE_TO_CIRCLE).empty());

  REQUIRE(emptyDependenciesBank.getLineAngleDependencies(DependenciesBank::LineAngleDependencies::BISECTOR_LINE).empty());

  REQUIRE(emptyDependenciesBank.getLineCircleDependencies(DependenciesBank::LineCircleDependencies::TANGENT_LINE_TO_CIRCLE).empty());

  REQUIRE(emptyDependenciesBank.getLinePointsPairDependencies(DependenciesBank::LinePointsPairDependencies::MID_PERPENDICULAR_LINE).empty());

  REQUIRE(emptyDependenciesBank.getLinesDependencies(DependenciesBank::LinesDependencies::PERPENDICULAR_LINES).empty());
  REQUIRE(emptyDependenciesBank.getLinesDependencies(DependenciesBank::LinesDependencies::PARALLEL_LINES).empty());

  REQUIRE(emptyDependenciesBank.getPointsPairsDependencies(DependenciesBank::PointsPairsDependencies::EQUAL_SEGMENTS).empty());

  REQUIRE(emptyDependenciesBank.getPolygonDependencies(DependenciesBank::PolygonsDependencies::CONGRUENT_TRIANGLES).empty());
  REQUIRE(emptyDependenciesBank.getPolygonDependencies(DependenciesBank::PolygonsDependencies::SIMILAR_TRIANGLES).empty());

  REQUIRE(emptyDependenciesBank.getPolygonTypeDependencies(DependenciesBank::PolygonTypeDependencies::POLYGON_TYPE).empty());

  REQUIRE(emptyDependenciesBank.getPolygonPointsPairDependencies(DependenciesBank::PolygonPointsPairDependencies::MEDIAN).empty());
  REQUIRE(emptyDependenciesBank.getPolygonPointsPairDependencies(DependenciesBank::PolygonPointsPairDependencies::ALTITUDE).empty());
  REQUIRE(emptyDependenciesBank.getPolygonPointsPairDependencies(DependenciesBank::PolygonPointsPairDependencies::MID_SEGMENT).empty());

  REQUIRE(emptyDependenciesBank.getPolygonExpressionDependencies(DependenciesBank::PolygonExpressionDependencies::POLYGON_PERIMETER).empty());
  REQUIRE(emptyDependenciesBank.getPolygonExpressionDependencies(DependenciesBank::PolygonExpressionDependencies::POLYGON_AREA).empty());
}

TEST_CASE("Creating DependenciesBank based on ShapesBank", "[expertBackground]") {
  const std::string pointAId{"idA"};
  const std::string pointAName{"A"};
  const float pointACoords[2] = {-1.0F, 3.0F};

  const std::string pointBId{"idB"};
  const std::string pointBName{"B"};
  const float pointBCoords[2] = {-2.0F, 1.0F};

  const std::string pointAPrimId{"idA'"};
  const std::string pointAPrimName{"A'"};
  const float pointAPrimCoords[2] = {-5.0F, 7.0F};

  ShapesBank shapesBank;
  shapesBank.addPoint(pointAId, pointACoords[0], pointACoords[1], pointAName);
  shapesBank.addPoint(pointBId, pointBCoords[0], pointBCoords[1], pointBName);
  shapesBank.addPoint(pointAPrimId, pointAPrimCoords[0], pointAPrimCoords[1], pointAPrimName);

  constexpr size_t angleIsConvexMark{1};
  constexpr size_t angleIsConcaveMark{0};

  const size_t pointAIdPos{shapesBank.getPointPositionInVector(pointAId)};
  const size_t pointBIdPos{shapesBank.getPointPositionInVector(pointBId)};
  const size_t pointAPrimIdPos{shapesBank.getPointPositionInVector(pointAPrimId)};

  const json expectedVariables = {
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos}}, {"name", "|AA|"}},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointBIdPos}}, {"name", "|BB|"}},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointAPrimIdPos}}, {"name", "|A'A'|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos}}, {"name", "|AB|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos}}, {"name", "|AA'|"}},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAPrimIdPos}}, {"name", "|BA'|"}},

      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos, pointAIdPos, angleIsConvexMark}}, {"name", "|>AAA|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos, pointAIdPos, angleIsConcaveMark}}, {"name", "|<AAA|"}},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointBIdPos, pointBIdPos, angleIsConvexMark}}, {"name", "|>BBB|"}},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointBIdPos, pointBIdPos, angleIsConcaveMark}}, {"name", "|<BBB|"}},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointAPrimIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name", "|>A'A'A'|"}},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointAPrimIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name", "|<A'A'A'|"}},

      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name","|>AAA'|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name","|<AAA'|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos, pointAIdPos, angleIsConvexMark}}, {"name","|>AA'A|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos, pointAIdPos, angleIsConcaveMark}}, {"name","|<AA'A|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos, pointBIdPos, angleIsConvexMark}}, {"name","|>AAB|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAIdPos, pointBIdPos, angleIsConcaveMark}}, {"name","|<AAB|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos, pointAIdPos, angleIsConvexMark}}, {"name","|>ABA|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos, pointAIdPos, angleIsConcaveMark}}, {"name","|<ABA|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos, pointBIdPos, angleIsConvexMark}}, {"name","|>ABB|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos, pointBIdPos, angleIsConcaveMark}}, {"name","|<ABB|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAIdPos, pointBIdPos, angleIsConvexMark}}, {"name","|>BAB|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAIdPos, pointBIdPos, angleIsConcaveMark}}, {"name","|<BAB|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointBIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name","|>BBA'|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointBIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name","|<BBA'|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAPrimIdPos, pointBIdPos, angleIsConvexMark}}, {"name","|>BA'B|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAPrimIdPos, pointBIdPos, angleIsConcaveMark}}, {"name","|<BA'B|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name","|>AA'A'|" }},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name","|<AA'A'|" }},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointAIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name","|>A'AA'|" }},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointAIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name","|<A'AA'|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAPrimIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name","|>BA'A'|" }},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAPrimIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name","|<BA'A'|" }},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointBIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name","|>A'BA'|" }},
      {{ "ids", std::vector<size_t>{pointAPrimIdPos, pointBIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name","|<A'BA'|" }},

      {{ "ids", std::vector<size_t>{pointBIdPos, pointAIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name", "|>BAA'|"}},
      {{ "ids", std::vector<size_t>{pointBIdPos, pointAIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name", "|<BAA'|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos, pointAPrimIdPos, angleIsConvexMark}}, {"name", "|>ABA'|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointBIdPos, pointAPrimIdPos, angleIsConcaveMark}}, {"name", "|<ABA'|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos, pointBIdPos, angleIsConvexMark}}, {"name", "|>AA'B|"}},
      {{ "ids", std::vector<size_t>{pointAIdPos, pointAPrimIdPos, pointBIdPos, angleIsConcaveMark}}, {"name", "|<AA'B|"}}
  };

  const std::vector<std::vector<ExpressionModel>> expectedLengths = {
      {ExpressionModel {"|AA|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|BB|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|A'A'|"}, ExpressionModel {"0"}}
  };

  const std::vector<std::vector<ExpressionModel>> expectedMeasures1 = {
      {ExpressionModel {"|>AAA|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<AAA|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>BBB|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<BBB|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>A'A'A'|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<A'A'A'|"}, ExpressionModel {"360"}}
  };

  const std::vector<std::vector<ExpressionModel>> expectedMeasures2 = {
      {ExpressionModel {"|>BAB|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<BAB|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>A'AA'|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<A'AA'|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>ABA|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<ABA|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>A'BA'|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<A'BA'|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>AA'A|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<AA'A|"}, ExpressionModel {"360"}},
      {ExpressionModel {"|>BA'B|"}, ExpressionModel {"0"}},
      {ExpressionModel {"|<BA'B|"}, ExpressionModel {"360"}},
  };

  const DependenciesBank dependenciesBank{&shapesBank};

  const json variables = dependenciesBank.getVariablesIndexesJson();

  const std::vector<std::shared_ptr<EquationDependency>> equations =
    dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::EQUATION);

  const std::vector<std::shared_ptr<EquationDependency>> lengths =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::SEGMENT_LENGTH);

  const std::vector<std::shared_ptr<EquationDependency>> measures =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::ANGLE_MEASURE);

  const std::vector<std::vector<ExpressionModel>> expectedEquations = { };

  REQUIRE(variables.size() == expectedVariables.size());
  for (const auto& var: variables) {
    bool found = false;

    for (const auto& expVar: expectedVariables) {
      if(var["name"] != expVar["name"]) {
        continue;
      }

      bool equalVectors = true;
      for (size_t i = 0; i < expVar["ids"].size(); i++) {
        if(var["ids"][i] != expVar["ids"][i]) {
          equalVectors = false;
          break;
        }
      }

      found = equalVectors;
    }

    REQUIRE(found);
  }

  REQUIRE(dependenciesBank.getDependenciesNumber() ==
          (expectedEquations.size() + expectedLengths.size() + expectedMeasures1.size() + expectedMeasures2.size()));

  REQUIRE(equations.size() == expectedEquations.size());

  REQUIRE(lengths.size() == expectedLengths.size());
  for(const auto& equ: lengths) {
    bool found = false;

    for(const auto& expEqu: expectedLengths) {
      if(get<0>(equ->getArgs()) == expEqu[0] && get<1>(equ->getArgs()) == expEqu[1] && equ->getDependentDependencies().empty() &&
          equ->getImportance() == IDependency::Importance::LOW && equ->getCategory() == IDependency::Category::FORMULA &&
          equ->getType() == IDependency::Type::SEGMENT_LENGTH && equ->getReason() == IDependency::Reason::POINTS_ARE_THE_SAME) {
            found = true;
      }
    }

    REQUIRE(found);
  }

  REQUIRE(measures.size() == (expectedMeasures1.size() + expectedMeasures2.size()));
  for(const auto& equ: measures) {
    bool found = false;

    for(const auto& expEqu: expectedMeasures1) {
      if(get<0>(equ->getArgs()) == expEqu[0] && get<1>(equ->getArgs()) == expEqu[1] && equ->getDependentDependencies().empty() &&
          equ->getImportance() == IDependency::Importance::LOW && equ->getCategory() == IDependency::Category::FORMULA &&
          equ->getType() == IDependency::Type::ANGLE_MEASURE && equ->getReason() == IDependency::Reason::POINTS_ARE_THE_SAME) {
            found = true;
      }
    }

    for(const auto& expEqu: expectedMeasures2) {
      if(get<0>(equ->getArgs()) == expEqu[0] && get<1>(equ->getArgs()) == expEqu[1] && equ->getDependentDependencies().empty() &&
          equ->getImportance() == IDependency::Importance::LOW && equ->getCategory() == IDependency::Category::FORMULA &&
          equ->getType() == IDependency::Type::ANGLE_MEASURE && equ->getReason() == IDependency::Reason::ARMS_ARE_THE_SAME) {
            found = true;
      }
    }

    REQUIRE(found);
  }
}
