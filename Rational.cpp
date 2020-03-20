//
// Created by egork on 11.03.2020.
//

#include "Rational.h"

BigInteger gcd(BigInteger a, BigInteger b) {
  assert(a >= 0 && b >= 0);
  if (b > a)
    std::swap(a, b);

  while (b) {
    BigInteger copy = b;
    b = a % b;
    a = copy;
  }

  return a;
}
BigInteger lcm(BigInteger a, BigInteger b) {
  return (a * b) / gcd(a, b);
}
void Rational::normalize() {
  m_isPositive = !(up.isPositive() ^ down.isPositive());
  up.setPositive(true);
  down.setPositive(true);
  BigInteger common = gcd(up, down);
  up /= common;
  down /= common;
}
bool Rational::isPositive() const {
  return m_isPositive;
}
Rational::Rational(const BigInteger &n) : up(n) {
  normalize();
}
Rational::Rational(const BigInteger &p, const BigInteger &q) : up(p), down(q) {
  normalize();
}
Rational::Rational(long long n) : Rational(BigInteger(n)) {

}
Rational::Rational(long long p, long long q) : Rational(BigInteger(p), BigInteger(q)) {

}
Rational &Rational::operator+=(const Rational &other) {
  BigInteger commonMultiple = lcm(down, other.down);
  up *= commonMultiple / down;
  BigInteger otherUp = other.up * commonMultiple / other.down;

  up.setPositive(m_isPositive);
  otherUp.setPositive(other.m_isPositive);

  up += otherUp;
  down = commonMultiple;
  normalize();

  return *this;
}
Rational &Rational::operator-=(const Rational &other) {
  BigInteger commonMultiple = lcm(down, other.down);
  up *= commonMultiple / down;
  BigInteger otherUp = other.up * commonMultiple / other.down;

  up.setPositive(m_isPositive);
  otherUp.setPositive(other.m_isPositive);

  up -= otherUp;
  down = commonMultiple;
  normalize();

  return *this;
}
Rational &Rational::operator*=(const Rational &other) {
  up *= other.up;
  down *= other.down;
  m_isPositive = !(m_isPositive ^ other.m_isPositive);
  normalize();

  return *this;
}
Rational &Rational::operator/=(const Rational &other) {
  assert(other);
  up *= other.down;
  down *= other.up;
  m_isPositive = !(m_isPositive ^ other.m_isPositive);
  normalize();

  return *this;
}
Rational Rational::operator-() const {
  Rational copy(*this);
  copy.m_isPositive = !m_isPositive;
  return copy;
}
Rational::operator bool() const {
  return static_cast<bool>(up);
}
std::string Rational::toString() const {
  std::string result;

  if (!isPositive())
    result += '-';

  result += up.toString();

  if (down > 1) {
    result += '/';
    result += down.toString();
  }

  return result;
}
void Rational::setPositive(bool value) {
  m_isPositive = value;
}
bool operator<(const Rational &left, const Rational &right) {
  if (left.isPositive() && !right.isPositive()) {
    return false;
  } else if (!left.isPositive() && right.isPositive()) {
    return true;
  } else {
    BigInteger commonMultiple = lcm(left.down, right.down);
    BigInteger leftUp = left.up * commonMultiple / left.down;
    BigInteger rightUp = right.up * commonMultiple / right.down;

    leftUp.setPositive(left.m_isPositive);
    rightUp.setPositive(right.m_isPositive);

    return leftUp < rightUp;
  }
}
std::string Rational::asDecimal(size_t precision) const {
  std::string result;

  if (!isPositive())
    result += '-';

  BigInteger whole;
  BigInteger decimal;
  asDecimal(whole, decimal, precision);

  result += whole.toString();
  result += ',';
  result += decimal.toString();

  return result;
}
void Rational::asDecimal(BigInteger &whole, BigInteger &decimal, size_t precision) const {
  BigInteger pow10 = BigInteger(10).pow(precision);
  whole = up;
  decimal = whole.divmod(down);
  decimal = (decimal * pow10) / down;
}
Rational::operator double() const {
  return std::stod(asDecimal(17));
}
bool operator<=(const Rational &left, const Rational &right) {
  return !(right < left);
}
bool operator>(const Rational &left, const Rational &right) {
  return right < left;
}
bool operator>=(const Rational &left, const Rational &right) {
  return !(left < right);
}
bool operator==(const Rational &left, const Rational &right) {
  return left <= right && left >= right;
}
bool operator!=(const Rational &left, const Rational &right) {
  return !(left == right);
}
std::ostream &operator<<(std::ostream &out, const Rational &rational) {
  out << rational.toString();
  return out;
}
Rational operator+(const Rational &left, const Rational &right) {
  Rational result(left);
  result += right;
  return result;
}
Rational operator-(const Rational &left, const Rational &right) {
  Rational result(left);
  result -= right;
  return result;
}
Rational operator*(const Rational &left, const Rational &right) {
  Rational result(left);
  result *= right;
  return result;
}
Rational operator/(const Rational &left, const Rational &right) {
  Rational result(left);
  result /= right;
  return result;
}
