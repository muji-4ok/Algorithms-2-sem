//
// Created by egork on 06.04.2020.
//
// report: https://contest.yandex.ru/contest/17594/run-report/32062200/
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

struct Edge {
  Edge(int start, int finish, int cost) : start(start), finish(finish), cost(cost) {}

  int start;
  int finish;
  int cost;
};

class EdgeGraph {
 public:
  explicit EdgeGraph(int size, int expectedEdges = 0) : nodeCount(size) {
    if (expectedEdges)
      edges.reserve(expectedEdges);
  }

  void addEdge(int start, int finish, int cost) {
    assert(0 <= start && start < nodeCount);
    assert(0 <= finish && finish < nodeCount);
    edges.emplace_back(start, finish, cost);
  }

  const std::vector<Edge> &getEdges() const {
    return edges;
  }

  int getNodeCount() const {
    return nodeCount;
  }

 private:
  int nodeCount;
  std::vector<Edge> edges;
};

int findMinDistance(const EdgeGraph &graph, int source, int destination, int maxPathLength) {
  std::vector<int> distance(graph.getNodeCount(), INT32_MAX);
  std::vector<int> newDistance(distance);
  distance[source] = 0;

  for (int i = 0; i < maxPathLength && i < graph.getNodeCount() - 1; ++i) {
    newDistance = distance;

    for (Edge e : graph.getEdges())
      if (distance[e.start] < INT32_MAX)
        newDistance[e.finish] = std::min(newDistance[e.finish], distance[e.start] + e.cost);

    std::swap(distance, newDistance);
  }

  return distance[destination];
}

int main() {
  int n, m, k, s, f;
  std::cin >> n >> m >> k >> s >> f;
  --s;
  --f;
  EdgeGraph graph(n, m);

  for (int i = 0; i < m; ++i) {
    int start, finish, cost;
    std::cin >> start >> finish >> cost;
    --start;
    --finish;
    graph.addEdge(start, finish, cost);
  }

  int distance = findMinDistance(graph, s, f, k);
  std::cout << (distance == INT32_MAX ? -1 : distance) << '\n';
}
