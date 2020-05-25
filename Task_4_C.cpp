//
// Created by egork on 21.05.2020.
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
        childMin(2 * minPow2Greater(size) - 1, 0),
        childValue(2 * minPow2Greater(size) - 1, 0) {}
  int minOnInterval(unsigned queryL, unsigned queryR) const;
  void setOnInterval(unsigned queryL, unsigned queryR, int value);
 private:
  void setOnInterval(unsigned vertex,
                     unsigned vertexL,
                     unsigned vertexR,
                     unsigned queryL,
                     unsigned queryR,
                     int value);
  int minOnInterval(unsigned vertex,
                    unsigned vertexL,
                    unsigned vertexR,
                    unsigned queryL,
                    unsigned queryR) const;
  unsigned size;
  std::vector<int> childValue;
  std::vector<int> childMin;
};
int SegmentTree::minOnInterval(unsigned int queryL, unsigned int queryR) const {
  return minOnInterval(0, 0, size - 1, queryL, queryR);
}
int SegmentTree::minOnInterval(unsigned int vertex,
                               unsigned int vertexL,
                               unsigned int vertexR,
                               unsigned int queryL,
                               unsigned int queryR) const {
  if (queryL > queryR)
    return INT32_MAX;

  if (childValue[vertex] != -1 || (queryL == vertexL && queryR == vertexR))
    return childMin[vertex];

  unsigned vertexM = (vertexL + vertexR) / 2;

  int minL = minOnInterval(
      vertex * 2 + 1,
      vertexL,
      vertexM,
      std::max(queryL, vertexL),
      std::min(queryR, vertexM)
  );
  int minR = minOnInterval(
      vertex * 2 + 2,
      vertexM + 1,
      vertexR,
      std::max(queryL, vertexM + 1),
      std::min(queryR, vertexR)
  );

  return std::min(minL, minR);
}
void SegmentTree::setOnInterval(unsigned int queryL, unsigned int queryR, int value) {
  setOnInterval(0, 0, size - 1, queryL, queryR, value);
}
void SegmentTree::setOnInterval(unsigned int vertex,
                                unsigned int vertexL,
                                unsigned int vertexR,
                                unsigned int queryL,
                                unsigned int queryR,
                                int value) {

  if (queryL > queryR)
    return;

  if (queryL == vertexL && queryR == vertexR) {
    childValue[vertex] = value;
    childMin[vertex] = value;
    return;
  }

  unsigned vertexM = (vertexL + vertexR) / 2;

  if (childValue[vertex] != -1) {
    setOnInterval(vertex * 2 + 1, vertexL, vertexM, vertexL, vertexM, childValue[vertex]);
    setOnInterval(vertex * 2 + 2, vertexM + 1, vertexR, vertexM + 1, vertexR, childValue[vertex]);
    childValue[vertex] = -1;
  }

  setOnInterval(
      vertex * 2 + 1,
      vertexL,
      vertexM,
      std::max(queryL, vertexL),
      std::min(queryR, vertexM),
      value
  );
  setOnInterval(
      vertex * 2 + 2,
      vertexM + 1,
      vertexR,
      std::max(queryL, vertexM + 1),
      std::min(queryR, vertexR),
      value
  );
  childMin[vertex] = std::min(childMin[vertex * 2 + 1], childMin[vertex * 2 + 2]);
}

int readLight() {
  int r, g, b;
  std::cin >> r >> g >> b;
  return r + g + b;
}

int main() {
  int n;
  std::cin >> n;
  SegmentTree lights(n);

  for (unsigned i = 0; i < n; ++i) {
    lights.setOnInterval(i, i, readLight());
  }

  int k;
  std::cin >> k;

  for (int i = 0; i < k; ++i) {
    unsigned colorL, colorR;
    std::cin >> colorL >> colorR;
    int light = readLight();
    unsigned photoL, photoR;
    std::cin >> photoL >> photoR;
    lights.setOnInterval(colorL, colorR, light);
    std::cout << lights.minOnInterval(photoL, photoR) << ' ';
  }

}
