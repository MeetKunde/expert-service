#include "PolygonModel.h"

namespace expertBackground {
PolygonModel::PolygonModel(std::vector<unsigned int> vertices) : verticesIds(std::move(vertices)) {}

PolygonModel::PolygonModel(const PolygonModel& polygonModel) : verticesIds(polygonModel.verticesIds) {}

PolygonModel& PolygonModel::operator=(const PolygonModel& polygonModel) {
  verticesIds = std::vector<unsigned int>(polygonModel.verticesIds);

  return *this;
}

bool operator==(const PolygonModel& lhs, const PolygonModel& rhs) {
  const size_t lhsSize = lhs.verticesIds.size();
  const size_t rhsSize = rhs.verticesIds.size();

  if (lhsSize != rhsSize) {
    return false;
  }

  if (lhsSize == 0) {
    return true;
  }

  unsigned int lhsMinId = lhs.verticesIds[0];
  unsigned int rhsMinId = rhs.verticesIds[0];
  size_t lhsMinIdIndex = 0;
  size_t rhsMinIdIndex = 0;
  for (size_t i = 1; i < lhsSize; i++) {
    if (lhsMinId > lhs.verticesIds[i]) {
      lhsMinId = lhs.verticesIds[i];
      lhsMinIdIndex = i;
    }

    if (rhsMinId > rhs.verticesIds[i]) {
      rhsMinId = rhs.verticesIds[i];
      rhsMinIdIndex = i;
    }
  }

  bool test1 = true;
  bool test2 = true;
  for (size_t i = 0; i < lhsSize; i++) {
    if (lhs.verticesIds[(lhsMinIdIndex + i) % lhsSize] != rhs.verticesIds[(rhsMinIdIndex + i) % lhsSize]) {
      test1 = false;
      break;
    }
  }

  for (size_t i = 0; i < lhsSize; i++) {
    if (lhs.verticesIds[(lhsMinIdIndex + i) % lhsSize] != rhs.verticesIds[(rhsMinIdIndex - i) % lhsSize]) {
      test2 = false;
      break;
    }
  }

  return test1 || test2;
}

bool operator!=(const PolygonModel& lhs, const PolygonModel& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const PolygonModel& polygonModel) {
  stream << "->";
  std::vector<unsigned int>::const_iterator iter;
  for (iter = polygonModel.verticesIds.begin(); iter != polygonModel.verticesIds.end(); ++iter) {
    stream << (*iter) << "->";
  }

  return stream;
}

json PolygonModel::getJsonObject() const {
  return {{"verticesIds", json(verticesIds)}};
}
PolygonModel::~PolygonModel() {}
}  // namespace expertBackground