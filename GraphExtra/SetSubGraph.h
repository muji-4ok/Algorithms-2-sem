//
// Created by egork on 07.03.2020.
//

#ifndef ALGORITHMS_2SEM_GRAPHEXTRA_SETSUBGRAPH_H_
#define ALGORITHMS_2SEM_GRAPHEXTRA_SETSUBGRAPH_H_

#include <unordered_set>
#include <unordered_map>
#include <set>

class SetSubGraph {
 public:
  SetSubGraph() = default;
  void addNode(int vertex);
  void removeNode(int vertex);
  bool hasNode(int vertex) const;
  void addEdge(int start, int finish);
  void removeEdge(int start, int finish);
  bool hasEdge(int start, int finish) const;
  const std::unordered_set<int> &getChildren(int node) const;
  const std::unordered_set<int> &getVertices() const;
  int getNodeCount() const;

 private:
  std::unordered_set<int> vertices{};
  std::unordered_map<int, std::unordered_set<int>> children{};
};

#endif //ALGORITHMS_2SEM_GRAPHEXTRA_SETSUBGRAPH_H_
