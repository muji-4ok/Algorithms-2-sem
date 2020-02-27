//
// Created by egork on 28.02.2020.
//
#include <iostream>
#include "ListGraph.h"
#include "GraphTraversal.h"

int main() {
  int n, m;
  std::cin >> n >> m;
  ListGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    graph.addEdge(u, v);
  }

  std::vector<int> path = dfsTree(graph);

  for (int i = path.size() - 1; i >= 0; --i)
    std::cout << path[i] << ' ';

  std::cout << '\n';
}
