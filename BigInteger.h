//
// Created by egork on 11.03.2020.
//

#ifndef BIGINT__BIGINTEGER_H_
#define BIGINT__BIGINTEGER_H_

#include <vector>
#include <string>
#include <iostream>

// Ассерты...
#include <cassert>

#define max(a, b) ( (a) > (b) ? (a) : (b) )
#define min(a, b) ( (a) > (b) ? (b) : (a) )

template<typename T>
void reverse(std::vector<T> &vec) {
  for (int i = 0; i < vec.size() / 2; ++i)
    std::swap(vec[i], vec[vec.size() - 1 - i]);
}

void reverse(std::string &vec);

class BigInteger {
  constexpr static int RADIX_BITS = 30;
  constexpr static int RADIX = 1u << RADIX_BITS;

 public:
  BigInteger() = default;

  BigInteger(long long n);

  BigInteger &operator+=(const BigInteger &other);
  BigInteger &operator-=(const BigInteger &other);
  BigInteger& operator*= (const BigInteger &other);
  BigInteger &operator/=(const BigInteger &other);
  BigInteger &operator%=(const BigInteger &other);

  BigInteger operator-() const;
  BigInteger& operator++();
  BigInteger operator++(int);
  BigInteger& operator--();
  BigInteger operator--(int);

  // Returns modulo
  // both must be positive
  BigInteger divmod(const BigInteger &divider);

  // power must be non-negative
  BigInteger pow(const BigInteger &power) const;

  explicit operator int() const;
  explicit operator bool() const;

  std::string toString() const;

  friend BigInteger operator*(const BigInteger &left, const BigInteger &right);

  friend bool operator<(const BigInteger &left, const BigInteger &right);
  bool isPositive() const;
  void setPositive(bool value);

 private:
  explicit BigInteger(std::vector<int> &&reverseBuffer);

  void normalize();
  // Same sign
  void add(const BigInteger &other);
  // Different signs
  void subtract(const BigInteger &other);

  // Handles signs
  // other.buffer.size must be 1
  BigInteger multiplySingleDigit(const BigInteger &other) const;
  // Always positive, doesn't handle signs
  BigInteger multiplyByDigit(int d) const;
  // divident and divider sizes differ in at most 1
  // both must be positive
  int findQuotient(const BigInteger &divident, const BigInteger &divider) const;

  // Splits buffer into two parts:
  // everything before index into lower, and everything after into upper
  void divideByIndex(int index, BigInteger &lower, BigInteger &upper) const;
  void addWithOffset(const BigInteger &other, int offset);

  int addDigits(int a, int b, int &carry) const;
  int subtractDigits(int a, int b, int &carry) const;
  int multiplyDigits(int a, int b, int &carry) const;

  // Both values are positive, checks which is less
  bool isPositiveLess(const BigInteger &other) const;

  // Compare against digits in reversed order
  bool isPositiveGreater(const std::vector<int> &revBuffer) const;

  // Always just enough space to store number
  std::vector<int> buffer{0};
  bool m_isPositive = true;
};

std::ostream &operator<<(std::ostream &out, const BigInteger &bigInt);
std::istream &operator>>(std::istream &in, BigInteger &bigInt);
bool operator<=(const BigInteger &left, const BigInteger &right);
bool operator>(const BigInteger &left, const BigInteger &right);
bool operator>=(const BigInteger &left, const BigInteger &right);
bool operator==(const BigInteger &left, const BigInteger &right);
bool operator!=(const BigInteger &left, const BigInteger &right);

BigInteger operator+(const BigInteger &left, const BigInteger &right);
BigInteger operator-(const BigInteger &left, const BigInteger &right);
BigInteger operator/(const BigInteger &left, const BigInteger &right);
BigInteger operator%(const BigInteger &left, const BigInteger &right);

#endif //BIGINT__BIGINTEGER_H_
