//
// Created by egork on 20.05.2020.
//
#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

unsigned log2(unsigned x) {
  unsigned value = 1;
  unsigned log = 0;

  while (value * 2 <= x) {
    value *= 2;
    ++log;
  }

  return log;
}

class SparseTableMin {
 public:
  explicit SparseTableMin(std::vector<int> values);
  int minOnInterval(unsigned l, unsigned r) const;
  unsigned minIndexOnInterval(unsigned l, unsigned r) const;
 private:
  std::vector<std::vector<unsigned>> table;
  std::vector<int> values;
};
SparseTableMin::SparseTableMin(std::vector<int> _values) : table(), values(std::move(_values)) {
  unsigned depth = log2(values.size());

  for (unsigned i = 0; i <= depth; ++i) {
    table.emplace_back(values.size() - (1u << i) + 1, 0);

    for (unsigned j = 0; j <= values.size() - (1u << i); ++j) {
      unsigned minL = i > 0 ? table[i - 1][j] : j;
      unsigned minR = i > 0 ? table[i - 1][j + (1u << (i - 1))] : j;
      table[i][j] = values[minL] < values[minR] ? minL : minR;
    }
  }
}
int SparseTableMin::minOnInterval(unsigned l, unsigned r) const {
  return values[minIndexOnInterval(l, r)];
}
unsigned SparseTableMin::minIndexOnInterval(unsigned l, unsigned r) const {
  unsigned depth = log2(r - l + 1);
  unsigned minL = table[depth][l];
  unsigned minR = table[depth][r - (1u << depth) + 1];
  return values[minL] < values[minR] ? minL : minR;
}

class SparseTableSecondMin {
 public:
  explicit SparseTableSecondMin(std::vector<int> values) : minTable(std::move(values)) {}
  int secondMinOnInterval(unsigned l, unsigned r) const;
 private:
  SparseTableMin minTable;
};
int SparseTableSecondMin::secondMinOnInterval(unsigned l, unsigned r) const {
  assert(r - l >= 1);
  unsigned minIndex = minTable.minIndexOnInterval(l, r);

  if (minIndex == l)
    return minTable.minOnInterval(l + 1, r);
  else if (minIndex == r)
    return minTable.minOnInterval(l, r - 1);
  else
    return std::min(minTable.minOnInterval(l, minIndex - 1),
                    minTable.minOnInterval(minIndex + 1, r));
}

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> values(n);

  for (int &x : values)
    std::cin >> x;

  SparseTableSecondMin table(values);

  for (int i = 0; i < m; ++i) {
    unsigned l, r;
    std::cin >> l >> r;
    --l;
    --r;
    std::cout << table.secondMinOnInterval(l, r) << '\n';
  }
}
