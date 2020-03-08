//
// Created by egork on 07.03.2020.
//

#ifndef ALGORITHMS_2SEM_GRAPHEXTRA_EDGESUBGRAPH_H_
#define ALGORITHMS_2SEM_GRAPHEXTRA_EDGESUBGRAPH_H_

#include <unordered_set>
#include <vector>

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

class EdgeSubGraph {
 public:
  explicit EdgeSubGraph(std::vector<int> cycle);
  void addEdge(int start, int finish);
  void addEdge(Edge edge);
  void addNode(int vertex);
  bool hasEdge(int start, int finish) const;
  bool hasEdge(Edge edge) const;
  bool hasNode(int vertex) const;

 private:
  std::unordered_set<int> vertices{};
  std::unordered_set<Edge, EdgeHasher> edges;
};

#endif //ALGORITHMS_2SEM_GRAPHEXTRA_EDGESUBGRAPH_H_
