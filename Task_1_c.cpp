//
// Created by egork on 18.02.2020.
//
// repository: https://github.com/muji-4ok/Algorithms-2-sem/tree/master
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <queue>

struct Graph {
  virtual void addNode() = 0;
  virtual void addEdge(int start, int finish) = 0;
  virtual bool hasEdge(int start, int finish) const = 0;
  virtual const std::vector<int> &getChildren(int node) const = 0;
  virtual int getNodeCount() const = 0;
};

class MatrixGraph : public Graph {
 public:
  explicit MatrixGraph(int nodeCount);
  void addNode() override;
  void addEdge(int start, int finish) override;
  bool hasEdge(int start, int finish) const override;
  const std::vector<int> &getChildren(int node) const override;
  int getNodeCount() const override;
  bool isStart(int node) const;
  bool isFinish(int node) const;

 private:
  std::vector<std::vector<bool>> matrix;
};

MatrixGraph::MatrixGraph(int nodeCount) : matrix(nodeCount, std::vector<bool>(nodeCount, false)) {
}
void MatrixGraph::addNode() {
  for (auto &i : matrix)
    i.push_back(false);

  matrix.emplace_back(getNodeCount() + 1, false);
}
void MatrixGraph::addEdge(int start, int finish) {
  matrix[start][finish] = true;
}
bool MatrixGraph::hasEdge(int start, int finish) const {
  return matrix[start][finish];
}
const std::vector<int> &MatrixGraph::getChildren(int node) const {
  std::vector<int> children;

  for (int i = 0; i < getNodeCount(); ++i)
    if (matrix[node][i])
      children.push_back(i);
}
int MatrixGraph::getNodeCount() const {
  return matrix.size();
}
bool MatrixGraph::isStart(int node) const {
  for (int i = 0; i < getNodeCount(); ++i)
    if (i != node && hasEdge(i, node))
      return false;

  return true;
}
bool MatrixGraph::isFinish(int node) const {
  for (int i = 0; i < getNodeCount(); ++i)
    if (i != node && hasEdge(node, i))
      return false;

  return true;
}

class ListGraph : public Graph {
 public:
  explicit ListGraph(int nodeCount);
  void addNode() override;
  void addEdge(int start, int finish) override;
  bool hasEdge(int start, int finish) const override;
  const std::vector<int> &getChildren(int node) const override;
  int getNodeCount() const override;
  void makeSorted(bool reverse);
  ListGraph makeInverted() const;
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
ListGraph ListGraph::makeInverted() const {
  ListGraph newGraph(getNodeCount());

  for (int from = 0; from < getNodeCount(); ++from)
    for (int to : getChildren(from))
      newGraph.addEdge(to, from);

  return newGraph;
}

void dfs(int start, const Graph *graph, std::vector<bool> &visited, std::vector<int> &path) {
  visited[start] = true;

  for (int child : graph->getChildren(start))
    if (!visited[child])
      dfs(child, graph, visited, path);

  path.push_back(start);
}

void fillComponent(int start, int componentId,
                   const Graph *graph,
                   std::vector<int> &component,
                   std::vector<int> &collection) {
  component[start] = componentId;
  collection.push_back(start);

  for (int child : graph->getChildren(start))
    if (component[child] == -1)
      fillComponent(child, componentId, graph, component, collection);
}

ListGraph createAggregate(const ListGraph &graph) {
  std::vector<bool> visited(graph.getNodeCount(), false);
  std::vector<int> path;

  for (int i = 0; i < graph.getNodeCount(); ++i)
    if (!visited[i])
      dfs(i, &graph, visited, path);

  ListGraph invertedGraph = graph.makeInverted();
  std::vector<std::vector<int>> fullComponents;
  std::vector<int> component(graph.getNodeCount(), -1);
  int curComponent = 0;

  for (auto it = path.rbegin(); it != path.rend(); ++it)
    if (component[*it] == -1) {
      fullComponents.emplace_back();
      fillComponent(*it, curComponent, &invertedGraph, component, fullComponents.back());
      ++curComponent;
    }

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

int solve(const ListGraph& graph) {
  ListGraph aggregateGraph = createAggregate(graph);
  int finishCount = 0;

  for (int i = 0; i < aggregateGraph.getNodeCount(); ++i)
    if (aggregateGraph.getChildren(i).empty())
      ++finishCount;

  ListGraph inverted = aggregateGraph.makeInverted();
  int startCount = 0;

  for (int i = 0; i < inverted.getNodeCount(); ++i)
    if (inverted.getChildren(i).empty())
      ++startCount;

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