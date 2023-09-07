#ifndef EXPERT_SERVICE_DEPENDENCY_TWO_ARGS_H
#define EXPERT_SERVICE_DEPENDENCY_TWO_ARGS_H

#include "Dependency.h"

namespace expertBackground {
template <class T1, class T2>
class DependencyTwoArgs : public Dependency {
 private:
  T1 object1;

  T2 object2;

 public:
  DependencyTwoArgs(T1 object1, T2 object2, size_t identifier, IDependency::Category category, IDependency::Type type,
                   IDependency::Reason reason, std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel)
      : Dependency{identifier, category, type, reason, basedOn, importanceLevel}, object1{object1}, object2{object2} { }

  DependencyTwoArgs(const DependencyTwoArgs& dependencyTwoArgs)
      : Dependency(dependencyTwoArgs), object1{dependencyTwoArgs.object1}, object2{dependencyTwoArgs.object2} { }

  DependencyTwoArgs& operator=(const DependencyTwoArgs& dependencyTwoArgs) {
    Dependency::operator=(dependencyTwoArgs);
    object1 = dependencyTwoArgs.object1;
    object2 = dependencyTwoArgs.object2;

    return *this;
  }

  inline const T1& getFirstObject() const { return object1; }

  inline const T1& getSecondObject() const { return object2; }

  virtual json getObjectAsJson() const override {
    json baseObjectJson = Dependency::getObjectAsJson();
    baseObjectJson["object1"] = object1.getJsonObject();
    baseObjectJson["object2"] = object2.getJsonObject();

    return baseObjectJson;
  }

  friend bool operator==(const DependencyTwoArgs& lhs, const DependencyTwoArgs& rhs) {
    if (lhs.getReason() != rhs.getReason()) {
      return false;
    }

    if constexpr (std::is_same_v<T1, T2>) {
      return (lhs.object1 == rhs.object1 && lhs.object2 == rhs.object2) ||
             (lhs.object1 == rhs.object2 && lhs.object2 == rhs.object1);
    }
    else {
      return lhs.object1 == rhs.object1 && lhs.object2 == rhs.object2;
    }
  }

  friend std::ostream& operator<< (std::ostream& stream, const DependencyTwoArgs& dependencyTwoArgs) {
    stream << "[ " << dependencyTwoArgs.object1 << " & " << dependencyTwoArgs.object2 << " ]";
    return stream;
  }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_TWO_ARGS_H
