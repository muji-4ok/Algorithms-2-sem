//
// Created by egork on 04.03.2020.
//

#ifndef ALGORITHMS_2SEM_GRAPHEXTRA_SETGRAPH_H_
#define ALGORITHMS_2SEM_GRAPHEXTRA_SETGRAPH_H_

#include <vector>
#include <unordered_set>

class SetGraph {
 public:
  explicit SetGraph(int nodeCount);
  void addNode();
  void addEdge(int start, int finish);
  void removeEdge(int start, int finish);
  bool hasEdge(int start, int finish) const;
  const std::unordered_set<int> &getChildren(int node) const;
  int getNodeCount() const;
 private:
  std::vector<std::unordered_set<int>> children;

};

#endif //ALGORITHMS_2SEM_GRAPHEXTRA_SETGRAPH_H_
