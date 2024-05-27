#ifndef EXPERT_TEST_BASE_H
#define EXPERT_TEST_BASE_H

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <Expert.h>

typedef expert::Expert Expert;
typedef expertBackground::ExpressionModel ExpressionModel;
typedef expertBackground::PolygonModel::Type PolygonType;
typedef expertBackground::AngleModel::Type AngleType;
typedef expert::IDependency::Category DependencyCategory;
typedef expert::IDependency::Type DependencyType;
typedef expert::IDependency::Reason DependencyReason;
typedef expert::IDependency::Importance DependencyImportance;

#endif  //EXPERT_TEST_BASE_H
