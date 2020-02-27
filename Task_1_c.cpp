//
// Created by egork on 18.02.2020.
//
// repository: https://github.com/muji-4ok/Algorithms-2-sem/tree/master
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include <memory>
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
  ListGraph makeInverted() const;

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

ListGraph ListGraph::makeInverted() const {
  ListGraph newGraph(getNodeCount());

  for (int from = 0; from < getNodeCount(); ++from)
    for (int to : getChildren(from))
      newGraph.addEdge(to, from);

  return newGraph;
}

void dfsWalk(int start, const Graph &graph, std::vector<char> &states, std::vector<int> &path) {
  std::vector<int> stack;
  stack.push_back(start);

  while (!stack.empty()) {
    int vertex = stack.back();

    if (states[vertex] == 2) {
      stack.pop_back();
      continue;
    }

    states[vertex] = 1;

    for (int child : graph.getChildren(vertex)) {
      if (states[child] == 0) {
        stack.push_back(child);
      }
    }

    while (!stack.empty() && states[stack.back()] == 1) {
      path.push_back(stack.back());
      states[stack.back()] = 2;
      stack.pop_back();
    }
  }
}

std::vector<int> dfsTree(const Graph &graph) {
  std::vector<int> path;
  std::vector<char> states(graph.getNodeCount(), 0);

  for (int i = 0; i < graph.getNodeCount(); ++i)
    if (states[i] == 0)
      dfsWalk(i, graph, states, path);

  std::reverse(path.begin(), path.end());

  return path;
}

void fillComponent(int start,
                   int componentId,
                   const Graph &graph,
                   std::vector<int> &component,
                   std::vector<int> &collection) {
  std::vector<int> stack;
  stack.push_back(start);

  while (!stack.empty()) {
    int vertex = stack.back();

    if (component[vertex] >= 0) {
      stack.pop_back();
      continue;
    }

    component[vertex] = -2;

    for (int child : graph.getChildren(vertex)) {
      if (component[child] == -1) {
        stack.push_back(child);
      }
    }

    while (!stack.empty() && component[stack.back()] == -2) {
      collection.push_back(stack.back());
      component[stack.back()] = componentId;
      stack.pop_back();
    }
  }
}

std::tuple<std::vector<int>,
           std::vector<std::vector<int>>> createComponents(const std::vector<int> &path,
                                                           const Graph &invertedGraph) {
  std::vector<std::vector<int>> fullComponents;
  std::vector<int> component(invertedGraph.getNodeCount(), -1);
  int curComponent = 0;

  for (int v : path)
    if (component[v] == -1) {
      fullComponents.emplace_back();
      fillComponent(v, curComponent, invertedGraph, component, fullComponents.back());
      ++curComponent;
    }

  return {component, fullComponents};
}

ListGraph createAggregate(const ListGraph &graph) {
  std::vector<int> path = dfsTree(graph);
  ListGraph invertedGraph = graph.makeInverted();
  std::vector<std::vector<int>> fullComponents;
  std::vector<int> component;
  std::tie(component, fullComponents) = createComponents(path, invertedGraph);

  ListGraph aggregateGraph(fullComponents.size());

  for (int i = 0; i < fullComponents.size(); ++i)
    for (int v : fullComponents[i]) {
      std::unordered_set<int> children{};

      for (int u : graph.getChildren(v))
        children.insert(component[u]);

      for (int j : children)
        if (i != j)
          aggregateGraph.addEdge(i, j);
    }

  return aggregateGraph;
}

int countFinished(const Graph *graph) {
  int count = 0;

  for (int i = 0; i < graph->getNodeCount(); ++i)
    if (graph->getChildren(i).empty())
      ++count;

  return count;
}

int solve(const ListGraph &graph) {
  ListGraph aggregateGraph = createAggregate(graph);
  ListGraph inverted = aggregateGraph.makeInverted();
  int finishCount = countFinished(&aggregateGraph);
  int startCount = countFinished(&inverted);

  if (aggregateGraph.getNodeCount() == 1)
    return 0;
  else
    return std::max(startCount, finishCount);
}

int main() {
  int n, m;
  std::cin >> n >> m;
  ListGraph graph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    --u;
    --v;
    graph.addEdge(u, v);
  }

  std::cout << solve(graph);
}