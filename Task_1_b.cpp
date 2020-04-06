//
// Created by egork on 18.02.2020.
//
// repository: https://github.com/muji-4ok/Algorithms-2-sem/tree/master
// report: https://contest.yandex.ru/contest/17228/run-report/31168366/
#include <iostream>
#include <vector>
#include <algorithm>

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
  void makeSorted(bool reverse);
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
void ListGraph::makeSorted(bool reverse = false) {
  for (std::vector<int> &kids : children)
    if (reverse)
      std::sort(kids.rbegin(), kids.rend());
    else
      std::sort(kids.begin(), kids.end());
}

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
    const std::vector<int>& children = graph.getChildren(vertex);

    for (int i = children.size() - 1; i >= 0; --i) {
      if (states[children[i]] == 1) {
        return false;
      } else if (states[children[i]] == 0) {
        stack.push_back(children[i]);
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

std::vector<int> solve(ListGraph& graph, const std::vector<bool>& isSource) {
//  graph.makeSorted(true);
  std::vector<int> path;
  std::vector<char> states(graph.getNodeCount(), 0);

  for (int i = 0; i < graph.getNodeCount(); ++i) {
    if (states[i] == 0) {
      if (!dfsWalk(i, graph, states, path))
        return {};
    }
  }

  std::reverse(path.begin(), path.end());

  return path;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  ListGraph graph(n);
  std::vector<bool> isSource(n, true);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    graph.addEdge(u, v);
    isSource[v] = false;
  }

  std::vector<int> res = solve(graph, isSource);

  if (res.empty()) {
    std::cout << "NO\n";
  } else {
    std::cout << "YES\n";

    for (int v : res)
      std::cout << v << ' ';
  }
}
