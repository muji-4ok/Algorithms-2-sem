//
// Created by egork on 02.05.2020.
//
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cassert>

class SetGraph {
 public:
  explicit SetGraph(int nodeCount);
  void addNode();
  void addEdge(int start, int finish, int cost);
  int getEdgeCost(int start, int finish) const;
  const std::unordered_map<int, int> &getChildren(int node) const;
  int getNodeCount() const;
 private:
  std::vector<std::unordered_map<int, int>> children;
};

void SetGraph::addNode() {
  children.emplace_back();
}

void SetGraph::addEdge(int start, int finish, int cost) {
  children[start][finish] = cost;
}

const std::unordered_map<int, int> &SetGraph::getChildren(int node) const {
  return children[node];
}

SetGraph::SetGraph(int nodeCount) : children(nodeCount, std::unordered_map<int, int>()) {
}

int SetGraph::getNodeCount() const {
  return children.size();
}
int SetGraph::getEdgeCost(int start, int finish) const {
  auto child = children[start].find(finish);

  if (child == children[start].end())
    return -1;
  else
    return child->second;
}

std::vector<int> findMinSpanTree(const SetGraph &graph) {
  std::vector<int> minEdgeCost(graph.getNodeCount(), INT32_MAX);
  std::vector<bool> used(graph.getNodeCount(), false);
  std::vector<int> parent(graph.getNodeCount(), -1);
  // (dist, vertex)
  std::set<std::pair<int, int>> queue;
  queue.emplace(0, 0);
  minEdgeCost[0] = 0;

  while (!queue.empty()) {
    auto bestNode = queue.extract(queue.begin());
    int vertex = bestNode.value().second;

    if (used[vertex])
      continue;

    used[vertex] = true;

    for (auto child : graph.getChildren(vertex)) {
      int childV = child.first;
      int edgeCost = child.second;

      if (!used[childV] && edgeCost < minEdgeCost[childV]) {
        queue.erase(std::make_pair(minEdgeCost[childV], childV));
        parent[childV] = vertex;
        minEdgeCost[childV] = edgeCost;
        queue.emplace(minEdgeCost[childV], childV);
      }
    }
  }

  return parent;
}

typedef long long ll;

ll findTreeCost(const SetGraph &graph, const std::vector<int> &treeParents) {
  ll sum = 0;

  for (int v = 0; v < graph.getNodeCount(); ++v) {
    int u = treeParents[v];

    if (u != -1) {
      ll cost = graph.getEdgeCost(u, v);
      assert(cost >= 0);
      sum += cost;
    }
  }

  return sum;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  SetGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int b, e, w;
    std::cin >> b >> e >> w;
    --b;
    --e;
    graph.addEdge(b, e, w);
    graph.addEdge(e, b, w);
  }

  std::vector<int> treeParents = findMinSpanTree(graph);
  std::cout << findTreeCost(graph, treeParents) << '\n';
}
