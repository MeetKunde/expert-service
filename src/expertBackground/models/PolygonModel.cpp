#include "PolygonModel.h"

namespace expertBackground {
PolygonModel::PolygonModel(Type type, std::vector<std::string> vertices) : 
  type{type}, verticesIds{std::move(vertices)} {}

json PolygonModel::getJson() const {
  return {{"type", static_cast<int>(type)}, 
          {"vertices", verticesIds}};
}

bool operator==(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2) {
  const size_t size1 = polygonModel1.verticesIds.size();
  const size_t size2 = polygonModel2.verticesIds.size();

  if (size1 != size2) {
    return false;
  }

  if (size1 == 0) {
    return true;
  }

  std::string minId1 = polygonModel1.verticesIds[0];
  std::string minId2 = polygonModel2.verticesIds[0];
  size_t minId1Index = 0;
  size_t minId2Index = 0;
  for (size_t i = 1; i < size1; i++) {
    if (minId1 > polygonModel1.verticesIds[i]) {
      minId1 = polygonModel1.verticesIds[i];
      minId1Index = i;
    }

    if (minId2 > polygonModel2.verticesIds[i]) {
      minId2 = polygonModel2.verticesIds[i];
      minId2Index = i;
    }
  }

  bool test1 = true;
  bool test2 = true;
  for (size_t i = 0; i < size1; i++) {
    if (polygonModel1.verticesIds[(minId1Index + i) % size1] != polygonModel2.verticesIds[(minId2Index + i) % size1]) {
      test1 = false;
      break;
    }
  }

  for (size_t i = 0; i < size1; i++) {
    if (polygonModel1.verticesIds[(minId1Index + i) % size1] != polygonModel2.verticesIds[(minId2Index - i) % size1]) {
      test2 = false;
      break;
    }
  }

  if(test1 && test2) {
    if(polygonModel1.type != polygonModel2.type && 
      polygonModel1.type != PolygonModel::Type::UNKNOWN && 
      polygonModel2.type != PolygonModel::Type::UNKNOWN) {
      return false;
    }
  }

  return test1 || test2;
}

bool operator!=(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2) {
  return !(polygonModel1 == polygonModel2);
}

bool operator<(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2) {
  const size_t size1 = polygonModel1.verticesIds.size();
  const size_t size2 = polygonModel2.verticesIds.size();

  if (size1 != size2) {
    return size1 < size2;
  }

  if (size1 == 0) {
    return false;
  }

  std::string minId1 = polygonModel1.verticesIds[0];
  std::string minId2 = polygonModel2.verticesIds[0];
  size_t minId1Index = 0;
  size_t minId2Index = 0;
  for (size_t i = 1; i < size1; i++) {
    if (minId1 > polygonModel1.verticesIds[i]) {
      minId1 = polygonModel1.verticesIds[i];
      minId1Index = i;
    }

    if (minId2 > polygonModel2.verticesIds[i]) {
      minId2 = polygonModel2.verticesIds[i];
      minId2Index = i;
    }
  }

  for (size_t i = 0; i < size1; i++) {
    if (polygonModel1.verticesIds[(minId1Index + i) % size1] != polygonModel2.verticesIds[(minId2Index + i) % size1]) {
      return polygonModel1.verticesIds[(minId1Index + i) % size1] < polygonModel2.verticesIds[(minId2Index + i) % size1];
    }
  }

  return polygonModel1.type < polygonModel2.type;
}

bool operator>(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2) {
  return polygonModel2 < polygonModel1;
}

bool operator<=(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2) {
  return !(polygonModel2 < polygonModel1);
}

bool operator>=(const PolygonModel& polygonModel1, const PolygonModel& polygonModel2) {
  return !(polygonModel1 < polygonModel2);
}

std::ostream& operator<<(std::ostream& stream, const PolygonModel& polygonModel) {
  stream << "->";
  std::vector<std::string>::const_iterator iter;
  for (iter = polygonModel.verticesIds.begin(); iter != polygonModel.verticesIds.end(); ++iter) {
    stream << (*iter) << "->";
  }

  return stream;
}

json& operator<<(json& j, const PolygonModel& polygonModel) {
  j = polygonModel.getJson();
  return j;
}
}  // namespace expertBackground