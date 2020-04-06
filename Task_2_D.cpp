//
// Created by egork on 06.04.2020.
//
#include <iostream>
#include "PackedBitMatrix.h"

class PackedMatrixGraph {
 public:
  explicit PackedMatrixGraph(int size) : matrix(size) {}

  bool hasEdge(int start, int end) const { return matrix.getBit(start, end); }
  void setEdge(int start, int end, bool exists) { matrix.setBit(start, end, exists); }
  ull edgesBitBlock(int start, int end) const { return matrix.bitBlock(start, end); }
  ull &edgesBitBlock(int start, int end) { return matrix.bitBlock(start, end); }
  int getNodeCount() const { return matrix.getRowCount(); }
  int getRowSize() const { return matrix.getBlockCount(); }

 private:
  PackedBitMatrix matrix;
};

void propagatePaths(PackedMatrixGraph &graph) {
  for (int k = 0; k < graph.getNodeCount(); ++k)
    for (int i = 0; i < graph.getNodeCount(); ++i)
      if (graph.hasEdge(i, k))
        for (int j = 0; j < graph.getRowSize(); ++j)
          graph.edgesBitBlock(i, j) |= graph.edgesBitBlock(k, j);
}

int main() {
  int n;
  std::cin >> n;
  PackedMatrixGraph graph(n);

  for (int i = 0; i < n; ++i) {
    std::string line;
    std::cin >> line;

    for (int j = 0; j < n; ++j)
      graph.setEdge(i, j, line[j] == '1');
  }

  propagatePaths(graph);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      std::cout << (char)('0' + graph.hasEdge(i, j));

    std::cout << '\n';
  }
}
