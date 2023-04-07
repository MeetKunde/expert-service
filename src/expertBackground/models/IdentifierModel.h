#ifndef EXPERT_SERVICE_IDENTIFIER_MODEL_H
#define EXPERT_SERVICE_IDENTIFIER_MODEL_H

#include "IModel.h"

namespace expertBackground {
class IdentifierModel : public IModel {
 private:
  unsigned int id;

 public:
  explicit IdentifierModel(unsigned int identifier);
  IdentifierModel(const IdentifierModel& identifierModel);
  IdentifierModel& operator=(const IdentifierModel& identifierModel);

  inline unsigned int getId() const { return id; }

  friend bool operator==(const IdentifierModel& lhs, const IdentifierModel& rhs);
  friend bool operator!=(const IdentifierModel& lhs, const IdentifierModel& rhs);

  json getJsonObject() const override;
};
}  // namespace expertBackground

#endif  //EXPERT_SERVICE_IDENTIFIER_MODEL_H
