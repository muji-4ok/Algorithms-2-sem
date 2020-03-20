//
// Created by egork on 11.03.2020.
//

#ifndef BIGINT__RATIONAL_H_
#define BIGINT__RATIONAL_H_

#include "BigInteger.h"

// Only positive values
BigInteger gcd(BigInteger a, BigInteger b);

// Only positive values
BigInteger lcm(BigInteger a, BigInteger b);

class Rational {
 public:
  Rational() = default;
  Rational(long long n);
  Rational(long long p, long long q);
  Rational(const BigInteger &n);
  Rational(const BigInteger &p, const BigInteger &q);

  Rational &operator+=(const Rational &other);
  Rational &operator-=(const Rational &other);
  Rational& operator*= (const Rational &other);
  Rational &operator/=(const Rational &other);
  Rational operator-() const;

  explicit operator bool() const;
  explicit operator double() const;

  friend bool operator<(const Rational &left, const Rational &right);

  std::string toString() const;
  std::string asDecimal(size_t precision = 0) const;
  void asDecimal(BigInteger &whole, BigInteger &decimal, size_t precision = 0) const;

  bool isPositive() const;
  void setPositive(bool value);

 private:
  void normalize();

  BigInteger up{0};
  BigInteger down{1};
  bool m_isPositive{true};
};

bool operator<=(const Rational &left, const Rational &right);
bool operator>(const Rational &left, const Rational &right);
bool operator>=(const Rational &left, const Rational &right);
bool operator==(const Rational &left, const Rational &right);
bool operator!=(const Rational &left, const Rational &right);
std::ostream &operator<<(std::ostream &out, const Rational &bigInt);

Rational operator+(const Rational &left, const Rational &right);
Rational operator-(const Rational &left, const Rational &right);
Rational operator*(const Rational &left, const Rational &right);
Rational operator/(const Rational &left, const Rational &right);

#endif //BIGINT__RATIONAL_H_
