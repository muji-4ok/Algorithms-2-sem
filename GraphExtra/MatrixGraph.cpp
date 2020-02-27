//
// Created by egork on 28.02.2020.
//

#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int nodeCount) : matrix(nodeCount, std::vector<bool>(nodeCount, false)) {
}
void MatrixGraph::addNode() {
  for (auto &i : matrix)
    i.push_back(false);

  matrix.emplace_back(getNodeCount() + 1, false);
}
void MatrixGraph::addEdge(int start, int finish) {
  matrix[start][finish] = true;
}
bool MatrixGraph::hasEdge(int start, int finish) const {
  return matrix[start][finish];
}
const std::vector<int> &MatrixGraph::getChildren(int node) const {
  std::vector<int> children;

  for (int i = 0; i < getNodeCount(); ++i)
    if (matrix[node][i])
      children.push_back(i);
}
int MatrixGraph::getNodeCount() const {
  return matrix.size();
}
bool MatrixGraph::isStart(int node) const {
  for (int i = 0; i < getNodeCount(); ++i)
    if (i != node && hasEdge(i, node))
      return false;

  return true;
}
bool MatrixGraph::isFinish(int node) const {
  for (int i = 0; i < getNodeCount(); ++i)
    if (i != node && hasEdge(node, i))
      return false;

  return true;
}
