//
// Created by egork on 20.05.2020.
//
#include <iostream>
#include <vector>

unsigned minPow2Greater(unsigned x) {
  unsigned value = 1;

  while (value < x)
    value *= 2;

  return value;
}

class SegmentTree {
 public:
  explicit SegmentTree(unsigned size)
      : size(size),
        childMax(2 * minPow2Greater(size) - 1, 0),
        childAdd(2 * minPow2Greater(size) - 1, 0) {}
  int maxOnInterval(unsigned queryL, unsigned queryR) const;
  void addOnInterval(unsigned queryL, unsigned queryR, int diff);
 private:
  void addOnInterval(unsigned vertex,
                     unsigned vertexL,
                     unsigned vertexR,
                     unsigned queryL,
                     unsigned queryR,
                     int diff);
  int maxOnInterval(unsigned vertex,
                    unsigned vertexL,
                    unsigned vertexR,
                    unsigned queryL,
                    unsigned queryR) const;
  unsigned size;
  std::vector<int> childAdd;
  std::vector<int> childMax;
};
int SegmentTree::maxOnInterval(unsigned int queryL, unsigned int queryR) const {
  return maxOnInterval(0, 0, size - 1, queryL, queryR);
}
int SegmentTree::maxOnInterval(unsigned int vertex,
                               unsigned int vertexL,
                               unsigned int vertexR,
                               unsigned int queryL,
                               unsigned int queryR) const {
  if (queryL > queryR)
    return INT32_MIN;

  if (queryL == vertexL && queryR == vertexR)
    return childMax[vertex] + childAdd[vertex];

  unsigned vertexM = (vertexL + vertexR) / 2;

  int maxL = maxOnInterval(
      vertex * 2 + 1,
      vertexL,
      vertexM,
      std::max(queryL, vertexL),
      std::min(queryR, vertexM)
  );
  int maxR = maxOnInterval(
      vertex * 2 + 2,
      vertexM + 1,
      vertexR,
      std::max(queryL, vertexM + 1),
      std::min(queryR, vertexR)
  );

  return std::max(maxL, maxR) + childAdd[vertex];
}
void SegmentTree::addOnInterval(unsigned int queryL, unsigned int queryR, int diff) {
  addOnInterval(0, 0, size - 1, queryL, queryR, diff);
}
void SegmentTree::addOnInterval(unsigned int vertex,
                                unsigned int vertexL,
                                unsigned int vertexR,
                                unsigned int queryL,
                                unsigned int queryR,
                                int diff) {

  if (queryL > queryR)
    return;

  if (queryL == vertexL && queryR == vertexR) {
    childAdd[vertex] += diff;
    return;
  }

  unsigned vertexM = (vertexL + vertexR) / 2;
  addOnInterval(
      vertex * 2 + 1,
      vertexL,
      vertexM,
      std::max(queryL, vertexL),
      std::min(queryR, vertexM),
      diff
  );
  addOnInterval(
      vertex * 2 + 2,
      vertexM + 1,
      vertexR,
      std::max(queryL, vertexM + 1),
      std::min(queryR, vertexR),
      diff
  );
  childMax[vertex] = std::max(childMax[vertex * 2 + 1] + childAdd[vertex * 2 + 1],
                              childMax[vertex * 2 + 2] + childAdd[vertex * 2 + 2]);
}

int main() {
  int n;
  std::cin >> n;

  SegmentTree bookedTickets(n - 1);

  for (int i = 0; i < n - 1; ++i) {
    int tickets;
    std::cin >> tickets;
    bookedTickets.addOnInterval(i, i, tickets);
  }

  int capacity;
  std::cin >> capacity;
  int m;
  std::cin >> m;

  for (int i = 0; i < m; ++i) {
    unsigned s, f;
    int tickets;
    std::cin >> s >> f >> tickets;

    if (bookedTickets.maxOnInterval(s, f - 1) + tickets > capacity) {
      std::cout << i << '\n';
    } else {
      bookedTickets.addOnInterval(s, f - 1, tickets);
    }
  }
}
