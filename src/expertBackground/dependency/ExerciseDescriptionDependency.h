#ifndef EXPERT_SERVICE_EXERCISE_DESCRIPTION_DEPENDENCY_H
#define EXPERT_SERVICE_EXERCISE_DESCRIPTION_DEPENDENCY_H

#include "IDependency.h"

namespace expertBackground {
class ExerciseDescriptionDependency : public IDependency {
 private:
  size_t  id;
  std::vector<Reason> reasons;
  std::vector<std::vector<size_t>> dependentDependencies;
  std::vector<ImportanceLevel> importances;

 public:
  ExerciseDescriptionDependency():
        id{0}, reasons{std::vector<Reason>{Reason::USER_DEFINED}},
        dependentDependencies{std::vector<std::vector<size_t>>{}},
        importances{std::vector<ImportanceLevel>{ImportanceLevel::HIGH}}  { }

  ExerciseDescriptionDependency(size_t dependencyId):
        id{dependencyId}, reasons{std::vector<Reason>{Reason::USER_DEFINED}},
        dependentDependencies{std::vector<std::vector<size_t>>{}},
        importances{std::vector<ImportanceLevel>{ImportanceLevel::HIGH}}  { }

  inline size_t getId() const override { return id; };
  inline Category getCategory() const override { return IDependency::Category::EXERCISE_DESCRIPTION;}
  inline Type getType() const override { return IDependency::Type::EXERCISE_DESCRIPTION; }
  inline const std::vector<Reason>& getReasons() const override { return reasons; }
  inline const std::vector<std::vector<size_t>>& getDependentDependencies() const override { return dependentDependencies; }
  inline const std::vector<ImportanceLevel>& getImportanceLevels() const override { return importances; }
  inline json getObjectAsJson() const override {
    return {
        {"id", id},
        {"category", IDependency::Category::EXERCISE_DESCRIPTION},
        {"type", IDependency::Type::EXERCISE_DESCRIPTION},
        {"reasons", reasons},
        {"dependentDependencies", dependentDependencies},
        {"importances", importances}
    };
  }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_EXERCISE_DESCRIPTION_DEPENDENCY_H
