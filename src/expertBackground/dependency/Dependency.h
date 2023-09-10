#ifndef EXPERT_SERVICE_DEPENDENCY_H
#define EXPERT_SERVICE_DEPENDENCY_H

#include "IDependency.h"
#include "models/IModel.h"

namespace expertBackground {
/**
 * @brief Representing geometric dependency
 */
template <class T1, class T2>
class Dependency : public IDependency {
 private:
  T1 object1;

  T2 object2;

  bool sameMeaningOfModels;

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

 public:
  /**
   * @brief Constructor of a new Dependency object
   *
   * @param object1
   * @param object2
   * @param identifier
   * @param category
   * @param type
   * @param reason
   * @param basedOn
   * @param importanceLevel
   */
  Dependency(T1 object1, T2 object2, bool sameMeaningOfModels, size_t identifier, Category category, Type type, Reason reason,
                 std::vector<size_t> basedOn, ImportanceLevel importanceLevel)
      : object1{object1},
        object2{object2},
        sameMeaningOfModels{sameMeaningOfModels},
        dependencyId{identifier},
        dependencyCategory{category},
        dependencyType{type},
        dependencyReason{reason},
        basedOn{std::move(basedOn)},
        importance{importanceLevel} { }

  /**
   * @brief Constructor of a new Dependency object
   *
   * @param dependency other Dependency object
   */
  Dependency(const Dependency& dependency)
      : object1{object1},
        object2{object2},
        sameMeaningOfModels{sameMeaningOfModels},
        dependencyId{dependency.dependencyId},
        dependencyCategory{dependency.dependencyCategory},
        dependencyType{dependency.dependencyType},
        dependencyReason{dependency.dependencyReason},
        basedOn{dependency.basedOn},
        importance{dependency.importance} {}

  /**
   * @brief Override of the assignment operator
   *
   * @param dependency object to be assigned
   * @return new Dependency object
   */
  Dependency& operator=(const Dependency& dependency) {
    object1 = dependency.object1;
    object2 = dependency.object2;
    sameMeaningOfModels = dependency.sameMeaningOfModels;
    dependencyId = dependency.dependencyId;
    dependencyCategory = dependency.dependencyCategory;
    dependencyType = dependency.dependencyType;
    dependencyReason = dependency.dependencyReason;
    basedOn = dependency.basedOn;
    importance = dependency.importance;

    return *this;
  }

  /**
   * @brief Dependency first object getter
   *
   * @return dependency first object
   */
  inline const T1& getFirstObject() const { return object1; }

  /**
   * @brief Dependency second object getter
   *
   * @return dependency second object
   */
  inline const T2& getSecondObject() const { return object2; }

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
        {"object1", object1.getJsonObject()},
        {"object2", object2.getJsonObject()},
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

  friend bool operator==(const Dependency& lhs, const Dependency& rhs) {
    if (lhs.getReason() != rhs.getReason()) {
      return false;
    }

    if constexpr (std::is_same_v<T1, T2>) {
      if(lhs.sameMeaningOfModels && rhs.sameMeaningOfModels) {
        return (lhs.getFirstObject() == rhs.getFirstObject() && lhs.getSecondObject() == rhs.getSecondObject()) ||
               (lhs.getFirstObject() == rhs.getSecondObject() && lhs.getSecondObject() == rhs.getFirstObject());
      }

      return lhs.getFirstObject() == rhs.getFirstObject() && lhs.getSecondObject() == rhs.getSecondObject();
    }
    else {
      return lhs.getFirstObject() == rhs.getFirstObject() && lhs.getSecondObject() == rhs.getSecondObject();
    }
  }

  ~Dependency() { }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_H
