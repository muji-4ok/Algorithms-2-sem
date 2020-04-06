#include <cassert>
#include "PackedBitMatrix.h"

PackedBitMatrix::PackedBitMatrix(int size) {
  rowCount = size;
  blockCount = std::ceil(size / (double) BLOCK_BIT_COUNT);
  matrix = new ull[rowCount * blockCount];
  std::memset(matrix, 0, rowCount * blockCount * sizeof(ull));
}
PackedBitMatrix::PackedBitMatrix(const PackedBitMatrix &other)
    : rowCount(other.rowCount), blockCount(other.blockCount) {
  matrix = new ull[rowCount * blockCount];
  std::memcpy(matrix, other.matrix, rowCount * blockCount * sizeof(ull));
}
PackedBitMatrix::PackedBitMatrix(PackedBitMatrix &&other) noexcept
    : rowCount(other.rowCount), blockCount(other.blockCount) {
  matrix = other.matrix;
  other.matrix = nullptr;
}
PackedBitMatrix &PackedBitMatrix::operator=(const PackedBitMatrix &other) {
  if (this == &other)
    return *this;

  rowCount = other.rowCount;
  blockCount = other.blockCount;
  delete matrix;
  matrix = new ull[rowCount * blockCount];
  std::memcpy(matrix, other.matrix, rowCount * blockCount * sizeof(ull));

  return *this;
}
PackedBitMatrix &PackedBitMatrix::operator=(PackedBitMatrix &&other) noexcept {
  if (this == &other)
    return *this;

  std::swap(rowCount, other.rowCount);
  std::swap(blockCount, other.blockCount);
  std::swap(matrix, other.matrix);

  return *this;
}
bool PackedBitMatrix::getBit(int i, int j) const {
  assert(0 <= i && i < rowCount);
  assert(0 <= j && j < rowCount);
  return matrix[i * blockCount + j / BLOCK_BIT_COUNT] & (1ull << (j % BLOCK_BIT_COUNT));
}
void PackedBitMatrix::setBit(int i, int j, bool value) {
  assert(0 <= i && i < rowCount);
  assert(0 <= j && j < rowCount);
  matrix[i * blockCount + j / BLOCK_BIT_COUNT] &= ~(1ull << (j % BLOCK_BIT_COUNT));
  matrix[i * blockCount + j / BLOCK_BIT_COUNT] |= ((ull) value << (j % BLOCK_BIT_COUNT));
}
ull PackedBitMatrix::bitBlock(int i, int j) const {
  assert(0 <= i && i < rowCount);
  assert(0 <= j && j < blockCount);
  return matrix[i * blockCount + j];
}
ull &PackedBitMatrix::bitBlock(int i, int j) {
  assert(0 <= i && i < rowCount);
  assert(0 <= j && j < blockCount);
  return matrix[i * blockCount + j];
}
