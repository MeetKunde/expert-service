#include "Graph.h"

namespace expertBackground {
Graph::Graph() : verticesNumber(0), adjacencyList({}), adjacencyMatrix({}) {}

Graph::Graph(unsigned int n) : verticesNumber{n}, adjacencyList{}, adjacencyMatrix{} {
  for (unsigned int i = 0; i < n; i++) {
    adjacencyList.insert({i, {}});
  }

  adjacencyMatrix = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
}

void Graph::addEdge(unsigned int vertex1, unsigned int vertex2) {
  adjacencyMatrix[vertex1][vertex2] = 1;
  adjacencyMatrix[vertex2][vertex1] = 1;
  adjacencyList[vertex1].push_back(vertex2);
  adjacencyList[vertex2].push_back(vertex1);
}

void Graph::deleteEdge(unsigned int vertex1, unsigned int vertex2) {
  adjacencyMatrix[vertex1][vertex2] = -1;
  adjacencyMatrix[vertex2][vertex1] = -1;

  adjacencyList[vertex1].erase(std::find(adjacencyList[vertex1].begin(), adjacencyList[vertex1].end(), vertex2));
  adjacencyList[vertex2].erase(std::find(adjacencyList[vertex2].begin(), adjacencyList[vertex2].end(), vertex1));
}

bool Graph::checkIfEdgeExist(unsigned int vertex1, unsigned int vertex2) const {
  return adjacencyMatrix[vertex1][vertex2] != -1;
}

std::vector<std::vector<unsigned int>> Graph::getAllTriangles() const {
  std::vector<std::vector<unsigned int>> result = {};

  for (unsigned int i = 0; i < verticesNumber; i++) {
    findTriangleUtility(i, &result);
  }

  return result;
}

void Graph::findTriangleUtility(unsigned int startVertex, std::vector<std::vector<unsigned int>>* result) const {
  for (std::vector<unsigned int>::const_iterator iter1 = adjacencyList.at(startVertex).begin();
       iter1 != adjacencyList.at(startVertex).end(); ++iter1) {
    const unsigned int secondVertex = *iter1;

    if (secondVertex <= startVertex) {
      continue;
    }

    for (std::vector<unsigned int>::const_iterator iter2 = adjacencyList.at(secondVertex).begin();
         iter2 != adjacencyList.at(secondVertex).end(); ++iter2) {
      const unsigned int thirdVertex = *iter2;
      if (thirdVertex <= secondVertex) {
        continue;
      }

      if (adjacencyMatrix[startVertex][thirdVertex] == 1) {
        result->push_back({startVertex, secondVertex, thirdVertex});
      }
    }
  }
}
}  // namespace expertBackground