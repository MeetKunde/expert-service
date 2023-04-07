#include "IdentifierModel.h"

namespace expertBackground {
IdentifierModel::IdentifierModel(unsigned int identifier) : id(identifier) {}

IdentifierModel::IdentifierModel(const IdentifierModel& identifierModel) : id(identifierModel.id) {}

IdentifierModel& IdentifierModel::operator=(const IdentifierModel& identifierModel) {
  id = identifierModel.id;

  return *this;
}

json IdentifierModel::getJsonObject() const {
  return {{"id", id}};
}

bool operator==(const IdentifierModel& lhs, const IdentifierModel& rhs) {
  return lhs.id == rhs.id;
}

bool operator!=(const IdentifierModel& lhs, const IdentifierModel& rhs) {
  return !(lhs == rhs);
}
}  // namespace expertBackground