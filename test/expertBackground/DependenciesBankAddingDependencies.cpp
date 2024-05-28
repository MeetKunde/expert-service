#include "ExpertBackgroundTestBase.h"

TEST_CASE("Adding EquationDependencies to DependenciesBank", "[expertBackground]") {
  const PointModel pointA{"pointA", 6.0F, -1.0F, "A"};
  const PointModel pointB{"pointB", 8.0F, 1.0F, "B"};
  const PointModel pointC{"pointC", 9.0F, -4.0F, "C"};
  const PointModel pointD{"pointD", 1.0F, 0.0F, "D"};

  const std::vector<PointModel> points = {pointA, pointB, pointC, pointD};

  const LineModel lineK{"line k", LineModel::Type::SLANTED, 1.0F, -7.0, std::vector<std::string>{pointA.getId(), pointB.getId()}};

  const LineModel lineL{"line l", LineModel::Type::SLANTED, -1.0F, 5.0, std::vector<std::string>{pointA.getId(), pointC.getId()}};

  const std::vector<LineModel> lines = {lineK, lineL};

  const DependenciesBank::EquationDependencies type1{DependenciesBank::EquationDependencies::EQUATION};
  const DependenciesBank::EquationDependencies type2{DependenciesBank::EquationDependencies::SEGMENT_LENGTH};
  const DependenciesBank::EquationDependencies type3{DependenciesBank::EquationDependencies::ANGLE_MEASURE};

  const IDependency::Reason reason1{IDependency::Reason::EQUATION_EXTRACTION};
  const IDependency::Reason reason2{IDependency::Reason::USER_DEFINED};

  const std::vector<size_t> basedOn1 = {11, 13};
  const std::vector<size_t> basedOn2 = {1, 91, 0, 3};

  const IDependency::Importance Importance1{IDependency::Importance::LOW};
  const IDependency::Importance Importance2{IDependency::Importance::MEDIUM};

  const ExpressionModel expressionWithExternalVar1{"y + z + 6"};
  const ExpressionModel expressionWithExternalVar2{"x^2 + 3*x + 1"};
  const ExpressionModel expressionWithExternalVar3{"a * b"};
  const ExpressionModel expressionLenVar{"|AB|"};
  const ExpressionModel expressionConvexAngleVar{"|>ABC|"};
  const ExpressionModel expressionConcaveAngleVar{"|<ABC|"};

  ShapesBank shapesBank{};

  for (const auto& point : points) {
    shapesBank.addPoint(point.getId(), point.getX(), point.getY(), point.getName());
  }

  for (const auto& line : lines) {
    shapesBank.addLine(line.getId(), line.getType(), line.getCoeffA(), line.getCoeffB(), line.getIncludedPoints());
  }

  DependenciesBank dependenciesBank{&shapesBank};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t baseEquationsNumber =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::EQUATION).size();
  const size_t baseLengthsNumber =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::SEGMENT_LENGTH).size();
  const size_t baseMeasuresNumber =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::ANGLE_MEASURE).size();

  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};

  const json dependency1Json = {{"arg1",
                                 {
                                     {"value", "x^2+3*x+1"},
                                     {"variables", std::vector<std::string>{"x"}},
                                 }},
                                {"arg2",
                                 {
                                     {"value", "y+z+6"},
                                     {"variables", std::vector<std::string>{"y", "z"}},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::FORMULA},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency2Json = {{"arg1",
                                 {
                                     {"value", "y+z+6"},
                                     {"variables", std::vector<std::string>{"y", "z"}},
                                 }},
                                {"arg2", {{"value", "|AB|"}, {"variables", std::vector<std::string>{"|AB|"}}}},
                                {"id", dependency2Id},
                                {"category", IDependency::Category::FORMULA},
                                {"type", type2},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency3Json = {{"arg1",
                                 {
                                     {"value", "a*b"},
                                     {"variables", std::vector<std::string>{"a", "b"}},
                                 }},
                                {"arg2", {{"value", "|<ABC|"}, {"variables", std::vector<std::string>{"|<ABC|"}}}},
                                {"id", dependency4Id},
                                {"category", IDependency::Category::FORMULA},
                                {"type", type3},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addEquation(expressionWithExternalVar1, expressionWithExternalVar2, reason1, basedOn1, Importance1) ==
          1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency
  REQUIRE(dependenciesBank.addLength(pointA.getId(), pointB.getId(), expressionWithExternalVar1, reason1, basedOn1,
                                     Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency
  REQUIRE(dependenciesBank.addConvexAngle(pointC.getId(), pointB.getId(), pointA.getId(), expressionWithExternalVar2, reason1,
                                          basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency
  REQUIRE(dependenciesBank.addConcaveAngle(pointC.getId(), pointB.getId(), pointA.getId(), expressionWithExternalVar3, reason1,
                                           basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other method
  REQUIRE(dependenciesBank.addEquation(expressionLenVar, expressionWithExternalVar1, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other method
  REQUIRE(dependenciesBank.addEquation(expressionConvexAngleVar, expressionWithExternalVar2, reason1, basedOn1, Importance1) ==
          0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other method
  REQUIRE(dependenciesBank.addEquation(expressionConcaveAngleVar, expressionWithExternalVar3, reason1, basedOn1, Importance1) ==
          0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addEquation(expressionWithExternalVar1, expressionWithExternalVar2, reason1, basedOn2, Importance2) ==
          0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other expressions order
  REQUIRE(dependenciesBank.addEquation(expressionWithExternalVar2, expressionWithExternalVar1, reason1, basedOn1, Importance1) ==
          0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other first expression
  REQUIRE(dependenciesBank.addEquation(expressionWithExternalVar3, expressionWithExternalVar2, reason1, basedOn1, Importance1) ==
          1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  const std::vector<std::shared_ptr<EquationDependency>> equations =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::EQUATION);

  const std::vector<std::shared_ptr<EquationDependency>> lengths =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::SEGMENT_LENGTH);

  const std::vector<std::shared_ptr<EquationDependency>> measures =
      dependenciesBank.getEquationDependencies(DependenciesBank::EquationDependencies::ANGLE_MEASURE);

  REQUIRE(equations.size() == (baseEquationsNumber + 2));
  REQUIRE(lengths.size() == (baseLengthsNumber + 1));
  REQUIRE(measures.size() == (baseMeasuresNumber + 2));

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == equations.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency5Id)->getId() == equations.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency2Id)->getId() == lengths.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == measures.back()->getId());

  REQUIRE(equations.front()->getCategory() == IDependency::Category::FORMULA);
  REQUIRE(equations.front()->getType() == IDependency::Type::EQUATION);
  REQUIRE(equations.front()->getReason() == reason1);
  REQUIRE(equations.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(equations.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(equations.front()->getImportance() == Importance1);
  REQUIRE(equations.front()->getJson() == dependency1Json);

  REQUIRE(lengths.back()->getCategory() == IDependency::Category::FORMULA);
  REQUIRE(lengths.back()->getType() == IDependency::Type::SEGMENT_LENGTH);
  REQUIRE(lengths.back()->getReason() == reason1);
  REQUIRE(lengths.back()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(lengths.back()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(lengths.back()->getImportance() == Importance1);
  REQUIRE(lengths.back()->getJson() == dependency2Json);

  REQUIRE(measures.back()->getCategory() == IDependency::Category::FORMULA);
  REQUIRE(measures.back()->getType() == IDependency::Type::ANGLE_MEASURE);
  REQUIRE(measures.back()->getReason() == reason1);
  REQUIRE(measures.back()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(measures.back()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(measures.back()->getImportance() == Importance1);
  REQUIRE(measures.back()->getJson() == dependency3Json);
}

TEST_CASE("Adding LinesDependencies to DependenciesBank", "[expertBackground]") {
  const std::string lineA{"lineA"};
  const std::string lineB{"lineB"};
  const std::string lineC{"lineC"};

  const DependenciesBank::LinesDependencies type1{DependenciesBank::LinesDependencies::PERPENDICULAR_LINES};
  const DependenciesBank::LinesDependencies type2{DependenciesBank::LinesDependencies::PARALLEL_LINES};

  const IDependency::Reason reason1{IDependency::Reason::PERPENDICULAR_LINES};
  const IDependency::Reason reason2{IDependency::Reason::MID_SEGMENT};

  const std::vector<size_t> basedOn1 = {1, 3};
  const std::vector<size_t> basedOn2 = {11, 9};

  const IDependency::Importance Importance1{IDependency::Importance::HIGH};
  const IDependency::Importance Importance2{IDependency::Importance::MEDIUM};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};

  const json dependency1Json = {{"object1", {{"id", lineA}}},
                                {"object2", {{"id", lineB}}},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_LINES},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency4Json = {{"object1", {{"id", lineA}}},
                                {"object2", {{"id", lineB}}},
                                {"id", dependency4Id},
                                {"category", IDependency::Category::OF_LINES},
                                {"type", type2},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addLinesDependency(lineA, lineB, true, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addLinesDependency(lineA, lineB, true, type1, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other lines order
  REQUIRE(dependenciesBank.addLinesDependency(lineB, lineA, true, type1, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but with other
  // dependent dependencies and other importance
  REQUIRE(dependenciesBank.addLinesDependency(lineA, lineB, true, type1, reason1, basedOn2, Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addLinesDependency(lineA, lineB, true, type1, reason2, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other second line
  REQUIRE(dependenciesBank.addLinesDependency(lineA, lineC, true, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other type
  REQUIRE(dependenciesBank.addLinesDependency(lineA, lineB, true, type2, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  const std::vector<std::shared_ptr<LinesDependency>> perpendiculars =
      dependenciesBank.getLinesDependencies(DependenciesBank::LinesDependencies::PERPENDICULAR_LINES);

  const std::vector<std::shared_ptr<LinesDependency>> parallels =
      dependenciesBank.getLinesDependencies(DependenciesBank::LinesDependencies::PARALLEL_LINES);

  REQUIRE(perpendiculars.size() == 3);
  REQUIRE(parallels.size() == 1);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == perpendiculars.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency3Id)->getId() == perpendiculars.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == parallels.front()->getId());

  REQUIRE(perpendiculars.front()->getCategory() == IDependency::Category::OF_LINES);
  REQUIRE(perpendiculars.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(perpendiculars.front()->getReason() == reason1);
  REQUIRE(perpendiculars.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(perpendiculars.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(perpendiculars.front()->getImportance() == Importance1);
  REQUIRE(perpendiculars.front()->getJson() == dependency1Json);

  REQUIRE(parallels.front()->getCategory() == IDependency::Category::OF_LINES);
  REQUIRE(parallels.front()->getType() == static_cast<IDependency::Type>(type2));
  REQUIRE(parallels.front()->getReason() == reason1);
  REQUIRE(parallels.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(parallels.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(parallels.front()->getImportance() == Importance1);
  REQUIRE(parallels.front()->getJson() == dependency4Json);
}

TEST_CASE("Adding CirclesDependencies to DependenciesBank", "[expertBackground]") {
  const std::string circleA{"circleA"};
  const std::string circleB{"circleB"};
  const std::string circleC{"circleC"};

  const DependenciesBank::CirclesDependencies type{DependenciesBank::CirclesDependencies::TANGENT_CIRCLE_TO_CIRCLE};

  const IDependency::Reason reason1{IDependency::Reason::ISOSCELES_RIGHT_TRIANGLE};
  const IDependency::Reason reason2{IDependency::Reason::ANGLE_SIDE_ANGLE};

  const std::vector<size_t> basedOn1 = {0, 5};
  const std::vector<size_t> basedOn2 = {9, 0, 9};

  const IDependency::Importance Importance1{IDependency::Importance::LOW};
  const IDependency::Importance Importance2{IDependency::Importance::MEDIUM};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};

  const json dependency1Json = {{"object1", {{"id", circleA}}},
                                {"object2", {{"id", circleB}}},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_CIRCLES},
                                {"type", type},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addCirclesDependency(circleA, circleB, true, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addCirclesDependency(circleA, circleB, true, type, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other circles order
  REQUIRE(dependenciesBank.addCirclesDependency(circleB, circleA, true, type, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but with other
  // dependent dependencies and other importance
  REQUIRE(dependenciesBank.addCirclesDependency(circleA, circleB, true, type, reason1, basedOn2, Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addCirclesDependency(circleA, circleB, true, type, reason2, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other second line
  REQUIRE(dependenciesBank.addCirclesDependency(circleA, circleC, true, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  const std::vector<std::shared_ptr<CirclesDependency>> tangentCircles =
      dependenciesBank.getCirclesDependencies(DependenciesBank::CirclesDependencies::TANGENT_CIRCLE_TO_CIRCLE);

  REQUIRE(tangentCircles.size() == 3);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == tangentCircles.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency3Id)->getId() == tangentCircles.back()->getId());

  REQUIRE(tangentCircles.front()->getCategory() == IDependency::Category::OF_CIRCLES);
  REQUIRE(tangentCircles.front()->getType() == static_cast<IDependency::Type>(type));
  REQUIRE(tangentCircles.front()->getReason() == reason1);
  REQUIRE(tangentCircles.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(tangentCircles.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(tangentCircles.front()->getImportance() == Importance1);
  REQUIRE(tangentCircles.front()->getJson() == dependency1Json);
}

TEST_CASE("Adding LineCircleDependencies to DependenciesBank", "[expertBackground]") {
  const std::string lineA{"lineA"};
  const std::string lineB{"lineB"};
  const std::string circleA{"circleA"};
  const std::string circleB{"circleB"};
  const std::string& swipedLineAToCircleA{circleA};
  const std::string& swipedCircleAToLineA{lineA};

  const DependenciesBank::LineCircleDependencies type{DependenciesBank::LineCircleDependencies::TANGENT_LINE_TO_CIRCLE};

  const IDependency::Reason reason1{IDependency::Reason::PARALLEL_LINES};
  const IDependency::Reason reason2{IDependency::Reason::ALTITUDE};

  const std::vector<size_t> basedOn1 = {17, 1};
  const std::vector<size_t> basedOn2 = {1, 6};

  const IDependency::Importance Importance1{IDependency::Importance::LOW};
  const IDependency::Importance Importance2{IDependency::Importance::MEDIUM};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};

  const json dependency1Json = {{"object1", {{"id", lineA}}},
                                {"object2", {{"id", circleA}}},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_LINE_AND_CIRCLE},
                                {"type", type},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addLineCircleDependency(lineA, circleA, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addLineCircleDependency(lineA, circleA, type, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other line
  REQUIRE(dependenciesBank.addLineCircleDependency(lineB, circleA, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other circle
  REQUIRE(dependenciesBank.addLineCircleDependency(lineA, circleB, type, reason1, basedOn2, Importance2) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // attempt to add the same dependency but with other
  // dependent dependencies and other importance
  REQUIRE(dependenciesBank.addLineCircleDependency(lineA, circleA, type, reason1, basedOn2, Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addLineCircleDependency(lineA, circleA, type, reason2, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // new dependency = swiped line and circle
  REQUIRE(dependenciesBank.addLineCircleDependency(swipedLineAToCircleA, swipedCircleAToLineA, type, reason1, basedOn1,
                                                   Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  const std::vector<std::shared_ptr<LinesDependency>> tangentLines =
      dependenciesBank.getLineCircleDependencies(DependenciesBank::LineCircleDependencies::TANGENT_LINE_TO_CIRCLE);

  REQUIRE(tangentLines.size() == 5);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == tangentLines.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency5Id)->getId() == tangentLines.back()->getId());

  REQUIRE(tangentLines.front()->getCategory() == IDependency::Category::OF_LINE_AND_CIRCLE);
  REQUIRE(tangentLines.front()->getType() == static_cast<IDependency::Type>(type));
  REQUIRE(tangentLines.front()->getReason() == reason1);
  REQUIRE(tangentLines.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(tangentLines.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(tangentLines.front()->getImportance() == Importance1);
  REQUIRE(tangentLines.front()->getJson() == dependency1Json);
}

TEST_CASE("Adding CirclePolygonDependencies to DependenciesBank", "[expertBackground]") {
  const std::string circleA{"circleA"};
  const std::string circleB{"circleB"};

  const std::vector<std::string> polygonA = {"vertex1", "vertex2", "vertex3", "vertex4", "vertex5"};
  const std::vector<std::string> polygonB = {"vertex6", "vertex7", "vertex8", "vertex6"};

  const DependenciesBank::CirclePolygonDependencies type1{DependenciesBank::CirclePolygonDependencies::ESCRIBED_CIRCLE};
  const DependenciesBank::CirclePolygonDependencies type2{DependenciesBank::CirclePolygonDependencies::INSCRIBED_CIRCLE};

  const IDependency::Reason reason1{IDependency::Reason::FUNDAMENTAL_THEORY_OF_GEOMETRY};
  const IDependency::Reason reason2{IDependency::Reason::ESCRIBED_CIRCLE};

  const std::vector<size_t> basedOn1 = {14, 22};
  const std::vector<size_t> basedOn2 = {3, 81};

  const IDependency::Importance Importance1{IDependency::Importance::HIGH};
  const IDependency::Importance Importance2{IDependency::Importance::MEDIUM};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};

  const json dependency1Json = {{"object1",
                                 {
                                     {"id", circleA},
                                 }},
                                {"object2",
                                 {
                                     {"verticesIds", polygonA},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_CIRCLE_AND_POLYGON},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency5Json = {{"object1",
                                 {
                                     {"id", circleA},
                                 }},
                                {"object2",
                                 {
                                     {"verticesIds", polygonA},
                                 }},
                                {"id", dependency5Id},
                                {"category", IDependency::Category::OF_CIRCLE_AND_POLYGON},
                                {"type", type2},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleA, polygonA, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleA, polygonA, type1, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other circle
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleB, polygonA, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency other polygon
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleA, polygonB, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleA, polygonA, type1, reason1, basedOn2, Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleA, polygonA, type1, reason2, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // new dependency = other type
  REQUIRE(dependenciesBank.addCirclePolygonDependency(circleA, polygonA, type2, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  const std::vector<std::shared_ptr<CirclePolygonDependency>> escribedCircles =
      dependenciesBank.getCirclePolygonDependencies(DependenciesBank::CirclePolygonDependencies::ESCRIBED_CIRCLE);

  const std::vector<std::shared_ptr<CirclePolygonDependency>> inscribedCircles =
      dependenciesBank.getCirclePolygonDependencies(DependenciesBank::CirclePolygonDependencies::INSCRIBED_CIRCLE);

  REQUIRE(escribedCircles.size() == 4);
  REQUIRE(inscribedCircles.size() == 1);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == escribedCircles.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == escribedCircles.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency5Id)->getId() == inscribedCircles.front()->getId());

  REQUIRE(escribedCircles.front()->getCategory() == IDependency::Category::OF_CIRCLE_AND_POLYGON);
  REQUIRE(escribedCircles.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(escribedCircles.front()->getReason() == reason1);
  REQUIRE(escribedCircles.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(escribedCircles.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(escribedCircles.front()->getImportance() == Importance1);
  REQUIRE(escribedCircles.front()->getJson() == dependency1Json);

  REQUIRE(inscribedCircles.front()->getCategory() == IDependency::Category::OF_CIRCLE_AND_POLYGON);
  REQUIRE(inscribedCircles.front()->getType() == static_cast<IDependency::Type>(type2));
  REQUIRE(inscribedCircles.front()->getReason() == reason1);
  REQUIRE(inscribedCircles.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(inscribedCircles.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(inscribedCircles.front()->getImportance() == Importance1);
  REQUIRE(inscribedCircles.front()->getJson() == dependency5Json);
}

TEST_CASE("Adding PointsPairsDependencies to DependenciesBank", "[expertBackground]") {
  const std::string pointA{"pointA"};
  const std::string pointB{"pointB"};
  const std::string pointC{"pointC"};
  const std::string pointD{"pointD"};
  const std::string pointE{"pointE"};
  const std::string pointF{"pointF"};

  const DependenciesBank::PointsPairsDependencies type1{DependenciesBank::PointsPairsDependencies::EQUAL_SEGMENTS};

  const IDependency::Reason reason1{IDependency::Reason::ARMS_ARE_THE_SAME};
  const IDependency::Reason reason2{IDependency::Reason::BISECTOR};

  const std::vector<size_t> basedOn1 = {4, 2};
  const std::vector<size_t> basedOn2 = {43, 8};

  const IDependency::Importance Importance1{IDependency::Importance::MEDIUM};
  const IDependency::Importance Importance2{IDependency::Importance::LOW};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};

  const json dependency1Json = {{"object1", {{"end1Id", pointA}, {"end2Id", pointB}}},
                                {"object2", {{"end1Id", pointC}, {"end2Id", pointD}}},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_POINTS_PAIRS},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointA, pointB, pointC, pointD, true, type1, reason1, basedOn1,
                                                    Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointA, pointB, pointC, pointD, true, type1, reason1, basedOn1,
                                                    Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other PointsPairModels order
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointD, pointC, pointB, pointA, true, type1, reason1, basedOn1,
                                                    Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other points order in first PointsPairModel
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointB, pointA, pointC, pointD, true, type1, reason1, basedOn1,
                                                    Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other points order in second PointsPairModel
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointA, pointB, pointD, pointC, true, type1, reason1, basedOn1,
                                                    Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but with other
  // dependent dependencies and other importance
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointA, pointB, pointC, pointD, true, type1, reason1, basedOn2,
                                                    Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointA, pointB, pointC, pointD, true, type1, reason2, basedOn1,
                                                    Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other second line
  REQUIRE(dependenciesBank.addPointsPairsDependency(pointA, pointB, pointE, pointF, true, type1, reason1, basedOn1,
                                                    Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  const std::vector<std::shared_ptr<PointsPairsDependency>> equalSegments =
      dependenciesBank.getPointsPairsDependencies(DependenciesBank::PointsPairsDependencies::EQUAL_SEGMENTS);

  REQUIRE(equalSegments.size() == 3);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == equalSegments.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency3Id)->getId() == equalSegments.back()->getId());

  REQUIRE(equalSegments.front()->getCategory() == IDependency::Category::OF_POINTS_PAIRS);
  REQUIRE(equalSegments.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(equalSegments.front()->getReason() == reason1);
  REQUIRE(equalSegments.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(equalSegments.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(equalSegments.front()->getImportance() == Importance1);
  REQUIRE(equalSegments.front()->getJson() == dependency1Json);
}

TEST_CASE("Adding AnglesDependencies to DependenciesBank", "[expertBackground]") {
  const PointModel pointA{"pointA", 6.0F, -1.0F, "A"};
  const PointModel pointB{"pointB", 8.0F, 1.0F, "B"};
  const PointModel pointC{"pointC", 9.0F, -4.0F, "C"};
  const PointModel pointD{"pointD", 4.0F, 1.0F, "D"};
  const PointModel pointE{"pointE", 2.0F, -5.0F, "E"};
  const PointModel pointF{"pointF", 1.0F, 4.0F, "F"};
  const PointModel pointG{"pointG", 0.0F, -7.0F, "G"};
  const PointModel pointH{"pointH", 1.0F, 1.0F, "H"};

  const std::vector<PointModel> points = {pointA, pointB, pointC, pointD, pointE, pointF, pointG, pointH};

  const LineModel lineK{"line k", LineModel::Type::SLANTED, 1.0F, -7.0,
                        std::vector<std::string>{pointA.getId(), pointB.getId(), pointE.getId(), pointG.getId()}};

  const LineModel lineL{"line l", LineModel::Type::SLANTED, -1.0F, 5.0,
                        std::vector<std::string>{pointA.getId(), pointC.getId(), pointD.getId(), pointF.getId()}};

  const std::vector<LineModel> lines = {lineK, lineL};

  const AngleModel::Type convexAngleType{AngleModel::Type::CONVEX};
  const AngleModel::Type concaveAngleType{AngleModel::Type::CONCAVE};
  const AngleModel::Type unknownAngleType{AngleModel::Type::UNKNOWN};

  const DependenciesBank::AnglesDependencies type{DependenciesBank::AnglesDependencies::EQUAL_ANGLES};

  const IDependency::Reason reason1{IDependency::Reason::ALTERNATE_ANGLES};
  const IDependency::Reason reason2{IDependency::Reason::CORRESPONDING_ANGLES};

  const std::vector<size_t> basedOn1 = {14, 2, 9};
  const std::vector<size_t> basedOn2 = {2, 8, 3};

  const IDependency::Importance Importance1{IDependency::Importance::HIGH};
  const IDependency::Importance Importance2{IDependency::Importance::LOW};

  ShapesBank shapesBank{};

  for (const auto& point : points) {
    shapesBank.addPoint(point.getId(), point.getX(), point.getY(), point.getName());
  }

  for (const auto& line : lines) {
    shapesBank.addLine(line.getId(), line.getType(), line.getCoeffA(), line.getCoeffB(), line.getIncludedPoints());
  }

  DependenciesBank dependenciesBank{&shapesBank};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};

  const json dependency1Json = {{"object1",
                                 {
                                     {"point1Id", pointB.getId()},
                                     {"vertexId", pointA.getId()},
                                     {"point2Id", pointC.getId()},
                                     {"type", convexAngleType},
                                 }},
                                {"object2",
                                 {
                                     {"point1Id", pointF.getId()},
                                     {"vertexId", pointA.getId()},
                                     {"point2Id", pointG.getId()},
                                     {"type", convexAngleType},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_ANGLES},
                                {"type", type},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), convexAngleType, pointF.getId(),
                                               pointA.getId(), pointG.getId(), convexAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), convexAngleType, pointF.getId(),
                                               pointA.getId(), pointG.getId(), convexAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other AngleModels order
  REQUIRE(dependenciesBank.addAnglesDependency(pointF.getId(), pointA.getId(), pointG.getId(), convexAngleType, pointB.getId(),
                                               pointA.getId(), pointC.getId(), convexAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other points order in first and second AngleModel
  REQUIRE(dependenciesBank.addAnglesDependency(pointC.getId(), pointA.getId(), pointB.getId(), convexAngleType, pointG.getId(),
                                               pointA.getId(), pointF.getId(), convexAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  // but in other points on angle arms
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), convexAngleType, pointD.getId(),
                                               pointA.getId(), pointE.getId(), convexAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other angle types
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), concaveAngleType, pointD.getId(),
                                               pointA.getId(), pointE.getId(), concaveAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // attempt to add the same dependency but with other
  // dependent dependencies and other importance
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), convexAngleType, pointD.getId(),
                                               pointA.getId(), pointE.getId(), convexAngleType, true, type, reason1, basedOn2,
                                               Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), convexAngleType, pointD.getId(),
                                               pointA.getId(), pointE.getId(), convexAngleType, true, type, reason2, basedOn1,
                                               Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other second vertex of second AngleModel
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), convexAngleType, pointD.getId(),
                                               pointH.getId(), pointE.getId(), convexAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // attempt to add the same dependency
  // but with other angle type(unknown is compatible with convex and concave(
  REQUIRE(dependenciesBank.addAnglesDependency(pointB.getId(), pointA.getId(), pointC.getId(), unknownAngleType, pointD.getId(),
                                               pointA.getId(), pointE.getId(), unknownAngleType, true, type, reason1, basedOn1,
                                               Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  const std::vector<std::shared_ptr<AnglesDependency>> equalAngles =
      dependenciesBank.getAnglesDependencies(DependenciesBank::AnglesDependencies::EQUAL_ANGLES);

  REQUIRE(equalAngles.size() == 4);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == equalAngles.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == equalAngles.back()->getId());

  REQUIRE(equalAngles.front()->getCategory() == IDependency::Category::OF_ANGLES);
  REQUIRE(equalAngles.front()->getType() == static_cast<IDependency::Type>(type));
  REQUIRE(equalAngles.front()->getReason() == reason1);
  REQUIRE(equalAngles.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(equalAngles.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(equalAngles.front()->getImportance() == Importance1);
  REQUIRE(equalAngles.front()->getJson() == dependency1Json);
}

TEST_CASE("Adding PolygonsDependencies to DependenciesBank", "[expertBackground]") {
  const std::vector<std::string> triangleABC{"pointA", "pointB", "pointC"};
  const std::vector<std::string> triangleBCA{"pointB", "pointC", "pointA"};
  const std::vector<std::string> triangleCAB{"pointC", "pointA", "pointB"};
  const std::vector<std::string> triangleCDE{"pointC", "pointD", "pointE"};

  const std::vector<std::string> rectangleABCD{"pointA", "pointB", "pointC", "pointD"};
  const std::vector<std::string> rectangleDCBA{"pointD", "pointC", "pointB", "pointA"};
  const std::vector<std::string> rectangleBADC{"pointB", "pointA", "pointD", "pointC"};
  const std::vector<std::string> rectangleACBD{"pointA", "pointC", "pointB", "pointD"};

  const bool fixedPoints1Order = false;
  const bool fixedPoints2Order = true;

  const DependenciesBank::PolygonsDependencies type1{DependenciesBank::PolygonsDependencies::SIMILAR_TRIANGLES};
  const DependenciesBank::PolygonsDependencies type2{DependenciesBank::PolygonsDependencies::CONGRUENT_TRIANGLES};

  const IDependency::Reason reason1{IDependency::Reason::ANGLE_SIDE_ANGLE};
  const IDependency::Reason reason2{IDependency::Reason::SIDE_ANGLE_SIDE};

  const std::vector<size_t> basedOn1 = {12, 4, 9};
  const std::vector<size_t> basedOn2 = {3, 1, 22};

  const IDependency::Importance Importance1{IDependency::Importance::HIGH};
  const IDependency::Importance Importance2{IDependency::Importance::LOW};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};

  const json dependency1Json = {{"object1",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"object2",
                                 {
                                     {"verticesIds", rectangleABCD},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_POLYGONS},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency5Json = {{"object1",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"object2",
                                 {
                                     {"verticesIds", rectangleABCD},
                                 }},
                                {"id", dependency5Id},
                                {"category", IDependency::Category::OF_POLYGONS},
                                {"type", type2},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // add new dependency
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // add the same dependency
  // but with other polygons order
  REQUIRE(dependenciesBank.addPolygonsDependency(rectangleABCD, fixedPoints1Order, triangleABC, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // add the same dependency
  // but with other first polygon vertices permutation
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleBCA, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // add the same dependency
  // but with other first polygon vertices permutation
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleCAB, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // add the same dependency
  // but with other second polygon vertices permutation
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleBADC, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // add the same dependency
  // but with other second polygon vertices permutation
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleDCBA, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other second polygon vertices permutation
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleACBD, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type1, reason1, basedOn2,
                                                 Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type1, reason2, basedOn1,
                                                 Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other second polygon
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, triangleCDE, fixedPoints2Order, true, type1, reason1, basedOn1,
                                                 Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // new dependency = other type
  REQUIRE(dependenciesBank.addPolygonsDependency(triangleABC, fixedPoints1Order, rectangleABCD, fixedPoints2Order, true, type2, reason1, basedOn1,
                                                 Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  const std::vector<std::shared_ptr<PolygonsDependency>> similarTriangles =
      dependenciesBank.getPolygonDependencies(DependenciesBank::PolygonsDependencies::SIMILAR_TRIANGLES);

  const std::vector<std::shared_ptr<PolygonsDependency>> congruentTriangles =
      dependenciesBank.getPolygonDependencies(DependenciesBank::PolygonsDependencies::CONGRUENT_TRIANGLES);

  REQUIRE(similarTriangles.size() == 4);
  REQUIRE(congruentTriangles.size() == 1);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == similarTriangles.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == similarTriangles.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency5Id)->getId() == congruentTriangles.front()->getId());

  REQUIRE(similarTriangles.front()->getCategory() == IDependency::Category::OF_POLYGONS);
  REQUIRE(similarTriangles.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(similarTriangles.front()->getReason() == reason1);
  REQUIRE(similarTriangles.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(similarTriangles.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(similarTriangles.front()->getImportance() == Importance1);
  REQUIRE(similarTriangles.front()->getJson() == dependency1Json);

  REQUIRE(congruentTriangles.front()->getCategory() == IDependency::Category::OF_POLYGONS);
  REQUIRE(congruentTriangles.front()->getType() == static_cast<IDependency::Type>(type2));
  REQUIRE(congruentTriangles.front()->getReason() == reason1);
  REQUIRE(congruentTriangles.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(congruentTriangles.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(congruentTriangles.front()->getImportance() == Importance1);
  REQUIRE(congruentTriangles.front()->getJson() == dependency5Json);
}

TEST_CASE("Adding PolygonTypeDependencies to DependenciesBank", "[expertBackground]") {
  const std::vector<std::string> triangleABC{"pointA", "pointB", "pointC"};
  const std::vector<std::string> rectangleABCD{"pointA", "pointB", "pointC", "pointD"};

  const PolygonModel::Type polygonType1{PolygonModel::Type::EQUILATERAL_TRIANGLE};
  const PolygonModel::Type polygonType2{PolygonModel::Type::SQUARE};

  const DependenciesBank::PolygonTypeDependencies type{DependenciesBank::PolygonTypeDependencies::POLYGON_TYPE};

  const IDependency::Reason reason1{IDependency::Reason::USER_DEFINED};
  const IDependency::Reason reason2{IDependency::Reason::CONGRUENT_TRIANGLES};

  const std::vector<size_t> basedOn1 = {2, 4};
  const std::vector<size_t> basedOn2 = {3, 1};

  const IDependency::Importance Importance1{IDependency::Importance::MEDIUM};
  const IDependency::Importance Importance2{IDependency::Importance::HIGH};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};

  const json dependency1Json = {{"object1",
                                 {
                                     {"id", std::to_string(static_cast<unsigned int>(polygonType1))},
                                 }},
                                {"object2",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::POLYGON_TYPE},
                                {"type", type},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addPolygonTypeDependency(triangleABC, polygonType1, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addPolygonTypeDependency(triangleABC, polygonType1, type, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other polygon
  REQUIRE(dependenciesBank.addPolygonTypeDependency(rectangleABCD, polygonType1, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency other polygon type
  REQUIRE(dependenciesBank.addPolygonTypeDependency(triangleABC, polygonType2, type, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addPolygonTypeDependency(triangleABC, polygonType1, type, reason1, basedOn2, Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addPolygonTypeDependency(triangleABC, polygonType1, type, reason2, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  const std::vector<std::shared_ptr<PolygonTypeDependency>> polygonTypes =
      dependenciesBank.getPolygonTypeDependencies(DependenciesBank::PolygonTypeDependencies::POLYGON_TYPE);

  REQUIRE(polygonTypes.size() == 4);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == polygonTypes.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == polygonTypes.back()->getId());

  REQUIRE(polygonTypes.front()->getCategory() == IDependency::Category::POLYGON_TYPE);
  REQUIRE(polygonTypes.front()->getType() == static_cast<IDependency::Type>(type));
  REQUIRE(polygonTypes.front()->getReason() == reason1);
  REQUIRE(polygonTypes.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(polygonTypes.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(polygonTypes.front()->getImportance() == Importance1);
  REQUIRE(polygonTypes.front()->getJson() == dependency1Json);
}

TEST_CASE("Adding LinePointPairDependencies to DependenciesBank", "[expertBackground]") {
  const std::string lineA{"lineA"};
  const std::string lineB{"lineB"};

  const std::string pointA{"pointA"};
  const std::string pointB{"pointB"};
  const std::string pointC{"pointC"};

  const DependenciesBank::LinePointsPairDependencies type1{DependenciesBank::LinePointsPairDependencies::MID_PERPENDICULAR_LINE};

  const IDependency::Reason reason1{IDependency::Reason::ANGLE_SIDE_ANGLE};
  const IDependency::Reason reason2{IDependency::Reason::PERPENDICULAR_LINES};

  const std::vector<size_t> basedOn1 = {2, 4};
  const std::vector<size_t> basedOn2 = {3, 1};

  const IDependency::Importance Importance1{IDependency::Importance::MEDIUM};
  const IDependency::Importance Importance2{IDependency::Importance::HIGH};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};

  const json dependency1Json = {{"object1",
                                 {
                                     {"id", lineA},
                                 }},
                                {"object2",
                                 {
                                     {"end1Id", pointA},
                                     {"end2Id", pointB},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_LINE_AND_POINTS_PAIR},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addLinePointsPairDependency(lineA, pointA, pointB, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addLinePointsPairDependency(lineA, pointA, pointB, type1, reason1, basedOn1, Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other line
  REQUIRE(dependenciesBank.addLinePointsPairDependency(lineB, pointA, pointB, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other points pair
  REQUIRE(dependenciesBank.addLinePointsPairDependency(lineA, pointC, pointB, type1, reason1, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addLinePointsPairDependency(lineA, pointA, pointB, type1, reason1, basedOn2, Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addLinePointsPairDependency(lineA, pointA, pointB, type1, reason2, basedOn1, Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  const std::vector<std::shared_ptr<LinePointPairDependency>> midPerpendiculars =
      dependenciesBank.getLinePointsPairDependencies(DependenciesBank::LinePointsPairDependencies::MID_PERPENDICULAR_LINE);

  REQUIRE(midPerpendiculars.size() == 4);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == midPerpendiculars.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == midPerpendiculars.back()->getId());

  REQUIRE(midPerpendiculars.front()->getCategory() == IDependency::Category::OF_LINE_AND_POINTS_PAIR);
  REQUIRE(midPerpendiculars.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(midPerpendiculars.front()->getReason() == reason1);
  REQUIRE(midPerpendiculars.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(midPerpendiculars.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(midPerpendiculars.front()->getImportance() == Importance1);
  REQUIRE(midPerpendiculars.front()->getJson() == dependency1Json);
}

TEST_CASE("Adding LineAngleDependencies to DependenciesBank", "[expertBackground]") {
  const std::string lineA{"lineA"};
  const std::string lineB{"lineB"};

  const std::string pointA{"pointA"};
  const std::string pointB{"pointB"};
  const std::string pointC{"pointC"};
  const std::string pointD{"pointD"};

  const AngleModel::Type angleType1{AngleModel::Type::CONVEX};
  const AngleModel::Type angleType2{AngleModel::Type::CONCAVE};

  const DependenciesBank::LineAngleDependencies type{DependenciesBank::LineAngleDependencies::BISECTOR_LINE};

  const IDependency::Reason reason1{IDependency::Reason::PARALLEL_LINES};
  const IDependency::Reason reason2{IDependency::Reason::POINTS_ARE_THE_SAME};

  const std::vector<size_t> basedOn1 = {4, 5, 6};
  const std::vector<size_t> basedOn2 = {1};

  const IDependency::Importance Importance1{IDependency::Importance::LOW};
  const IDependency::Importance Importance2{IDependency::Importance::HIGH};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};
  const size_t dependency6Id{baseDependenciesNumber + 5};

  const json dependency1Json = {{"object1",
                                 {
                                     {"id", lineA},
                                 }},
                                {"object2",
                                 {
                                     {"point1Id", pointA},
                                     {"vertexId", pointB},
                                     {"point2Id", pointC},
                                     {"type", angleType1},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_LINE_AND_ANGLE},
                                {"type", type},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointA, pointB, pointC, angleType1, type, reason1, basedOn1,
                                                  Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointA, pointB, pointC, angleType1, type, reason1, basedOn1,
                                                  Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other line
  REQUIRE(dependenciesBank.addLineAngleDependency(lineB, pointA, pointB, pointC, angleType1, type, reason1, basedOn1,
                                                  Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other AngleModel's point
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointC, pointB, pointD, angleType1, type, reason1, basedOn1,
                                                  Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other AngleModel's point
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointC, pointD, pointC, angleType1, type, reason1, basedOn1,
                                                  Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // new dependency = other AngleModel's type
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointC, pointB, pointC, angleType2, type, reason1, basedOn1,
                                                  Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointA, pointB, pointC, angleType1, type, reason1, basedOn2,
                                                  Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addLineAngleDependency(lineA, pointA, pointB, pointC, angleType1, type, reason2, basedOn1,
                                                  Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 6));

  const std::vector<std::shared_ptr<LineAngleDependency>> bisectors =
      dependenciesBank.getLineAngleDependencies(DependenciesBank::LineAngleDependencies::BISECTOR_LINE);

  REQUIRE(bisectors.size() == 6);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == bisectors.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency6Id)->getId() == bisectors.back()->getId());

  REQUIRE(bisectors.front()->getCategory() == IDependency::Category::OF_LINE_AND_ANGLE);
  REQUIRE(bisectors.front()->getType() == static_cast<IDependency::Type>(type));
  REQUIRE(bisectors.front()->getReason() == reason1);
  REQUIRE(bisectors.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(bisectors.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(bisectors.front()->getImportance() == Importance1);
  REQUIRE(bisectors.front()->getJson() == dependency1Json);
}

/*
TEST_CASE("Adding TriplePointsPairDependencies to DependenciesBank", "[expertBackground]") {
  const std::string pointA{"pointA"};
  const std::string pointB{"pointB"};
  const std::string pointC{"pointC"};
  const std::string pointD{"pointD"};
  const std::string pointE{"pointE"};
  const std::string pointF{"pointF"};

  const DependenciesBank::PointsPairPairPointsPairDependencies
      type{DependenciesBank::PointsPairPairPointsPairDependencies::MID_SEGMENT};

  const IDependency::Reason reason1{IDependency::Reason::USER_DEFINED};
  const IDependency::Reason reason2{IDependency::Reason::SIMILAR_TRIANGLES};

  const std::vector<size_t> basedOn1 = {0, 2};
  const std::vector<size_t> basedOn2 = {2, 7};

  const IDependency::Importance Importance1{IDependency::Importance::LOW};
  const IDependency::Importance Importance2{IDependency::Importance::HIGH};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};

  const json dependency1Json = {
      {"object1", {
        {"first", {
          {"end1Id", pointA},
          {"end2Id", pointB}
        }},
        {"second", {
          {"end1Id", pointC},
          {"end2Id", pointD}
        }}
      }},
      {"object2", {
        {"end1Id", pointE},
        {"end2Id", pointF}
      }},
      {"id", dependency1Id},
      {"category", IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR},
      {"type", type},
      {"reason", reason1},
      {"basedOn", basedOn1},
      {"importance", Importance1}
  };

  // new dependency
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointB,
      pointC, pointD,
      pointE, pointF,
      type, reason1, basedOn1, Importance1
  ) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointB,
      pointC, pointD,
      pointE, pointF,
      type, reason1, basedOn1, Importance1
  ) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but in other points order
  // in first PointsPairModel in ModelsPair
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointB, pointA,
      pointC, pointD,
      pointE, pointF,
      type, reason1, basedOn1, Importance1
  ) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but in other PointsPairModels
  // order in ModelsPair
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointC, pointD,
      pointA, pointB,
      pointE, pointF,
      type, reason1, basedOn1, Importance1
  ) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but in other points order
  // in PointsPairModel
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointB,
      pointC, pointD,
      pointF, pointE,
      type, reason1, basedOn1, Importance1
  ) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency but with other
  // dependent dependencies and other importance
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointB,
      pointC, pointD,
      pointE, pointF,
      type, reason1, basedOn2, Importance2
  ) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointB,
      pointC, pointD,
      pointE, pointF,
      type, reason2, basedOn1, Importance1
  ) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other second point in first PointsPairModel in ModelsPair
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointD,
      pointC, pointD,
      pointE, pointF,
      type, reason1, basedOn1, Importance1
  ) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other first point in PointsPairModel
  REQUIRE(dependenciesBank.addPointsPairPairPointsPairDependency(
      pointA, pointB,
      pointC, pointD,
      pointA, pointF,
      type, reason1, basedOn1, Importance1
  ) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  const std::vector<std::shared_ptr<PointsPairPairPointsPairDependency>> midSegments =
    dependenciesBank.getPointsPairPairPointsPairDependencies(DependenciesBank::PointsPairPairPointsPairDependencies::MID_SEGMENT);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == midSegments.front()->getId());

  REQUIRE(midSegments.size() == 4);
  REQUIRE(midSegments.front()->getCategory() == IDependency::Category::OF_POINTS_PAIRS_PAIR_AND_POINTS_PAIR);
  REQUIRE(midSegments.front()->getType() == static_cast<IDependency::Type>(type));
  REQUIRE(midSegments.front()->getReason() == reason1);
  REQUIRE(midSegments.front()->getDependentDependencies().size() == basedOn1.size());
  for(size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(midSegments.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(midSegments.front()->getImportance() == Importance1);
  REQUIRE(midSegments.front()->getJson() == dependency1Json);
}
*/

TEST_CASE("Adding PolygonPointsPairDependency to DependenciesBank", "[expertBackground]") {
  const std::vector<std::string> triangleABC{"pointA", "pointB", "pointC"};
  const std::vector<std::string> triangleDEF{"pointD", "pointE", "pointF"};
  const std::string pointG{"pointG"};
  const std::string pointH{"pointH"};
  const std::string pointI{"pointI"};

  const bool fixedPointsOrder = true;

  const DependenciesBank::PolygonPointsPairDependencies type1{DependenciesBank::PolygonPointsPairDependencies::ALTITUDE};
  const DependenciesBank::PolygonPointsPairDependencies type2{DependenciesBank::PolygonPointsPairDependencies::MEDIAN};

  const IDependency::Reason reason1{IDependency::Reason::ALTITUDE};
  const IDependency::Reason reason2{IDependency::Reason::CIRCUMSCRIBED_CIRCLE};

  const std::vector<size_t> basedOn1 = {21, 3, 1};
  const std::vector<size_t> basedOn2 = {9, 7, 11, 3};

  const IDependency::Importance Importance1{IDependency::Importance::LOW};
  const IDependency::Importance Importance2{IDependency::Importance::HIGH};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};

  const json dependency1Json = {{"object1",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"object2",
                                 {
                                     {"end1Id", pointG},
                                     {"end2Id", pointH},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_POLYGON_AND_POINTS_PAIRS},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency5Json = {{"object1",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"object2", {{"end1Id", pointG}, {"end2Id", pointH}}},
                                {"id", dependency5Id},
                                {"category", IDependency::Category::OF_POLYGON_AND_POINTS_PAIRS},
                                {"type", type2},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleABC, fixedPointsOrder, pointG, pointH, type1, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleABC, fixedPointsOrder, pointG, pointH, type1, reason1, basedOn1,
                                                          Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other polygon
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleDEF, fixedPointsOrder, pointG, pointH, type1, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency = other points pair
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleABC, fixedPointsOrder, pointG, pointI, type1, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleABC, fixedPointsOrder, pointG, pointH, type1, reason1, basedOn2,
                                                          Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleABC, fixedPointsOrder, pointG, pointH, type1, reason2, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // new dependency = other type
  REQUIRE(dependenciesBank.addPolygonPointsPairDependency(triangleABC, fixedPointsOrder, pointG, pointH, type2, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  const std::vector<std::shared_ptr<PolygonPointsPairDependency>> altitudes =
      dependenciesBank.getPolygonPointsPairDependencies(DependenciesBank::PolygonPointsPairDependencies::ALTITUDE);

  const std::vector<std::shared_ptr<PolygonPointsPairDependency>> medians =
      dependenciesBank.getPolygonPointsPairDependencies(DependenciesBank::PolygonPointsPairDependencies::MEDIAN);

  REQUIRE(altitudes.size() == 4);
  REQUIRE(medians.size() == 1);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == altitudes.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == altitudes.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency5Id)->getId() == medians.front()->getId());

  REQUIRE(altitudes.front()->getCategory() == IDependency::Category::OF_POLYGON_AND_POINTS_PAIRS);
  REQUIRE(altitudes.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(altitudes.front()->getReason() == reason1);
  REQUIRE(altitudes.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(altitudes.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(altitudes.front()->getImportance() == Importance1);
  REQUIRE(altitudes.front()->getJson() == dependency1Json);

  REQUIRE(medians.front()->getCategory() == IDependency::Category::OF_POLYGON_AND_POINTS_PAIRS);
  REQUIRE(medians.front()->getType() == static_cast<IDependency::Type>(type2));
  REQUIRE(medians.front()->getReason() == reason1);
  REQUIRE(medians.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(medians.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(medians.front()->getImportance() == Importance1);
  REQUIRE(medians.front()->getJson() == dependency5Json);
}

TEST_CASE("Adding PolygonExpressionDependency to DependenciesBank", "[expertBackground]") {
  const std::vector<std::string> triangleABC{"pointA", "pointB", "pointC"};
  const std::vector<std::string> triangleDEF{"pointD", "pointE", "pointF"};

  const ExpressionModel expression1{"3*x + y"};
  const ExpressionModel expression2{"165"};

  const bool fixedPointsOrder = false;

  const DependenciesBank::PolygonExpressionDependencies type1{DependenciesBank::PolygonExpressionDependencies::POLYGON_PERIMETER};
  const DependenciesBank::PolygonExpressionDependencies type2{DependenciesBank::PolygonExpressionDependencies::POLYGON_AREA};

  const IDependency::Reason reason1{IDependency::Reason::USER_DEFINED};
  const IDependency::Reason reason2{IDependency::Reason::CONGRUENT_TRIANGLES};

  const std::vector<size_t> basedOn1 = {21};
  const std::vector<size_t> basedOn2 = {13, 11, 9};

  const IDependency::Importance Importance1{IDependency::Importance::MEDIUM};
  const IDependency::Importance Importance2{IDependency::Importance::LOW};

  DependenciesBank dependenciesBank{};

  const size_t baseDependenciesNumber = dependenciesBank.getDependenciesNumber();
  const size_t dependency1Id{baseDependenciesNumber};
  const size_t dependency2Id{baseDependenciesNumber + 1};
  const size_t dependency3Id{baseDependenciesNumber + 2};
  const size_t dependency4Id{baseDependenciesNumber + 3};
  const size_t dependency5Id{baseDependenciesNumber + 4};

  const json dependency1Json = {{"object1",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"object2",
                                 {
                                     {"value", "3*x+y"},
                                     {"variables", std::vector<std::string>{"x", "y"}},
                                 }},
                                {"id", dependency1Id},
                                {"category", IDependency::Category::OF_POLYGON_AND_EXPRESSION},
                                {"type", type1},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  const json dependency5Json = {{"object1",
                                 {
                                     {"verticesIds", triangleABC},
                                 }},
                                {"object2",
                                 {
                                     {"value", "3*x+y"},
                                     {"variables", std::vector<std::string>{"x", "y"}},
                                 }},
                                {"id", dependency5Id},
                                {"category", IDependency::Category::OF_POLYGON_AND_EXPRESSION},
                                {"type", type2},
                                {"reason", reason1},
                                {"basedOn", basedOn1},
                                {"importance", Importance1}};

  // new dependency
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleABC, fixedPointsOrder, expression1, type1, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // attempt to add the same dependency
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleABC, fixedPointsOrder, expression1, type1, reason1, basedOn1,
                                                          Importance1) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 1));

  // new dependency = other polygon
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleDEF, fixedPointsOrder, expression1, type1, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 2));

  // new dependency other expression
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleABC, fixedPointsOrder, expression2, type1, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // attempt to add the same dependency
  // but with other dependent dependencies and importance
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleABC, fixedPointsOrder, expression1, type1, reason1, basedOn2,
                                                          Importance2) == 0);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 3));

  // new dependency = other reason
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleABC, fixedPointsOrder, expression1, type1, reason2, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 4));

  // new dependency = other type
  REQUIRE(dependenciesBank.addPolygonExpressionDependency(triangleABC, fixedPointsOrder, expression1, type2, reason1, basedOn1,
                                                          Importance1) == 1);
  REQUIRE(dependenciesBank.getDependenciesNumber() == (baseDependenciesNumber + 5));

  const std::vector<std::shared_ptr<PolygonExpressionDependency>> perimeters =
      dependenciesBank.getPolygonExpressionDependencies(DependenciesBank::PolygonExpressionDependencies::POLYGON_PERIMETER);

  const std::vector<std::shared_ptr<PolygonExpressionDependency>> areas =
      dependenciesBank.getPolygonExpressionDependencies(DependenciesBank::PolygonExpressionDependencies::POLYGON_AREA);

  REQUIRE(perimeters.size() == 4);
  REQUIRE(areas.size() == 1);

  REQUIRE(dependenciesBank.getDependencyById(dependency1Id)->getId() == perimeters.front()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency4Id)->getId() == perimeters.back()->getId());
  REQUIRE(dependenciesBank.getDependencyById(dependency5Id)->getId() == areas.back()->getId());

  REQUIRE(perimeters.front()->getCategory() == IDependency::Category::OF_POLYGON_AND_EXPRESSION);
  REQUIRE(perimeters.front()->getType() == static_cast<IDependency::Type>(type1));
  REQUIRE(perimeters.front()->getReason() == reason1);
  REQUIRE(perimeters.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(perimeters.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(perimeters.front()->getImportance() == Importance1);
  REQUIRE(perimeters.front()->getJson() == dependency1Json);

  REQUIRE(areas.front()->getCategory() == IDependency::Category::OF_POLYGON_AND_EXPRESSION);
  REQUIRE(areas.front()->getType() == static_cast<IDependency::Type>(type2));
  REQUIRE(areas.front()->getReason() == reason1);
  REQUIRE(areas.front()->getDependentDependencies().size() == basedOn1.size());
  for (size_t i = 0; i < basedOn1.size(); i++) {
    REQUIRE(areas.front()->getDependentDependencies().at(i) == basedOn1.at(i));
  }
  REQUIRE(areas.front()->getImportance() == Importance1);
  REQUIRE(areas.front()->getJson() == dependency5Json);
}