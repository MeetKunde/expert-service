#include "PolygonModel.h"

namespace expertBackground {
PolygonModel::PolygonModel(std::vector<std::string> vertices, bool fixedPointsOrder) : 
  verticesIds{std::move(vertices)}, fixedPointsOrder{fixedPointsOrder} {}

PolygonModel::PolygonModel(const PolygonModel& polygonModel) : 
  verticesIds{polygonModel.verticesIds}, fixedPointsOrder{polygonModel.fixedPointsOrder} {}

PolygonModel& PolygonModel::operator=(const PolygonModel& polygonModel) {
  verticesIds = std::vector<std::string>(polygonModel.verticesIds);
  fixedPointsOrder = polygonModel.fixedPointsOrder;

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

  if(lhs.fixedPointsOrder && rhs.fixedPointsOrder) {
    for (size_t i = 0; i < lhsSize; i++) {
      if(lhs.verticesIds[i] != rhs.verticesIds[i]) {
        return false;
      }
    }

    return true;
  }
  else {
    std::string lhsMinId = lhs.verticesIds[0];
    std::string rhsMinId = rhs.verticesIds[0];
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
}

bool operator!=(const PolygonModel& lhs, const PolygonModel& rhs) {
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const PolygonModel& polygonModel) {
  stream << "->";
  std::vector<std::string>::const_iterator iter;
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