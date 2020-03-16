//
// Created by egork on 28.02.2020.
//
#include "Graph.h"
#include <vector>

#ifndef ALGORITHMS_2SEM__MATRIXGRAPH_H_
#define ALGORITHMS_2SEM__MATRIXGRAPH_H_

class MatrixGraph : public Graph {
 public:
  explicit MatrixGraph(int nodeCount);
  void addNode() override;
  void addEdge(int start, int finish) override;
  bool hasEdge(int start, int finish) const override;
  const std::vector<int> &getChildren(int node) const override;
  int getNodeCount() const override;
  bool isStart(int node) const;
  bool isFinish(int node) const;

 private:
  std::vector<std::vector<bool>> matrix;
};


#endif //ALGORITHMS_2SEM__MATRIXGRAPH_H_
