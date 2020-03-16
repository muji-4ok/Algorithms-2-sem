//
// Created by egork on 28.02.2020.
//
#include <vector>

#ifndef ALGORITHMS_2SEM__GRAPH_H_
#define ALGORITHMS_2SEM__GRAPH_H_

struct Graph {
  virtual void addNode() = 0;
  virtual void addEdge(int start, int finish) = 0;
  virtual bool hasEdge(int start, int finish) const = 0;
  virtual const std::vector<int> &getChildren(int node) const = 0;
  virtual int getNodeCount() const = 0;
};

#endif //ALGORITHMS_2SEM__GRAPH_H_
