#ifndef EXPERT_SERVICE_DEPENDENCY_H
#define EXPERT_SERVICE_DEPENDENCY_H

#include "IDependency.h"
#include "DependencyConcepts.cpp"

#include <tuple>
#include <type_traits>

typedef nlohmann::json json;

namespace expertBackground {
/**
 * @brief Representing dependency
 */
template <typename... Args>
requires AllStreamable<Args...> && AllJSONStreamable<Args...> && AllComparable<Args...>
class Dependency : public IDependency {
 public:
  Dependency() = delete;
  Dependency(size_t id, Category category, Type type, Reason reason, std::vector<size_t> basedOn, Importance importance, Args... args)
    : id(id), category(category), type(type), reason(reason), basedOn(basedOn), importance(importance), args(args...) {}

  Dependency(const Dependency& dependency) = default;
  Dependency(Dependency&& dependency) = default;

  Dependency& operator=(const Dependency& dependency) = default;
  Dependency& operator=(Dependency&& dependency) = default;

  ~Dependency() = default;

  inline size_t getId() const override { return id; }

  inline Category getCategory() const override { return category; }
  inline Type getType() const override { return type; }
  inline Reason getReason() const override { return reason; }
  inline const std::vector<size_t>& getDependentDependencies() const override { return basedOn; }
  inline Importance getImportance() const override { return importance; }

  inline json getJson() const override {
    json j;
    j["id"] = id;
    j["category"] = static_cast<int>(category);
    j["type"] = static_cast<int>(type);
    j["reason"] = static_cast<int>(reason);
    j["basedOn"] = basedOn;
    j["importance"] = static_cast<int>(importance);

    std::apply([&j](const auto&... elems) {
        std::size_t index = 0;
        ((j["arg" + std::to_string(index++)] << elems), ...);
    }, args);

    return j;
  }

  inline const std::tuple<Args...>& getArgs() const { return args; }

  friend bool operator==(const Dependency& dependency1, const Dependency& dependency2) {
    return dependency1.category == dependency2.category && dependency1.type == dependency2.type && 
           dependency1.reason == dependency2.reason && dependency1.basedOn == dependency2.basedOn && 
           dependency1.importance == dependency2.importance && dependency1.args == dependency2.args;
   
  }

  friend bool operator!=(const Dependency& dependency1, const Dependency& dependency2) {
    return !(dependency1 == dependency2);
  }

 private:
  size_t id;
  IDependency::Category category;
  IDependency::Type type;
  IDependency::Reason reason;
  std::vector<size_t> basedOn;
  IDependency::Importance importance;
  std::tuple<Args...> args;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_H
