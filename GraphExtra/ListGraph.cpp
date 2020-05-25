//
// Created by egork on 28.02.2020.
//

#include "ListGraph.h"
#include <algorithm>

void ListGraph::addNode() {
  children.emplace_back();
}

void ListGraph::addEdge(int start, int finish) {
  children[start].push_back(finish);
}

bool ListGraph::hasEdge(int start, int finish) const {
  for (const int &child : children[start])
    if (child == finish) return true;

  return false;
}

const std::vector<int> &ListGraph::getChildren(int node) const {
  return children[node];
}

ListGraph::ListGraph(int nodeCount) : children(nodeCount, std::vector<int>()) {
}

int ListGraph::getNodeCount() const {
  return children.size();
}
void ListGraph::makeSorted(bool reverse = false) {
  for (std::vector<int> &kids : children)
    if (reverse)
      std::sort(kids.rbegin(), kids.rend());
    else
      std::sort(kids.begin(), kids.end());
}
ListGraph ListGraph::makeInverted() const {
  ListGraph newGraph(getNodeCount());

  for (int from = 0; from < getNodeCount(); ++from)
    for (int to : getChildren(from))
      newGraph.addEdge(to, from);

  return newGraph;
}
