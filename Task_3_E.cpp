//
// Created by egork on 03.05.2020.
//
#include <iostream>
#include <vector>
#include <unordered_map>

struct Flow {
  explicit Flow(int flow = 0, int cap = 0) : flow(flow), cap(cap) {}

  int flow;
  int cap;
};

template<typename T>
class SetGraph {
 public:
  explicit SetGraph(int nodeCount) : children(nodeCount, std::unordered_map<int, T>()) {}
  void setEdge(int start, int finish, T cost) { children[start][finish] = cost; }
  T getEdgeCost(int start, int finish) const;
  const std::unordered_map<int, T> &getChildren(int node) const { return children[node]; }
  int getNodeCount() const { return children.size(); }
 private:
  std::vector<std::unordered_map<int, T>> children;
};

template<typename T>
T SetGraph<T>::getEdgeCost(int start, int finish) const {
  auto child = children[start].find(finish);

  if (child == children[start].end())
    return T();
  else
    return child->second;
}

bool tryFlow(SetGraph<Flow> &graph, int start, int target, std::vector<bool> &visited) {
  if (start == target)
    return true;

  visited[start] = true;

  for (auto it : graph.getChildren(start)) {
    int child = it.first;
    Flow flow = it.second;

    if (!visited[child] && flow.flow < flow.cap) {
      bool hasPath = tryFlow(graph, child, target, visited);

      if (hasPath) {
        graph.setEdge(start, child, Flow(flow.flow + 1, flow.cap));
        graph.setEdge(child, start, Flow(0, flow.cap));
        return true;
      }
    }
  }

  return false;
}

bool startFlow(SetGraph<Flow> &graph, int start, int target) {
  std::vector<bool> visited(graph.getNodeCount(), false);
  return tryFlow(graph, start, target, visited);
}

bool makePath(SetGraph<Flow> &graph,
              int start,
              int target,
              std::vector<bool> &visited,
              std::vector<int> &path) {
  path.push_back(start);

  if (start == target)
    return true;

  visited[start] = true;

  for (auto it : graph.getChildren(start)) {
    int child = it.first;
    Flow flow = it.second;

    if (!visited[child] && flow.flow > 0) {
      bool canGo = makePath(graph, child, target, visited, path);

      if (canGo)
        return true;
    }
  }

  path.pop_back();

  return false;
}

std::vector<int> constructPath(SetGraph<Flow> &graph, int start, int target) {
  std::vector<int> path;
  std::vector<bool> visited(graph.getNodeCount(), false);
  makePath(graph, start, target, visited, path);

  for (int i = 1; i < path.size(); ++i) {
    int from = path[i - 1];
    int to = path[i];
    Flow oldFlow = graph.getEdgeCost(from, to);
    graph.setEdge(from, to, Flow(oldFlow.flow - 1, oldFlow.cap));
  }

  return path;
}

int main() {
  int n, m, s, t;
  std::cin >> n >> m >> s >> t;
  --s;
  --t;
  SetGraph<Flow> graph(n);

  for (int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    --x;
    --y;
    Flow flow = graph.getEdgeCost(x, y);
    graph.setEdge(x, y, Flow(0, flow.cap + 1));
  }

  bool canMakeTwoPaths = startFlow(graph, s, t) && startFlow(graph, s, t);

  if (!canMakeTwoPaths) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n";

    for (int i = 0; i < 2; ++i) {
      std::vector<int> path = constructPath(graph, s, t);

      for (int v : path)
        std::cout << (v + 1) << ' ';

      std::cout << '\n';
    }
  }

  return 0;
}
