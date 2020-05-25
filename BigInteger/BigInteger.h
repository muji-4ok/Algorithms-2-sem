//
// Created by egork on 11.03.2020.
//

#ifndef BIGINT__BIGINTEGER_H_
#define BIGINT__BIGINTEGER_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>

#ifndef max
#define max(a, b) ( (a) > (b) ? (a) : (b) )
#endif
#ifndef min
#define min(a, b) ( (a) > (b) ? (b) : (a) )
#endif

template<typename T>
void reverse(std::vector<T> &vec) {
  for (size_t i = 0; i < vec.size() / 2; ++i)
    std::swap(vec[i], vec[vec.size() - 1 - i]);
}

class BigInteger {
 public:
  constexpr static int RADIX_SIZE = 9;
  constexpr static int RADIX = 1'000'000'000;

  BigInteger() = default;

  BigInteger(long long n);
  BigInteger(const std::vector<int> &buffer, bool m_isPositive);
  BigInteger(const std::string &str);

  BigInteger &operator+=(const BigInteger &other);
  BigInteger &operator-=(const BigInteger &other);
  BigInteger &operator*=(const BigInteger &other);
  BigInteger &operator/=(const BigInteger &other);
  BigInteger &operator%=(const BigInteger &other);

  BigInteger operator-() const;
  BigInteger &operator++();
  BigInteger operator++(int);
  BigInteger &operator--();
  BigInteger operator--(int);

  // Returns modulo
  // Ignores signs
  BigInteger divmod(const BigInteger &divider);
  void nonNormalDiv(const BigInteger &divider);

  BigInteger pow(size_t power) const;

  explicit operator bool() const;

  std::string toString() const;
  // Used for printing strings with possible leading zeros, size is not trimming
  // Very specific
  std::string toStringFixedSize(bool skipLeadingZeros, int size) const;

  BigInteger abs() const;

  friend BigInteger operator*(const BigInteger &left, const BigInteger &right);

  friend bool operator==(const BigInteger &left, const BigInteger &right);
  friend bool operator<(const BigInteger &left, const BigInteger &right);
  bool isPositive() const;
  void setPositive(bool value);

 private:
  void normalize();
  // Same sign
  void add(const BigInteger &other);
  // Different signs
  void subtract(const BigInteger &other);

  // divident and divider sizes differ in at most 1
  // both must be positive
  static int findQuotient(const std::vector<int> &divident, const std::vector<int> &divider);
  // Returns modulo
  // Normalize divisible or not
  static std::vector<int> divmod(std::vector<int> &divisible,
                                 const std::vector<int> &divider,
                                 bool _normalize = true);

  // Ends are vec.size() - 1 by default
  static std::vector<int> add(const std::vector<int> &left,
                              const std::vector<int> &right,
                              int leftStart = 0,
                              int leftEnd = -1,
                              int rightStart = 0,
                              int rightEnd = -1);
  // Ends are vec.size() - 1 by default
  static std::vector<int> multiply(const std::vector<int> &left,
                                   const std::vector<int> &right,
                                   int leftStart = 0,
                                   int leftEnd = -1,
                                   int rightStart = 0,
                                   int rightEnd = -1);
  // End is vec.size() - 1 by default
  static std::vector<int> multiplyByDigit(const std::vector<int> &vec,
                                          int d,
                                          int start = 0,
                                          int end = -1);
  // If subtracting, left *MUST* be >= right
  static void addWithOffset(std::vector<int> &left,
                            const std::vector<int> &right,
                            bool subtract = false,
                            int offset = 0,
                            bool _normalize = true);
  // Removes leading zeros
  static void normalize(std::vector<int> &vec);

  // zero <=> (size == 1 && vec[0] == 0)
  static bool isEmpty(const std::vector<int> &vec);

  static int addDigits(int a, int b, int &carry);
  static int subtractDigits(int a, int b, int &carry);
  static int multiplyDigits(int a, int b, int &carry);

  static bool isLess(const std::vector<int> &left, const std::vector<int> &right);
  // Second vec is in reversed order
  // skipZeros - only revRights, leading(preceding) zeros
  static bool isGreaterReversed(const std::vector<int> &left,
                                const std::vector<int> &revRight,
                                bool skipZeros = false);

  // Always just enough space to store number
  std::vector<int> buffer{0};
  bool m_isPositive = true;
};

std::istream &operator>>(std::istream &in, BigInteger &bigInt);
std::ostream &operator<<(std::ostream &out, const BigInteger &bigInt);
bool operator<=(const BigInteger &left, const BigInteger &right);
bool operator>(const BigInteger &left, const BigInteger &right);
bool operator>=(const BigInteger &left, const BigInteger &right);
bool operator!=(const BigInteger &left, const BigInteger &right);

BigInteger operator+(const BigInteger &left, const BigInteger &right);
BigInteger operator-(const BigInteger &left, const BigInteger &right);
BigInteger operator/(const BigInteger &left, const BigInteger &right);
BigInteger operator%(const BigInteger &left, const BigInteger &right);

#endif //BIGINT__BIGINTEGER_H_
