#ifndef EXPERT_SERVICE_GRAPH_H
#define EXPERT_SERVICE_GRAPH_H

#include <map>
#include <vector>
#include <algorithm>

namespace expertBackground {

class Graph {
 private:
  unsigned int verticesNumber;
  std::map<unsigned int, std::vector<unsigned int>> adjacencyList;
  std::vector<std::vector<int>> adjacencyMatrix;
  std::vector<bool> visited;

 public:
  Graph();
  explicit Graph(unsigned int n);  // vertices = 0, 1, ..., n-1

  void addEdge(unsigned int vertex1, unsigned int vertex2);
  void deleteEdge(unsigned int vertex1, unsigned int vertex2);
  bool checkIfEdgeExist(unsigned int vertex1, unsigned int vertex2) const;

  std::vector<std::vector<unsigned int>> getAllTriangles() const;

 private:
  void findTriangleUtility(unsigned int startVertex, std::vector<std::vector<unsigned int>>* result) const;
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_GRAPH_H
