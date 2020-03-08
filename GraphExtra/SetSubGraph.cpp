//
// Created by egork on 07.03.2020.
//

#include "SetSubGraph.h"
#include <cassert>

void SetSubGraph::addNode(int vertex) {
  vertices.insert(vertex);
}
bool SetSubGraph::hasNode(int vertex) const {
  return vertices.find(vertex) != vertices.end();
}
void SetSubGraph::addEdge(int start, int finish) {
  assert(hasNode(start) && hasNode(finish));
  children[start].insert(finish);
}
void SetSubGraph::removeEdge(int start, int finish) {
  assert(hasNode(start) && hasNode(finish));
  children[start].erase(finish);
}
bool SetSubGraph::hasEdge(int start, int finish) const {
  return hasNode(start) && hasNode(finish)
      && children.at(start).find(finish) != children.at(start).end();
}
const std::unordered_set<int> &SetSubGraph::getChildren(int node) const {
  assert(hasNode(node));
  return children.at(node);
}
int SetSubGraph::getNodeCount() const {
  return vertices.size();
}
const std::unordered_set<int> &SetSubGraph::getVertices() const {
  return vertices;
}
void SetSubGraph::removeNode(int vertex) {
  vertices.erase(vertex);
  children.erase(vertex);

  for (int v : vertices)
    children[v].erase(vertex);
}
