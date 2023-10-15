#include "SchemeGraph.h"

namespace expertBackground {
SchemeGraph::SchemeGraph() : graphCapacity(INITIAL_GRAPH_CAPACITY), adjacencyList({}) {
  for(size_t i = 0; i < graphCapacity; i++) {
    adjacencyList.insert({i, {}});
  }
}

void SchemeGraph::addEdge(size_t vertex1, size_t vertex2) {
  while(vertex1 >= graphCapacity || vertex2 >= graphCapacity) {
    resizeAdjacencyList();
  }
  adjacencyList[vertex1].insert(vertex2);
  adjacencyList[vertex2].insert(vertex1);
}

void SchemeGraph::deleteEdge(size_t vertex1, size_t vertex2) {
  adjacencyList[vertex1].erase(std::find(adjacencyList[vertex1].begin(), adjacencyList[vertex1].end(), vertex2));
  adjacencyList[vertex2].erase(std::find(adjacencyList[vertex2].begin(), adjacencyList[vertex2].end(), vertex1));
}

bool SchemeGraph::checkIfEdgeExist(size_t vertex1, size_t vertex2) const {
  return std::find(adjacencyList.at(vertex1).begin(), adjacencyList.at(vertex1).end(), vertex2) != adjacencyList.at(vertex1).end();
}

std::vector<std::pair<size_t, size_t>> SchemeGraph::getAllEdges() const {
    std::vector<std::pair<size_t, size_t>>  edges{};

    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
      for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
        if(it->first <= *jt) {
          edges.emplace_back(it->first, *jt);
        }
      }
    }

    return edges;
}

std::vector<std::vector<size_t>> SchemeGraph::getAllTriangles() const {
  std::vector<std::vector<size_t>> triangles = {};

  for (size_t i = 0; i < graphCapacity; i++) {
    findTriangleUtility(i, &triangles);
  }

  return triangles;
}

std::ostream& operator<<(std::ostream& stream, const SchemeGraph& schemeGraph) {
  for (auto it = schemeGraph.adjacencyList.begin(); it != schemeGraph.adjacencyList.end(); ++it) {
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
      stream << "{" << it->first << ", " << *jt << "}\n";
    }
  }

  return stream;
}

void SchemeGraph::resizeAdjacencyList() {
  for(size_t i = graphCapacity; i < 2 * graphCapacity; i++) {
    adjacencyList.insert({i, {}});
  }
  graphCapacity = 2 * graphCapacity;
}

void SchemeGraph::findTriangleUtility(size_t startVertex, std::vector<std::vector<size_t>>* result) const {
  std::set<size_t>::const_iterator iter1;
  for (iter1 = adjacencyList.at(startVertex).begin(); iter1 != adjacencyList.at(startVertex).end(); ++iter1) {
    const size_t secondVertex = *iter1;

    if (secondVertex <= startVertex) {
      continue;
    }

    std::set<size_t>::const_iterator iter2;
    for (iter2 = adjacencyList.at(secondVertex).begin(); iter2 != adjacencyList.at(secondVertex).end(); ++iter2) {
      const size_t thirdVertex = *iter2;
      if (thirdVertex <= secondVertex) {
        continue;
      }

      if (checkIfEdgeExist(startVertex, thirdVertex)) {
        result->push_back({startVertex, secondVertex, thirdVertex});
      }
    }
  }
}
}  // namespace expertBackground