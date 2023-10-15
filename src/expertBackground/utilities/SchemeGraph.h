#ifndef EXPERT_SERVICE_SCHEME_GRAPH_H
#define EXPERT_SERVICE_SCHEME_GRAPH_H

#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <ostream>

namespace expertBackground {

class SchemeGraph {
 private:
  static constexpr size_t INITIAL_GRAPH_CAPACITY = 16;

  size_t graphCapacity;   // vertices: 0, 1, ..., graphCapacity-1
  std::map<size_t, std::set<size_t>> adjacencyList;

 public:
  SchemeGraph();

  void addEdge(size_t vertex1, size_t vertex2);
  void deleteEdge(size_t vertex1, size_t vertex2);
  bool checkIfEdgeExist(size_t vertex1, size_t vertex2) const;

  std::vector<std::pair<size_t, size_t>> getAllEdges() const;
  std::vector<std::vector<size_t>> getAllTriangles() const;

  friend std::ostream& operator<<(std::ostream& stream, const SchemeGraph& schemeGraph);

 private:
  void resizeAdjacencyList();
  void findTriangleUtility(size_t startVertex, std::vector<std::vector<size_t>>* result) const;
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_SCHEME_GRAPH_H
