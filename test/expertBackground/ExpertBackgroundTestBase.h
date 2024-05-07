#ifndef EXPERT_BACKGROUND_TEST_BASE_H
#define EXPERT_BACKGROUND_TEST_BASE_H

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <expertBackground.h>

constexpr float EPSILON = 0.00001;

typedef expertBackground::ShapesBank ShapesBank;
typedef expertBackground::DependenciesBank DependenciesBank;
typedef expertBackground::IDependency IDependency;

typedef expertBackground::PointModel PointModel;
typedef expertBackground::LineModel LineModel;
typedef expertBackground::CircleModel CircleModel;
typedef expertBackground::IdentifierModel IdentifierModel;
typedef expertBackground::AngleModel AngleModel;
typedef expertBackground::PolygonModel PolygonModel;
typedef expertBackground::ExpressionModel ExpressionModel;

typedef expertBackground::EquationDependency EquationDependency;
typedef expertBackground::LinesDependency LinesDependency;
typedef expertBackground::CirclesDependency CirclesDependency;
typedef expertBackground::AnglesDependency AnglesDependency;
typedef expertBackground::PolygonsDependency PolygonsDependency;
typedef expertBackground::LineCircleDependency LineCircleDependency;
typedef expertBackground::PolygonTypeDependency PolygonTypeDependency;
typedef expertBackground::CirclePolygonDependency CirclePolygonDependency;
typedef expertBackground::PointsPairsDependency PointsPairsDependency;
typedef expertBackground::LinePointsPairDependency LinePointPairDependency;
typedef expertBackground::LineAngleDependency LineAngleDependency;
typedef expertBackground::PointsPairPairPointsPairDependency PointsPairPairPointsPairDependency;
typedef expertBackground::PolygonPointsPairDependency PolygonPointsPairDependency;
typedef expertBackground::PolygonExpressionDependency PolygonExpressionDependency;

#endif  //EXPERT_BACKGROUND_TEST_BASE_H
