#ifndef EXPERT_SERVICE_DEPENDENCY_H
#define EXPERT_SERVICE_DEPENDENCY_H

#include "IDependency.h"
#include "models/IModel.h"

namespace expertBackground {
/**
 * @brief Representing geometric dependency
 */
class Dependency : public IDependency {
 public:
  /**
   * @brief Dependency ID
   */
  size_t dependencyId;

  /**
   * @brief Dependency category
   */
  Category dependencyCategory;

  /**
   * @brief Dependency type
   */
  Type dependencyType;

  /**
   * @brief Dependency reason
   */
  Reason dependencyReason;

  /**
   * @brief IDs of dependencies from which this dependency is implied
   */
  std::vector<size_t> basedOn;

  /**
   * @brief Importance of dependency
   */
  ImportanceLevel importance;

  /**
   * @brief Constructor of a new Dependency object
   *
   * @param identifier
   * @param category
   * @param type
   * @param reason
   * @param basedOn
   * @param importanceLevel
   */
  Dependency(size_t identifier, Category category, Type type, Reason reason,
                 std::vector<size_t> basedOn, ImportanceLevel importanceLevel)
      : dependencyId(identifier),
        dependencyCategory(category),
        dependencyType(type),
        dependencyReason(reason),
        basedOn(std::move(basedOn)),
        importance(importanceLevel) { }

  /**
   * @brief Constructor of a new Dependency object
   *
   * @param dependency other Dependency object
   */
  Dependency(const Dependency& dependency)
      : dependencyId(dependency.dependencyId),
        dependencyCategory(dependency.dependencyCategory),
        dependencyType(dependency.dependencyType),
        dependencyReason(dependency.dependencyReason),
        basedOn(dependency.basedOn),
        importance(dependency.importance) {}

  /**
   * @brief Override of the assignment operator
   *
   * @param dependency object to be assigned
   * @return new Dependency object
   */
  Dependency& operator=(const Dependency& dependency) {
    dependencyId = dependency.dependencyId;
    dependencyCategory = dependency.dependencyCategory;
    dependencyType = dependency.dependencyType;
    dependencyReason = dependency.dependencyReason;
    basedOn = dependency.basedOn;
    importance = dependency.importance;

    return *this;
  }

  /**
   * @brief Dependency ID getter
   *
   * @return dependency ID
   */
  inline size_t getId() const override { return dependencyId; }

  /**
   * @brief Dependency category getter
   *
   * @return dependency category
   */
  inline Category getCategory() const override { return dependencyCategory; }

  /**
   * @brief Dependency type getter
   *
   * @return dependency type
   */
  inline Type getType() const override { return dependencyType; }

  /**
   * @brief Dependency reason getter
   *
   * @return dependency reason
   */
  inline Reason getReason() const override { return dependencyReason; }

  /**
   * @brief Reference to vector of included dependencies on which this is implied IDs getter
   *
   * @return reference to vector of included dependencies on which this is implied IDs
   */
  inline const std::vector<size_t>& getDependentDependencies() const override { return basedOn; }

  /**
   * @brief Dependency importance getter
   *
   * @return dependency reason
   */
  inline ImportanceLevel getImportanceLevel() const override { return importance; }

  /**
   * @brief Getting JSON object representing Dependency object
   *
   * @return JSON object representing Dependency object
   */
  virtual json getObjectAsJson() const override {
    return {
        {"id", dependencyId},
        {"category", dependencyCategory},
        {"type", dependencyType},
        {"reason", dependencyReason},
        {"basedOn", basedOn},
        {"importance", importance}
    };
  }

  friend std::ostream& operator<< (std::ostream& stream, const Dependency& /*dependency*/) {
    return stream;
  }

  ~Dependency() { }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_H
