//
// Created by egork on 07.03.2020.
//

#include "SetSubGraph.h"
#include <cassert>

void SetSubGraph::addNode(int vertex) {
  vertices.insert(vertex);
  children[vertex];
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

  for (int v : getVertices())
    children[v].erase(vertex);
}
std::string SetSubGraph::toString() const {
  std::string out;
  std::unordered_map<int, std::unordered_map<int, bool>> printed;

  for (int v : getVertices())
    for (int u : getChildren(v))
      if (!printed[v][u]) {
        out += std::to_string(v) + " <---> " + std::to_string(u) + '\n';
        printed[v][u] = true;
        printed[u][v] = true;
      }

  return out;
}
SetSubGraph::SetSubGraph(int nodeCount) {
  for (int i = 0; i < nodeCount; ++i)
    addNode(i);
}
