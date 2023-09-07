#ifndef EXPERT_SERVICE_DEPENDENCY_ONE_ARG_H
#define EXPERT_SERVICE_DEPENDENCY_ONE_ARG_H

#include "Dependency.h"

namespace expertBackground {
template <class T1>
class DependencyOneArg : public Dependency {
 private:
  T1 object;

 public:
  DependencyOneArg(T1 object, size_t identifier, IDependency::Category category, IDependency::Type type,
                   IDependency::Reason reason, std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel)
      : Dependency{identifier, category, type, reason, basedOn, importanceLevel}, object{object} { }

  DependencyOneArg(const DependencyOneArg& dependencyOneArg)
      : Dependency(dependencyOneArg), object{dependencyOneArg.object} { }

  DependencyOneArg& operator=(const DependencyOneArg& dependencyOneArg) {
    Dependency::operator=(dependencyOneArg);
    object = dependencyOneArg.object;

    return *this;
  }

  inline const T1& getObject() const { return object; }

  virtual json getObjectAsJson() const override {
    json baseObjectJson = Dependency::getObjectAsJson();
    baseObjectJson["object"] = object.getJsonObject();

    return baseObjectJson;
  }

  friend bool operator==(const DependencyOneArg& lhs, const DependencyOneArg& rhs) {
    if (lhs.getReason() != rhs.getReason()) {
      return false;
    }

    return lhs.object == rhs.object;
  }

  friend std::ostream& operator<< (std::ostream& stream, const DependencyOneArg& dependencyOneArg) {
    stream << "[ " << dependencyOneArg.object << " ]";
    return stream;
  }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_ONE_ARG_H
