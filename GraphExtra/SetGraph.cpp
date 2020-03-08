//
// Created by egork on 04.03.2020.
//

#include "SetGraph.h"
SetGraph::SetGraph(int nodeCount) : children(nodeCount) {

}
void SetGraph::addNode() {
  children.emplace_back();
}
void SetGraph::addEdge(int start, int finish) {
  children[start].insert(finish);
}
bool SetGraph::hasEdge(int start, int finish) const {
  return children[start].find(finish) != children[start].end();
}
const std::unordered_set<int> &SetGraph::getChildren(int node) const {
  return children[node];
}
int SetGraph::getNodeCount() const {
  return children.size();
}
void SetGraph::removeEdge(int start, int finish) {
  children[start].erase(finish);
}
