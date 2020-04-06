//
// Created by egork on 06.04.2020.
//
// report: https://contest.yandex.ru/contest/17594/run-report/32063722/
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

template<typename T>
struct Edge {
  Edge(int start, int finish, T cost) : start(start), finish(finish), cost(cost) {}

  int start;
  int finish;
  T cost;
};

template<typename T>
class EdgeGraph {
 public:
  explicit EdgeGraph(int size, int expectedEdges = 0) : nodeCount(size) {
    if (expectedEdges)
      edges.reserve(expectedEdges);
  }

  void addEdge(int start, int finish, T cost) {
    assert(0 <= start && start < nodeCount);
    assert(0 <= finish && finish < nodeCount);
    edges.emplace_back(start, finish, cost);
  }

  const std::vector<Edge<T>> &getEdges() const {
    return edges;
  }

  int getNodeCount() const {
    return nodeCount;
  }

 private:
  int nodeCount;
  std::vector<Edge<T>> edges;
};

// Считаем вероятность того, что не изобьют
double findMaxProbability(const EdgeGraph<double> &graph, int source, int destination) {
  std::vector<double> probability(graph.getNodeCount(), -1.0);
  std::vector<double> newProbability(probability);
  probability[source] = 1.0;

  for (int i = 0; i < graph.getNodeCount() - 1; ++i) {
    newProbability = probability;

    for (Edge e : graph.getEdges())
      if (probability[e.start] != -1.0)
        newProbability[e.finish] = std::max(
            newProbability[e.finish],
            probability[e.start] * e.cost
        );

    std::swap(probability, newProbability);
  }

  return probability[destination];
}

int main() {
  int n, m, s, f;
  std::cin >> n >> m >> s >> f;
  --s;
  --f;
  EdgeGraph<double> graph(n, m);

  for (int i = 0; i < m; ++i) {
    int start, finish;
    double probability;
    std::cin >> start >> finish >> probability;
    --start;
    --finish;
    graph.addEdge(start, finish, 1.0 - probability / 100.0);
    graph.addEdge(finish, start, 1.0 - probability / 100.0);
  }

  double maxProbability = findMaxProbability(graph, s, f);
  std::cout << (1.0 - maxProbability) << '\n';
}
