//
// Created by egork on 28.02.2020.
//
#include "Graph.h"

#ifndef ALGORITHMS_2SEM__LISTGRAPH_H_
#define ALGORITHMS_2SEM__LISTGRAPH_H_

class ListGraph : public Graph {
 public:
  explicit ListGraph(int nodeCount);
  void addNode() override;
  void addEdge(int start, int finish) override;
  bool hasEdge(int start, int finish) const override;
  const std::vector<int> &getChildren(int node) const override;
  int getNodeCount() const override;
  void makeSorted(bool reverse);
  ListGraph makeInverted() const;
 public:
 private:
  std::vector<std::vector<int>> children;
};


#endif //ALGORITHMS_2SEM__LISTGRAPH_H_
