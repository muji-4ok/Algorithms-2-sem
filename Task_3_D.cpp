//
// Created by egork on 02.05.2020.
//
#include <iostream>
#include <vector>

class ListGraph {
 public:
  explicit ListGraph(int nodeCount);
  void addEdge(int start, int finish);
  const std::vector<int> &getChildren(int node) const;
  int getNodeCount() const;
 private:
  std::vector<std::vector<int>> children;
};

void ListGraph::addEdge(int start, int finish) {
  children[start].push_back(finish);
}
const std::vector<int> &ListGraph::getChildren(int node) const {
  return children[node];
}
ListGraph::ListGraph(int nodeCount) : children(nodeCount, std::vector<int>()) {
}
int ListGraph::getNodeCount() const {
  return children.size();
}

bool tryExpand(const ListGraph &graph,
               int start,
               std::vector<int> &match,
               std::vector<bool> &used) {
  if (used[start])
    return false;

  used[start] = true;

  for (int child : graph.getChildren(start)) {
    if (match[child] == -1 || tryExpand(graph, match[child], match, used)) {
      match[child] = start;
      return true;
    }
  }

  return false;
}

int parity(int i, int rowSize) {
  return (i / rowSize + i % rowSize) % 2;
}

int findMaxMatches(const ListGraph &graph, int rowSize) {
  std::vector<int> match(graph.getNodeCount(), -1);
  std::vector<bool> used(graph.getNodeCount(), false);

  for (int i = 0; i < graph.getNodeCount(); ++i) {
    used.assign(graph.getNodeCount(), false);
    tryExpand(graph, i, match, used);
  }

  int matches = 0;

  for (int i = 0; i < graph.getNodeCount(); ++i)
    if (parity(i, rowSize) && match[i] != -1)
      ++matches;

  return matches;
}

bool isHole(const std::vector<std::vector<bool>> &matrix, int i, int j) {
  if (i < 0 || i >= matrix.size())
    return false;

  if (j < 0 || j >= matrix[0].size())
    return false;

  return !matrix[i][j];
}

int flatten(int i, int j, int rowSize) {
  return i * rowSize + j;
}

ListGraph buildHoleGraph(const std::vector<std::vector<bool>> &filled, int n, int m) {
  ListGraph holes(n * m);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (isHole(filled, i, j)) {
        if (isHole(filled, i - 1, j))
          holes.addEdge(flatten(i, j, m), flatten(i - 1, j, m));

        if (isHole(filled, i + 1, j))
          holes.addEdge(flatten(i, j, m), flatten(i + 1, j, m));

        if (isHole(filled, i, j - 1))
          holes.addEdge(flatten(i, j, m), flatten(i, j - 1, m));

        if (isHole(filled, i, j + 1))
          holes.addEdge(flatten(i, j, m), flatten(i, j + 1, m));
      }
    }
  }

  return holes;
}

int main() {
  int n, m, a, b;
  std::cin >> n >> m >> a >> b;
  std::vector<std::vector<bool>> filled(n, std::vector<bool>(m, false));
  int holeCount = 0;

  for (int i = 0; i < n; ++i) {
    std::string line;
    std::cin >> line;

    for (int j = 0; j < m; ++j) {
      filled[i][j] = (line[j] == '.');

      if (!filled[i][j])
        ++holeCount;
    }
  }

  if (a >= 2 * b) {
    std::cout << b * holeCount << '\n';
    return 0;
  }

  ListGraph holes = buildHoleGraph(filled, n, m);
  int doubleCount = findMaxMatches(holes, m);

  std::cout << (doubleCount * a + (holeCount - doubleCount * 2) * b) << '\n';
}

