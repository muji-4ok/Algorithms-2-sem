#pragma once
#include <cmath>
#include <cstring>

typedef unsigned long long ull;

class PackedBitMatrix {
 public:
  static constexpr ull BLOCK_BIT_COUNT = 64;

  explicit PackedBitMatrix(int size);

  PackedBitMatrix(const PackedBitMatrix &other);
  PackedBitMatrix(PackedBitMatrix &&other) noexcept;
  PackedBitMatrix &operator=(const PackedBitMatrix &other);
  PackedBitMatrix &operator=(PackedBitMatrix &&other) noexcept;
  ~PackedBitMatrix() { delete[] matrix; }

  bool getBit(int i, int j) const;
  void setBit(int i, int j, bool value);

  ull bitBlock(int i, int j) const;
  ull &bitBlock(int i, int j);

  int getRowCount() const { return rowCount; }
  int getBlockCount() const { return blockCount; }

 private:
  int rowCount;
  int blockCount;
  ull *matrix;
};

