//
// Created by egork on 07.03.2020.
//

#ifndef ALGORITHMS_2SEM_GRAPHEXTRA_EDGESUBGRAPH_H_
#define ALGORITHMS_2SEM_GRAPHEXTRA_EDGESUBGRAPH_H_

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

struct Edge {
  Edge() = default;
  Edge(int start, int finish) : start(start), finish(finish) {}
  bool operator==(const Edge& other) const;

  int start{};
  int finish{};
};

struct EdgeHasher {
  std::size_t operator()(const Edge& edge) const noexcept;
};

class EdgeStorage {
 public:
  EdgeStorage() = default;
  void addEdge(int start, int finish);
  void addEdge(Edge edge);
  bool hasEdge(int start, int finish) const;
  bool hasEdge(Edge edge) const;
  void removeEdge(int start, int finish);
  void removeEdge(Edge edge);
  std::string toString() const;
  int getEdgeCount() const;

 private:
  std::unordered_set<Edge, EdgeHasher> edges;
};

class EdgeSubGraph : public EdgeStorage {
 public:
  EdgeSubGraph() = default;
  explicit EdgeSubGraph(std::vector<int> cycle);
  void addNode(int vertex);
  bool hasNode(int vertex) const;
  int getNodeCount() const;

 private:
  std::unordered_set<int> vertices{};
  std::unordered_set<Edge, EdgeHasher> edges;
};

#endif //ALGORITHMS_2SEM_GRAPHEXTRA_EDGESUBGRAPH_H_
