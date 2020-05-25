//
// Created by egork on 08.05.2020.
//
#include <iostream>
#include <vector>
#include <cassert>
#include <tuple>

class MatrixGraph {
 public:
  explicit MatrixGraph(int nodeCount) :
      costs(nodeCount, std::vector<int>(nodeCount, false)) {}

  void setCost(int start, int finish, int cost) { costs[start][finish] = cost; }
  int getCost(int start, int finish) { return costs[start][finish]; }
  int getNodeCount() const { return costs.size(); }
 private:
  std::vector<std::vector<int>> costs;
};

class DSU {
 public:
  explicit DSU(int vertexCount);
  void merge(int v1, int v2);
  int parent(int vertex);
  bool areMerged(int v1, int v2);
  bool isRoot(int v) const { return parents[v] == v; }
  int getNodeCount() const { return parents.size(); }
  std::vector<int> getSet(int v);
 private:
  std::vector<int> parents;
  std::vector<int> sizes;
};
DSU::DSU(int vertexCount) : parents(vertexCount, 0), sizes(vertexCount, 1) {
  for (int i = 0; i < vertexCount; ++i)
    parents[i] = i;
}
void DSU::merge(int v1, int v2) {
  int p1 = parent(v1);
  int p2 = parent(v2);

  if (p1 == p2)
    return;

  if (sizes[p2] > sizes[p1])
    std::swap(p1, p2);
  // p1 is bigger

  parents[p2] = p1;
  sizes[p1] += sizes[p2];
}
int DSU::parent(int vertex) {
  assert(0 <= vertex && vertex < getNodeCount());
  if (parents[vertex] == vertex)
    return vertex;

  return (parents[vertex] = parent(parents[vertex]));
}
bool DSU::areMerged(int v1, int v2) {
  return parent(v1) == parent(v2);
}
std::vector<int> DSU::getSet(int v) {
  int p = parent(v);
  std::vector<int> res;

  for (int i = 0; i < getNodeCount(); ++i)
    if (parent(i) == p)
      res.push_back(i);

  return res;
}

int findMaxWeighted(const std::vector<int> &weight,
                    const DSU &connectedSets,
                    const std::vector<bool> &added) {
  int best = -1;

  for (int i = 0; i < weight.size(); ++i)
    if (connectedSets.isRoot(i) && !added[i] && (best == -1 || weight[i] > weight[best]))
      best = i;

  assert(best != -1);

  return best;
}

std::vector<int> makeOppositeCut(const std::vector<int> &firstCut, int nodeCount) {
  std::vector<int> otherCut;

  for (int i = 0; i < nodeCount; ++i) {
    for (int v : firstCut)
      if (v == i)
        goto next;

    otherCut.push_back(i);
    next:;
  }

  return otherCut;
}

// Штор-Вагнер
std::pair<std::vector<int>, std::vector<int>> findMinCut(MatrixGraph &graph) {
  DSU connectedSets(graph.getNodeCount());
  std::vector<bool> added(graph.getNodeCount(), false);
  std::vector<int> weight(graph.getNodeCount(), 0);
  int minCost = INT32_MAX;
  std::vector<int> minCut;

  for (int i = 0; i < graph.getNodeCount() - 1; ++i) {
    added.assign(graph.getNodeCount(), false);
    weight.assign(graph.getNodeCount(), 0);
    int prevBest = -1;

    for (int j = 0; j < graph.getNodeCount() - i - 1; ++j) {
      int best = findMaxWeighted(weight, connectedSets, added);
      added[best] = true;

      for (int k = 0; k < graph.getNodeCount(); ++k)
        weight[k] += graph.getCost(best, k);

      prevBest = best;
    }

    int best = findMaxWeighted(weight, connectedSets, added);

    if (weight[best] < minCost) {
      minCost = weight[best];
      minCut = connectedSets.getSet(best);
    }

    connectedSets.merge(best, prevBest);
    int root = connectedSets.parent(best);

    for (int j = 0; j < graph.getNodeCount(); ++j) {
      graph.setCost(j, root, graph.getCost(j, prevBest) + graph.getCost(best, j));
      graph.setCost(root, j, graph.getCost(j, root));
    }
  }

  return std::make_pair(minCut, makeOppositeCut(minCut, graph.getNodeCount()));
}

int main() {
  int n;
  std::cin >> n;
  MatrixGraph graph(n);

  for (int i = 0; i < n; ++i) {
    std::string line;
    std::cin >> line;

    for (int j = 0; j < n; ++j) {
      graph.setCost(i, j, line[j] - '0');
    }
  }

  std::vector<int> part1, part2;
  std::tie(part1, part2) = findMinCut(graph);

  for (int v : part1)
    std::cout << (v + 1) << ' ';

  std::cout << '\n';

  for (int v : part2)
    std::cout << (v + 1) << ' ';

  std::cout << '\n';
}
