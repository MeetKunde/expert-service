#ifndef EXPERT_SERVICE_DEPENDENCY_THREE_ARGS_H
#define EXPERT_SERVICE_DEPENDENCY_THREE_ARGS_H

namespace expertBackground {
template <class T1, class T2, class T3>
class DependencyThreeArgs : public Dependency {
 private:
  T1 object1;
  
  T2 object2;
  
  T3 object3;

 public:
  DependencyThreeArgs(T1 object1, T2 object2, T3 object3, size_t identifier, IDependency::Category category, IDependency::Type type,
                    IDependency::Reason reason, std::vector<size_t> basedOn, IDependency::ImportanceLevel importanceLevel)
      : Dependency{identifier, category, type, reason, basedOn, importanceLevel}, object1{object1}, object2{object2}, object3{object3} { }

  DependencyThreeArgs(const DependencyThreeArgs& dependencyThreeArgs)
      : Dependency(dependencyThreeArgs), object1{dependencyThreeArgs.object1}, object2{dependencyThreeArgs.object2}, object3{dependencyThreeArgs.object3} { }

  DependencyThreeArgs& operator=(const DependencyThreeArgs& dependencyThreeArgs) {
    Dependency::operator=(dependencyThreeArgs);
    object1 = dependencyThreeArgs.object1;
    object2 = dependencyThreeArgs.object2;
    object3 = dependencyThreeArgs.object3;

    return *this;
  }

  inline const T1& getFirstObject() const { return object1; }

  inline const T1& getSecondObject() const { return object2; }

  inline const T1& getThirdObject() const { return object3; }

  virtual json getObjectAsJson() const override {
    json baseObjectJson = Dependency::getObjectAsJson();
    baseObjectJson["object1"] = object1.getJsonObject();
    baseObjectJson["object2"] = object2.getJsonObject();
    baseObjectJson["object3"] = object3.getJsonObject();

    return baseObjectJson;
  }

  friend bool operator==(const DependencyThreeArgs& lhs, const DependencyThreeArgs& rhs) {
    if (lhs.getReason() != rhs.getReason()) {
      return false;
    }

    if constexpr (std::is_same_v<T1, T2> && std::is_same_v<T2, T3>) {
      return (lhs.object1 == rhs.object1 && lhs.object2 == rhs.object2 && lhs.object3 == rhs.object3) ||
             (lhs.object1 == rhs.object1 && lhs.object2 == rhs.object3 && lhs.object3 == rhs.object2) ||
             (lhs.object1 == rhs.object2 && lhs.object2 == rhs.object1 && lhs.object3 == rhs.object3) ||
             (lhs.object1 == rhs.object2 && lhs.object2 == rhs.object3 && lhs.object3 == rhs.object1) ||
             (lhs.object1 == rhs.object3 && lhs.object2 == rhs.object2 && lhs.object3 == rhs.object1) ||
             (lhs.object1 == rhs.object3 && lhs.object2 == rhs.object1 && lhs.object3 == rhs.object2);
    }
    else if constexpr (std::is_same_v<T1, T2>) {
      return ((lhs.object1 == rhs.object1 && lhs.object2 == rhs.object2) ||
             (lhs.object1 == rhs.object2 && lhs.object2 == rhs.object1)) && (lhs.object3 == rhs.object3);
    }
    else if constexpr (std::is_same_v<T1, T3>) {
      return ((lhs.object1 == rhs.object1 && lhs.object3 == rhs.object3) ||
             (lhs.object1 == rhs.object3 && lhs.object3 == rhs.object1)) && (lhs.object2 == rhs.object2);;
    }
    else if constexpr (std::is_same_v<T2, T3>) {
      return ((lhs.object2 == rhs.object2 && lhs.object3 == rhs.object3) ||
             (lhs.object2 == rhs.object3 && lhs.object3 == rhs.object2)) && (lhs.object1 == rhs.object1);
    }
    else {
      return lhs.object1 == rhs.object1 && lhs.object2 == rhs.object2 && lhs.object3 == rhs.object3;
    }
  }

  friend std::ostream& operator<< (std::ostream& stream, const DependencyThreeArgs& dependencyThreeArgs) {
    stream << "[ " << dependencyThreeArgs.object1 << " & " << dependencyThreeArgs.object2 >> " & " << dependencyThreeArgs << " ]";
    return stream;
  }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_DEPENDENCY_THREE_ARGS_H
