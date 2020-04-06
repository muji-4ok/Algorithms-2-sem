//
// Created by egork on 06.04.2020.
//
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>

struct Edge {
  Edge(int start, int finish, double probability)
      : start(start), finish(finish), probability(probability) {}

  int start;
  int finish;
  double probability;
};

class EdgeGraph {
 public:
  explicit EdgeGraph(int size, int expectedEdges = 0) : nodeCount(size) {
    if (expectedEdges)
      edges.reserve(expectedEdges);
  }

  void addEdge(int start, int finish, double probability) {
    assert(0 <= start && start < nodeCount);
    assert(0 <= finish && finish < nodeCount);
    edges.emplace_back(start, finish, probability);
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

// Считаем вероятность того, что не изобьют
double findMaxProbability(const EdgeGraph &graph, int source, int destination) {
  std::vector<double> probability(graph.getNodeCount(), -1.0);
  std::vector<double> newProbability(probability);
  probability[source] = 1.0;

  for (int i = 0; i < graph.getNodeCount() - 1; ++i) {
    newProbability = probability;

    for (Edge e : graph.getEdges())
      if (probability[e.start] != -1.0)
        newProbability[e.finish] = std::max(
            newProbability[e.finish],
            probability[e.start] * e.probability
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
  EdgeGraph graph(n, m);

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
