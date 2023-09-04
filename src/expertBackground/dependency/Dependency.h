#ifndef EXPERT_SERVICE_DEPENDENCY_H
#define EXPERT_SERVICE_DEPENDENCY_H

#include "IDependency.h"
#include "models/IModel.h"

namespace expertBackground {
/**
 * @brief Representing geometric dependency
 */
template <class T, class U>
class Dependency : public IDependency {
 private:
  /**
   * @brief Dependency ID
   */
  size_t dependencyId;

  /**
   * @brief First dependency element of T type
   */
  T firstObject;

  /**
   * @brief Second dependency element of U type
   */
  U secondObject;

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
  std::vector<size_t> dependencyBasedOn;

  /**
   * @brief Usefulness of dependency
   */
  UsefulnessLevel usefulness;

 public:
  /**
   * @brief Constructor of a new Dependency object
   *
   * @param first
   * @param second
   * @param id
   * @param category
   * @param type
   * @param reason
   * @param basedOn
   */
  Dependency(T first, U second, size_t identifier, Category category, Type type,
             Reason reason, std::vector<size_t> basedOn, UsefulnessLevel usefulnessLevel)
      : dependencyId(identifier),
        firstObject(first),
        secondObject(second),
        dependencyCategory(category),
        dependencyType(type),
        dependencyReason(reason),
        dependencyBasedOn(std::move(basedOn)),
        usefulness(usefulnessLevel) {

    static_assert(std::is_convertible<T*, IModel*>::value, "Class T must inherit IModel as public");
    static_assert(std::is_convertible<U*, IModel*>::value, "Class U must inherit IModel as public");
  }

  /**
   * @brief Constructor of a new Dependency object
   *
   * @param dependencyModel other Dependency object
   */
  Dependency(const Dependency& dependencyModel)
      : dependencyId(dependencyModel.dependencyId),
        firstObject(dependencyModel.firstObject),
        secondObject(dependencyModel.secondObject),
        dependencyCategory(dependencyModel.dependencyCategory),
        dependencyType(dependencyModel.dependencyType),
        dependencyReason(dependencyModel.dependencyReason),
        dependencyBasedOn(dependencyModel.dependencyBasedOn),
        usefulness(dependencyModel.usefullness) {}

  /**
   * @brief Override of the assignment operator
   *
   * @param dependencyModel object to be assigned
   * @return new Dependency object
   */
  Dependency& operator=(const Dependency& dependencyModel) {
    dependencyId = dependencyModel.dependencyId;
    firstObject = dependencyModel.firstObject;
    secondObject = dependencyModel.secondObject;
    dependencyCategory = dependencyModel.dependencyCategory;
    dependencyType = dependencyModel.dependencyType;
    dependencyReason = dependencyModel.dependencyReason;
    dependencyBasedOn = dependencyModel.dependencyBasedOn;
    usefulness = dependencyModel.usefullness;

    return *this;
  }

  /**
   * @brief Dependency ID getter
   *
   * @return dependency ID
   */
  inline size_t getId() const override { return dependencyId; }

  inline const T& getFirstElement() const { return firstObject; }

  inline const U& getSecondElement() const { return secondObject; }

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
  inline const std::vector<size_t>& getDependentDependencies() const override { return dependencyBasedOn; }

  inline UsefulnessLevel getUsefulnessLevel() const override { return usefulness; }

  /**
   * @brief Getting JSON object representing DependencyModel object
   *
   * @return JSON object representing DependencyModel object
   */
  virtual json getObjectAsJson() const override {
    return {{"object1", firstObject.getJsonObject()},
            {"object2", secondObject.getJsonObject()},
            {"id", dependencyId},
            {"category", dependencyCategory},
            {"type", dependencyType},
            {"reason", dependencyReason},
            {"basedOn", dependencyBasedOn},
            {"usefulness", usefulness}};
  }

  friend bool operator==(const Dependency& lhs, const Dependency& rhs) {
    if (lhs.getReason() != rhs.getReason()) {
      return false;
    }

    if constexpr (std::is_same_v<T, U>) {
      return (lhs.getFirstElement() == rhs.getFirstElement() && lhs.getSecondElement() == rhs.getSecondElement()) ||
             (lhs.getFirstElement() == rhs.getSecondElement() && lhs.getSecondElement() == rhs.getFirstElement());
    }
    else {
      return lhs.getFirstElement() == rhs.getFirstElement() && lhs.getSecondElement() == rhs.getSecondElement();
    }
  }

  ~Dependency() { }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_H
