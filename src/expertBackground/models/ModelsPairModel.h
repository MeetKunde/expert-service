#ifndef EXPERT_SERVICE_MODELS_PAIR_MODEL_H
#define EXPERT_SERVICE_MODELS_PAIR_MODEL_H

#include "IModel.h"

namespace expertBackground {
template <typename T>
class ModelsPairModel : public IModel {
 private:
  T first;
  T second;

 public:
  ModelsPairModel(T firstModel, T secondModel) : first(std::move(firstModel)), second(std::move(secondModel)) {
    if (firstModel > secondModel) {
      T tmp = std::move(first);
      first = std::move(second);
      second = std::move(tmp);
    }

    static_assert(std::is_convertible<T*, IModel*>::value, "Class T must inherit IModel as public");
  }

  ModelsPairModel(const ModelsPairModel& modelsPairModel) : first(modelsPairModel.first), second(modelsPairModel.second) {}

  ModelsPairModel& operator=(const ModelsPairModel& modelsPairModel) {
    first = modelsPairModel.first;
    second = modelsPairModel.second;

    return *this;
  }

  inline const T& getFirstElement() const { return first; }
  inline const T& getSecondElement() const { return second; }

  friend bool operator==(const ModelsPairModel& lhs, const ModelsPairModel& rhs) {
    return lhs.getFirstElement() == rhs.getFirstElement() && lhs.getSecondElement() == rhs.getSecondElement();
  }

  friend bool operator!=(const ModelsPairModel& lhs, const ModelsPairModel& rhs) { return !(lhs == rhs); }

  json getJsonObject() const override { return {{"first", first.getJsonObject()}, {"second", second.getJsonObject()}}; }
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_MODELS_PAIR_MODEL_H
