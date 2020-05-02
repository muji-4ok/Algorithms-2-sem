//
// Created by egork on 02.05.2020.
//
#include <iostream>
#include <vector>
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
  explicit EdgeGraph(int size, int expectedEdges = 0);
  void addEdge(int start, int finish, T cost);
  void addEdge(Edge<T> edge);
  void makeSorted();
  const std::vector<Edge<T>> &getEdges() const { return edges; }
  int getNodeCount() const { return nodeCount; }

 private:
  int nodeCount;
  std::vector<Edge<T>> edges;
};
template<typename T>
EdgeGraph<T>::EdgeGraph(int size, int expectedEdges) : nodeCount(size) {
  if (expectedEdges)
    edges.reserve(expectedEdges);
}
template<typename T>
void EdgeGraph<T>::addEdge(int start, int finish, T cost) {
  assert(0 <= start && start < nodeCount);
  assert(0 <= finish && finish < nodeCount);
  edges.emplace_back(start, finish, cost);
}
template<typename T>
void EdgeGraph<T>::addEdge(Edge<T> edge) {
  assert(0 <= edge.start && edge.start < nodeCount);
  assert(0 <= edge.finish && edge.finish < nodeCount);
  edges.push_back(edge);
}
template<typename T>
void EdgeGraph<T>::makeSorted() {
  std::sort(
      edges.begin(), edges.end(),
      [](const Edge<T> &e1, const Edge<T> &e2) -> bool {
        return e1.cost < e2.cost;
      }
  );
}

class DSU {
 public:
  explicit DSU(int vertexCount);
  void merge(int v1, int v2);
  int parent(int vertex);
  bool areMerged(int v1, int v2);
  int getNodeCount() const { return parents.size(); }
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

typedef unsigned long long ull;

EdgeGraph<ull> findMST(const EdgeGraph<ull> &graph) {
  EdgeGraph<ull> minSpanTree(graph.getNodeCount(), graph.getNodeCount() - 1);
  DSU component(graph.getNodeCount());

  for (Edge e : graph.getEdges())
    if (!component.areMerged(e.start, e.finish)) {
      component.merge(e.start, e.finish);
      minSpanTree.addEdge(e);
    }

  return minSpanTree;
}

// assume that tree is not directed
ull findTreeCost(const EdgeGraph<ull> &tree) {
  ull sum = 0;

  for (Edge e : tree.getEdges())
    sum += e.cost;

  return sum;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<ull> weights(n, UINT64_MAX);
  ull minWeightVertex = 0;

  for (int i = 0; i < n; ++i) {
    std::cin >> weights[i];

    if (weights[i] < weights[minWeightVertex])
      minWeightVertex = i;
  }

  EdgeGraph<ull> graph(n, m + n - 1);

  for (int i = 0; i < m; ++i) {
    int b, e;
    ull w;
    std::cin >> b >> e >> w;
    --b;
    --e;
    graph.addEdge(b, e, w);
  }

  for (int i = 0; i < n; ++i)
    if (i != minWeightVertex)
      graph.addEdge(i, minWeightVertex, weights[i] + weights[minWeightVertex]);

  graph.makeSorted();
  EdgeGraph<ull> minSpanTree = findMST(graph);
  std::cout << findTreeCost(minSpanTree) << '\n';
}
