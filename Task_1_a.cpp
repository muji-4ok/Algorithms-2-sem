//
// Created by egork on 17.02.2020.
//
// repository: https://github.com/muji-4ok/Algorithms-2-sem/tree/master
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

struct Graph {
  virtual void addNode() = 0;
  virtual void addEdge(int start, int finish) = 0;
  virtual bool hasEdge(int start, int finish) const = 0;
  virtual const std::vector<int> &getChildren(int node) const = 0;
  virtual int getNodeCount() const = 0;
};

class ListGraph : public Graph {
 public:
  explicit ListGraph(int nodeCount);
  void addNode() override;
  void addEdge(int start, int finish) override;
  bool hasEdge(int start, int finish) const override;
  const std::vector<int> &getChildren(int node) const override;
  int getNodeCount() const override;
 public:
 private:
  std::vector<std::vector<int>> children;
};

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

// In a undirected graph
std::vector<int> getDistances(int start, const Graph &graph) {
  std::vector<int> distances(graph.getNodeCount(), 0);
  std::queue<int> queue;
  queue.push(start);

  while (!queue.empty()) {
    int vertex = queue.front();
    queue.pop();

    for (const int &child : graph.getChildren(vertex)) {
      if (child == vertex || distances[child] || child == start) continue;
      distances[child] = distances[vertex] + 1;
      queue.push(child);
    }
  }

  return distances;
}

int findMinDistance(const Graph &graph, int leon, int matilda, int milk) {
  auto distsLeon = getDistances(leon, graph);
  auto distsMatilda = getDistances(matilda, graph);
  auto distsMilk = getDistances(milk, graph);

  int minDist = distsLeon[0] + distsMatilda[0] + distsMilk[0];

  for (int i = 0; i < graph.getNodeCount(); ++i)
    minDist = std::min(minDist, distsLeon[i] + distsMatilda[i] + distsMilk[i]);

  return minDist;
}

int main() {
  int n, m, leon, matilda, milk;
  std::cin >> n >> m >> leon >> matilda >> milk;
  ListGraph graph(n);
  --leon;
  --matilda;
  --milk;

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    --u;
    --v;
    graph.addEdge(u, v);
    graph.addEdge(v, u);
  }

  std::cout << findMinDistance(graph, leon, matilda, milk);
}
