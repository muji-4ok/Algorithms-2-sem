//
// Created by egork on 28.02.2020.
//

#include "GraphTraversal.h"

bool dfsWalk(int start, const Graph &graph, std::vector<char> &states, std::vector<int> &path) {
  std::vector<int> stack;
  stack.push_back(start);

  while (!stack.empty()) {
    int vertex = stack.back();

    if (states[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    states[vertex] = 1;

    for (int child : graph.getChildren(vertex)) {
      if (states[child] == 1) {
        return false;
      } else if (states[child] == 0) {
        stack.push_back(child);
      }
    }

    while (!stack.empty() && states[stack.back()] == 1) {
      path.push_back(stack.back());
      states[stack.back()] = 2;
      stack.pop_back();
    }
  }

  return true;
}
std::vector<int> dfsTree(const Graph &graph) {
  std::vector<int> path;
  std::vector<char> states(graph.getNodeCount(), 0);

  for (int i = 0; i < graph.getNodeCount(); ++i) {
    if (states[i] == 0) {
      if (!dfsWalk(i, graph, states, path))
        return {};
    }
  }

  return path;
}
