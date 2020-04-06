//
// Created by egork on 06.04.2020.
//
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

class MatrixGraph {
 public:
  explicit MatrixGraph(int size) : matrix(size, std::vector(size, false)) {}

  bool hasEdge(int start, int end) const { return matrix[start][end]; }
  void setEdge(int start, int end, bool exists) { matrix[start][end] = exists; }
  int getNodeCount() const { return matrix.size(); }

 private:
  std::vector<std::vector<bool>> matrix;
};

class ListGraph {
 public:
  explicit ListGraph(int size) : children(size, std::vector<int>()) {}

  void addEdge(int start, int end) { children[start].push_back(end); }
  const std::vector<int> &getChildren(int vertex) const { return children[vertex]; }
  int getNodeCount() const { return children.size(); }

 private:
  std::vector<std::vector<int>> children;
};

typedef unsigned long long ull;

void propagatePaths(ull *graph, int size, int ullSize) {
  for (int k = 0; k < size; ++k)
    for (int i = 0; i < size; ++i)
      if (graph[i * ullSize + k / 64] & (1ull << (k % 64)))
        for (int j = 0; j < ullSize; ++j)
          graph[i * ullSize + j] |= graph[k * ullSize + j];
}

int main() {
  int n;
  std::cin >> n;
  int ullSize = std::ceil(n / 64.0);
  auto *graph = new ull[n * ullSize];

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < ullSize; ++j)
      graph[i * ullSize + j] = 0;

  for (int i = 0; i < n; ++i) {
    std::string line;
    std::cin >> line;

    for (int j = 0; j < n; ++j) {
      bool hasEdge = line[j] == '1';

      if (hasEdge)
        graph[i * ullSize + j / 64] |= 1ull << (j % 64);
    }
  }

  propagatePaths(graph, n, ullSize);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      bool hasEdge = graph[i * ullSize + j / 64] & (1ull << (j % 64));
      std::cout << (char) ('0' + hasEdge);
    }

    std::cout << '\n';
  }
}
