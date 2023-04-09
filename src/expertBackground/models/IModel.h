#ifndef EXPERT_SERVICE_I_MODEL_H
#define EXPERT_SERVICE_I_MODEL_H

#include <nlohmann/json.hpp>

typedef nlohmann::json json;

namespace expertBackground {

class IModel {
 public:
  virtual json getJsonObject() const = 0;
  virtual ~IModel();
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_I_MODEL_H
