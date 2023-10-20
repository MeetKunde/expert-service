#include "Expert.h"

namespace expert {
unsigned int Expert::explorePolygonsTheorems() {
  unsigned int sumOfNewDependencies{0};

  if(heuristicsBank.getNewEvaluationFlag() || heuristicsBank.getNewDependencyFlag(IDependency::Type::SEGMENT_LENGTH) || heuristicsBank.getNewDependencyFlag(IDependency::Type::ANGLE_MEASURE) || heuristicsBank.getNewDependencyFlag(IDependency::Type::EQUATION)) {
    //sumOfNewDependencies += setSumOfAnglesInTriangles();
    sumOfNewDependencies += findCongruentAndSimilarTriangles();
  }

  return sumOfNewDependencies;
}

unsigned int Expert::findCongruentAndSimilarTriangles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<PointModel>& points = shapesBank.getPointsVector();
  const std::vector<std::vector<size_t>> triangles = heuristicsBank.trianglesInScheme();
  std::vector<std::vector<size_t>> nonDegenerateTriangles{};
  for (const auto& triangle: triangles) {
    if (!pointsLiesOnOneLine(triangle)) {
      nonDegenerateTriangles.push_back(triangle);
    }
  }

  for(size_t triangle1Pos = 0; triangle1Pos < nonDegenerateTriangles.size(); triangle1Pos++) {
    const std::vector<size_t>& triangle1 = nonDegenerateTriangles[triangle1Pos];

    const std::vector<symbolicAlgebra::Variable> triangle1Sides = {
        dependenciesBank.getSegmentLengthVariable(points.at(triangle1[0]), points.at(triangle1[1])),
        dependenciesBank.getSegmentLengthVariable(points.at(triangle1[1]), points.at(triangle1[2])),
        dependenciesBank.getSegmentLengthVariable(points.at(triangle1[2]), points.at(triangle1[0]))
    };

    const std::vector<symbolicAlgebra::Variable> triangle1Angles = {
        dependenciesBank.getAngleMeasureVariable(points.at(triangle1[0]), points.at(triangle1[1]), points.at(triangle1[2]), true),
        dependenciesBank.getAngleMeasureVariable(points.at(triangle1[1]), points.at(triangle1[2]), points.at(triangle1[0]), true),
        dependenciesBank.getAngleMeasureVariable(points.at(triangle1[2]), points.at(triangle1[0]), points.at(triangle1[1]), true)
    };

    for(size_t triangle2Pos = triangle1Pos + 1; triangle2Pos < nonDegenerateTriangles.size(); triangle2Pos++) {
      const std::vector<size_t>& triangle2 = nonDegenerateTriangles[triangle2Pos];

      const std::vector<symbolicAlgebra::Variable> triangle2Sides = {
          dependenciesBank.getSegmentLengthVariable(points.at(triangle2[0]), points.at(triangle2[1])),
          dependenciesBank.getSegmentLengthVariable(points.at(triangle2[1]), points.at(triangle2[2])),
          dependenciesBank.getSegmentLengthVariable(points.at(triangle2[2]), points.at(triangle2[0]))
      };

      const std::vector<symbolicAlgebra::Variable> triangle2Angles = {
          dependenciesBank.getAngleMeasureVariable(points.at(triangle2[0]), points.at(triangle2[1]), points.at(triangle2[2]), true),
          dependenciesBank.getAngleMeasureVariable(points.at(triangle2[1]), points.at(triangle2[2]), points.at(triangle2[0]), true),
          dependenciesBank.getAngleMeasureVariable(points.at(triangle2[2]), points.at(triangle2[0]), points.at(triangle2[1]), true)
      };

      bool foundCongruent = false;

      // congruent SSS rule
      for(size_t shift = 0; shift < 3; shift++) {
        auto compare1 = dependenciesBank.evaluateEquation(triangle1Sides[0], triangle2Sides[(0 + shift) % 3]);
        if (!compare1.first) {
          continue;
        }

        auto compare2 = dependenciesBank.evaluateEquation(triangle1Sides[1], triangle2Sides[(1 + shift) % 3]);
        if (!compare2.first) {
          continue;
        }

        auto compare3 = dependenciesBank.evaluateEquation(triangle1Sides[2], triangle2Sides[(2 + shift) % 3]);
        if (!compare3.first) {
          continue;
        }

        std::vector<size_t> dependentDependencies{};
        dependentDependencies.insert(dependentDependencies.end(), compare1.second.begin(), compare1.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare2.second.begin(), compare2.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare3.second.begin(), compare3.second.end());

        const std::vector<std::string> triangle1Ids = {
          points[triangle1[0]].getId(), points[triangle1[1]].getId(), points[triangle1[2]].getId()
        };

        const std::vector<std::string> triangle2Ids = {
            points[triangle2[(0 + shift) % 3]].getId(), points[triangle2[(1 + shift) % 3]].getId(), points[triangle2[(2 + shift) % 3]].getId()
        };

        sumOfNewDependencies += dependenciesBank.addPolygonsDependency(triangle1Ids, triangle2Ids, true,
                                                                       PolygonsDependencies::CONGRUENT_TRIANGLES,
                                                                       IDependency::Reason::SIDE_SIDE_SIDE,
                                                                       dependentDependencies, IDependency::ImportanceLevel::HIGH);
        foundCongruent = true;
        break;
      }

      // congruent SAS rule
      for (size_t shift = 0; shift < 3; shift++) {
        auto compare1 = dependenciesBank.evaluateEquation(triangle1Sides[0], triangle2Sides[(0 + shift) % 3]);
        if (!compare1.first) {
          continue;
        }

        auto compare2 = dependenciesBank.evaluateEquation(triangle1Sides[1], triangle2Sides[(1 + shift) % 3]);
        if (!compare2.first) {
          continue;
        }

        auto compare3 = dependenciesBank.evaluateEquation(triangle1Angles[0], triangle2Angles[(0 + shift) % 3]);
        if (!compare3.first) {
          continue;
        }

        std::vector<size_t> dependentDependencies{};
        dependentDependencies.insert(dependentDependencies.end(), compare1.second.begin(), compare1.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare2.second.begin(), compare2.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare3.second.begin(), compare3.second.end());

        const std::vector<std::string> triangle1Ids = {
            points[triangle1[0]].getId(), points[triangle1[1]].getId(), points[triangle1[2]].getId()
        };

        const std::vector<std::string> triangle2Ids = {
            points[triangle2[(0 + shift) % 3]].getId(), points[triangle2[(1 + shift) % 3]].getId(), points[triangle2[(2 + shift) % 3]].getId()
        };

        sumOfNewDependencies += dependenciesBank.addPolygonsDependency(triangle1Ids, triangle2Ids, true,
                                                                       PolygonsDependencies::CONGRUENT_TRIANGLES,
                                                                       IDependency::Reason::SIDE_ANGLE_SIDE,
                                                                       dependentDependencies, IDependency::ImportanceLevel::HIGH);
        foundCongruent = true;
        break;
      }

      // congruent ASA rule
      for (size_t shift = 0; shift < 3; shift++) {
        auto compare1 = dependenciesBank.evaluateEquation(triangle1Angles[0], triangle2Angles[(0 + shift) % 3]);
        if (!compare1.first) {
          continue;
        }

        auto compare2 = dependenciesBank.evaluateEquation(triangle1Angles[1], triangle2Angles[(1 + shift) % 3]);
        if (!compare2.first) {
          continue;
        }

        auto compare3 = dependenciesBank.evaluateEquation(triangle1Sides[1], triangle2Sides[(1 + shift) % 3]);
        if (!compare3.first) {
          continue;
        }

        std::vector<size_t> dependentDependencies{};
        dependentDependencies.insert(dependentDependencies.end(), compare1.second.begin(), compare1.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare2.second.begin(), compare2.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare3.second.begin(), compare3.second.end());

        const std::vector<std::string> triangle1Ids = {
            points[triangle1[0]].getId(), points[triangle1[1]].getId(), points[triangle1[2]].getId()
        };

        const std::vector<std::string> triangle2Ids = {
            points[triangle2[(0 + shift) % 3]].getId(), points[triangle2[(1 + shift) % 3]].getId(), points[triangle2[(2 + shift) % 3]].getId()
        };

        sumOfNewDependencies += dependenciesBank.addPolygonsDependency(triangle1Ids, triangle2Ids, true,
                                                                       PolygonsDependencies::CONGRUENT_TRIANGLES,
                                                                       IDependency::Reason::ANGLE_SIDE_ANGLE,
                                                                       dependentDependencies, IDependency::ImportanceLevel::HIGH);
        foundCongruent = true;
        break;
      }

      if(foundCongruent) {
        continue;
      }

      // similar AAA rule
      for (size_t shift = 0; shift < 3; shift++) {
        auto compare1 = dependenciesBank.evaluateEquation(triangle1Angles[0], triangle2Angles[(0 + shift) % 3]);
        if (!compare1.first) {
          continue;
        }

        auto compare2 = dependenciesBank.evaluateEquation(triangle1Angles[1], triangle2Angles[(1 + shift) % 3]);
        if (!compare2.first) {
          continue;
        }

        auto compare3 = dependenciesBank.evaluateEquation(triangle1Angles[2], triangle2Angles[(2 + shift) % 3]);
        if (!compare3.first) {
          continue;
        }

        std::vector<size_t> dependentDependencies{};
        dependentDependencies.insert(dependentDependencies.end(), compare1.second.begin(), compare1.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare2.second.begin(), compare2.second.end());
        dependentDependencies.insert(dependentDependencies.end(), compare3.second.begin(), compare3.second.end());

        const std::vector<std::string> triangle1Ids = {
            points[triangle1[0]].getId(), points[triangle1[1]].getId(), points[triangle1[2]].getId()
        };

        const std::vector<std::string> triangle2Ids = {
            points[triangle2[(0 + shift) % 3]].getId(), points[triangle2[(1 + shift) % 3]].getId(), points[triangle2[(2 + shift) % 3]].getId()
        };

        sumOfNewDependencies += dependenciesBank.addPolygonsDependency(triangle1Ids, triangle2Ids, true,
                                                                       PolygonsDependencies::SIMILAR_TRIANGLES,
                                                                       IDependency::Reason::ANGLE_ANGLE_ANGLE,
                                                                       dependentDependencies, IDependency::ImportanceLevel::HIGH);
      }
    }
  }

  return sumOfNewDependencies;
}

unsigned int Expert::setSumOfAnglesInTriangles() {
  unsigned int sumOfNewDependencies{0};

  const std::vector<PointModel>& points = shapesBank.getPointsVector();
  const std::vector<std::vector<size_t>> triangles = heuristicsBank.trianglesInScheme();
  std::vector<std::vector<size_t>> nonDegenerateTriangles{};
  for (const auto& triangle: triangles) {
    if (!pointsLiesOnOneLine(triangle)) {
      nonDegenerateTriangles.push_back(triangle);
    }
  }

  for (const auto& triangle: nonDegenerateTriangles) {
    auto angle1 =  dependenciesBank.getAngleMeasureVariable(points.at(triangle.at(0)), points.at(triangle.at(1)), points.at(triangle.at(2)), true);
    auto angle2 =  dependenciesBank.getAngleMeasureVariable(points.at(triangle.at(1)), points.at(triangle.at(2)), points.at(triangle.at(0)), true);
    auto angle3 =  dependenciesBank.getAngleMeasureVariable(points.at(triangle.at(2)), points.at(triangle.at(0)), points.at(triangle.at(1)), true);

    sumOfNewDependencies += dependenciesBank.addEquation(angle1 + angle2 + angle3, Integer(MathHelper::SUM_OF_ANGLES_IN_TRIANGLE),
                                                         IDependency::Reason::SUM_OF_ANGLES_IN_TRIANGLE, {EXERCISE_DESCRIPTION_ID},
                                                         IDependency::ImportanceLevel::LOW);
  }

  return sumOfNewDependencies;
}

}  // namespace expert