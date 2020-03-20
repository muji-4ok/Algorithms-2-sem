//
// Created by egork on 11.03.2020.
//

#include "BigInteger.h"

void BigInteger::normalize() {
  int i = buffer.size() - 1;

  while (buffer[i] == 0 && i > 0) {
    buffer.pop_back();
    --i;
  }

  if (buffer.back() == 0)
    m_isPositive = true;
}
int BigInteger::subtractDigits(int a, int b, int &carry) const {
  assert(a >= 0 && b >= 0 && carry >= 0);
  int res = a - b - carry;
  carry = res < 0;
  return (res + RADIX) % RADIX;
}
int BigInteger::addDigits(int a, int b, int &carry) const {
  assert(a >= 0 && b >= 0 && carry >= 0);
  int res = a + b + carry;
  carry = res / RADIX;
  return res % RADIX;
}
int BigInteger::multiplyDigits(int a, int b, int &carry) const {
  assert(a >= 0 && b >= 0 && carry >= 0);
  long long res = static_cast<long long>(a) * b + carry;
  carry = static_cast<int>(res / RADIX);
  return static_cast<int>(res % RADIX);
}
void BigInteger::add(const BigInteger &other) {
  int carry = 0;
  size_t i = 0;

  for (; i < other.buffer.size(); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = addDigits(buffer[i], other.buffer[i], carry);
  }

  if (carry > 0) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = carry;
  }
}
void BigInteger::subtract(const BigInteger &other) {
  int carry = 0;
  size_t i = 0;
  const BigInteger *bigger = this;
  const BigInteger *smaller = &other;

  if (bigger->isPositiveLess(*smaller)) {
    std::swap(bigger, smaller);
    m_isPositive = !m_isPositive;
  }

  for (; i < smaller->buffer.size(); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = subtractDigits(bigger->buffer[i], smaller->buffer[i], carry);
  }

  for (; i < bigger->buffer.size(); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = subtractDigits(bigger->buffer[i], 0, carry);
  }

  normalize();
}
BigInteger &BigInteger::operator+=(const BigInteger &other) {
  if (m_isPositive == other.m_isPositive) {
    this->add(other);
  } else {
    this->subtract(other);
  }

  return *this;
}
BigInteger &BigInteger::operator-=(const BigInteger &other) {
  if (m_isPositive == other.m_isPositive) {
    this->subtract(other);
  } else {
    this->add(other);
  }

  return *this;
}
bool BigInteger::isPositive() const {
  return m_isPositive;
}
bool BigInteger::isPositiveLess(const BigInteger &other) const {
  if (buffer.size() < other.buffer.size()) {
    return true;
  } else if (buffer.size() > other.buffer.size()) {
    return false;
  } else {
    for (int i = buffer.size() - 1; i >= 0; --i)
      if (buffer[i] < other.buffer[i])
        return true;
      else if (buffer[i] > other.buffer[i])
        return false;

    return false;
  }
}
bool BigInteger::isPositiveGreater(const std::vector<int> &revBuffer) const {
  if (buffer.size() < revBuffer.size()) {
    return false;
  } else if (buffer.size() > revBuffer.size()) {
    return true;
  } else {
    for (int i = buffer.size() - 1; i >= 0; --i)
      if (buffer[i] < revBuffer[buffer.size() - 1 - i])
        return false;
      else if (buffer[i] > revBuffer[buffer.size() - 1 - i])
        return true;

    return false;
  }
}
bool operator<(const BigInteger &left, const BigInteger &right) {
  if (left.isPositive() && !right.isPositive()) {
    return false;
  } else if (!left.isPositive() && right.isPositive()) {
    return true;
  } else if (left.isPositive() && right.isPositive()) {
    return left.isPositiveLess(right);
  } else {
    return right.isPositiveLess(left);
  }
}
std::ostream &operator<<(std::ostream &out, const BigInteger &bigInt) {
  out << bigInt.toString();
  return out;
}
bool operator<=(const BigInteger &left, const BigInteger &right) {
  return !(right < left);
}
bool operator>(const BigInteger &left, const BigInteger &right) {
  return right < left;
}
bool operator>=(const BigInteger &left, const BigInteger &right) {
  return !(left < right);
}
bool operator==(const BigInteger &left, const BigInteger &right) {
  return left <= right && left >= right;
}
bool operator!=(const BigInteger &left, const BigInteger &right) {
  return !(left == right);
}
void reverse(std::string &vec) {
  for (size_t i = 0; i < vec.size() / 2; ++i)
    std::swap(vec[i], vec[vec.size() - 1 - i]);
}
BigInteger operator+(const BigInteger &left, const BigInteger &right) {
  BigInteger result(left);
  result += right;
  return result;
}
BigInteger operator-(const BigInteger &left, const BigInteger &right) {
  BigInteger result(left);
  result -= right;
  return result;
}
BigInteger operator/(const BigInteger &left, const BigInteger &right) {
  BigInteger result(left);
  result /= right;
  return result;
}
BigInteger operator%(const BigInteger &left, const BigInteger &right) {
  BigInteger result(left);
  result %= right;
  return result;
}
BigInteger::BigInteger(long long n) : m_isPositive(n >= 0) {
  if (n == 0)
    return;

  if (n < 0)
    n *= -1;

  buffer.clear();

  for (int i = 0; n > 0; n /= RADIX, ++i)
    buffer.push_back(n % RADIX);
}
std::string BigInteger::toString() const {
  std::string result;

  BigInteger left(*this);
  left.m_isPositive = true;

  if (!left)
    result.push_back('0');

  while (left) {
    BigInteger mod = left.divmod(10);
    result.push_back('0' + mod.buffer[0]);
  }

  if (!isPositive())
    result += '-';

  reverse(result);

  return result;
}
BigInteger BigInteger::divmod(const BigInteger &divider) {
  assert(this->isPositive() && divider.isPositive() && divider);
  std::vector<int> resultRevBuffer{};

  while (*this >= divider) {
    int i = this->buffer.size() - 1;
    std::vector<int> revBuffer{};

    while (divider.isPositiveGreater(revBuffer))
      revBuffer.push_back(this->buffer[i--]);

    BigInteger divident(std::move(revBuffer));
    int quotient = findQuotient(divident, divider);
    BigInteger multiple = divider.multiplyByDigit(quotient);
    BigInteger subtracted;
    subtracted.buffer.clear();

    for (int j = 0; j <= i; ++j) {
      subtracted.buffer.push_back(0);
    }

    for (int j : multiple.buffer)
      subtracted.buffer.push_back(j);

    *this -= subtracted;

    resultRevBuffer.push_back(quotient);

    // We zeroed out before getting to the end of this
    // In this case we add all prepended zeroes to our result
    if (!(*this)) {
      for (int j = 0; j <= i; ++j)
        resultRevBuffer.push_back(0);
    }
  }

  if (resultRevBuffer.empty())
    resultRevBuffer.push_back(0);

  BigInteger result(std::move(resultRevBuffer));

  std::swap(*this, result);

  return result;
}
BigInteger BigInteger::multiplyByDigit(int d) const {
  assert(d >= 0);
  BigInteger result;
  int carry = 0;
  size_t i = 0;
  result.buffer.clear();

  for (; i < buffer.size(); ++i)
    result.buffer.emplace_back(multiplyDigits(buffer[i], d, carry));

  if (carry > 0)
    result.buffer.emplace_back(carry);

  result.normalize();

  return result;
}
int BigInteger::findQuotient(const BigInteger &divident, const BigInteger &divider) const {
  assert(divident.isPositive() && divider.isPositive());
  // Using binary search
  int low = 0;
  int high = RADIX;

  while (high - low > 1) {
    int median = (low + high) / 2;
    BigInteger multiple = divider.multiplyByDigit(median);

    if (divident >= multiple)
      low = median;
    else
      high = median;
  }

  return low;
}
BigInteger::BigInteger(std::vector<int> &&reverseBuffer) : buffer{} {
  while (!reverseBuffer.empty()) {
    buffer.push_back(reverseBuffer.back());
    reverseBuffer.pop_back();
  }
}
BigInteger &BigInteger::operator/=(const BigInteger &other) {
  this->divmod(other);
  return *this;
}
BigInteger &BigInteger::operator%=(const BigInteger &other) {
  *this = this->divmod(other);
  return *this;
}
BigInteger::operator int() const {
  int res = 0;
  size_t i = 0;

  do {
    res |= buffer[i] << (i * RADIX_BITS);
    ++i;
  } while (i < buffer.size() && (i * RADIX_BITS) < 32);

  if ((res >= 0) != isPositive())
    res = -res;

  return res;
}
BigInteger::operator bool() const {
  return buffer.size() > 1 || buffer[0];
}
BigInteger operator*(const BigInteger &left, const BigInteger &right) {
  assert(!left.buffer.empty() && !right.buffer.empty());

  if (left.buffer.size() == 1) {
    return right.multiplySingleDigit(left);
  } else if (right.buffer.size() == 1) {
    return left.multiplySingleDigit(right);
  } else {
//     L = a * 2 ^ (index) + b
//     R = c * 2 ^ (index) + d
    int index = max(left.buffer.size(), right.buffer.size()) / 2;
    BigInteger leftLower;
    BigInteger leftUpper;
    left.divideByIndex(index, leftLower, leftUpper);

    BigInteger rightLower;
    BigInteger rightUpper;
    right.divideByIndex(index, rightLower, rightUpper);

    BigInteger bigMultiple = leftUpper * rightUpper;
    BigInteger smallMultiple = leftLower * rightLower;
    BigInteger mediumMultiple =
        (leftLower + leftUpper) * (rightLower + rightUpper) - smallMultiple - bigMultiple;

    BigInteger result;
    result += smallMultiple;
    result.addWithOffset(mediumMultiple, index);
    result.addWithOffset(bigMultiple, index * 2);

    return result;
  }
}
void BigInteger::divideByIndex(size_t index, BigInteger &lower, BigInteger &upper) const {
  lower.buffer.clear();
  lower.m_isPositive = m_isPositive;

  for (size_t i = 0; i < index && i < buffer.size(); ++i)
    lower.buffer.push_back(buffer[i]);

  lower.normalize();

  if (buffer.size() >= index + 1) {
    upper.buffer.clear();
    upper.m_isPositive = m_isPositive;

    for (size_t i = index; i < buffer.size(); ++i)
      upper.buffer.push_back(buffer[i]);
  } else {
    upper = BigInteger();
  }
}
BigInteger BigInteger::multiplySingleDigit(const BigInteger &other) const {
  assert(other.buffer.size() == 1);
  BigInteger result = multiplyByDigit(other.buffer[0]);
  result.m_isPositive = !(isPositive() ^ other.isPositive());
  return result;
}
void BigInteger::addWithOffset(const BigInteger &other, int offset) {
  BigInteger increased;
  increased.buffer.clear();

  for (int i = 0; i < offset; ++i)
    increased.buffer.push_back(0);

  for (int j : other.buffer)
    increased.buffer.push_back(j);

  *this += increased;
}
BigInteger &BigInteger::operator*=(const BigInteger &other) {
  *this = *this * other;
  return *this;
}
BigInteger BigInteger::operator-() const {
  BigInteger result(*this);

  if (*this)
    result.m_isPositive = !m_isPositive;

  return result;
}
BigInteger &BigInteger::operator++() {
  *this += 1;
  return *this;
}
BigInteger BigInteger::operator++(int) {
  BigInteger copy(*this);
  ++(*this);
  return copy;
}
BigInteger &BigInteger::operator--() {
  *this -= 1;
  return *this;
}
BigInteger BigInteger::operator--(int) {
  BigInteger copy(*this);
  --(*this);
  return copy;
}
void BigInteger::setPositive(bool value) {
  m_isPositive = value;
}
BigInteger BigInteger::pow(const BigInteger &power) const {
  if (!power) {
    return 1;
  }

  BigInteger powerHalf(power);
  BigInteger mod2 = powerHalf.divmod(2);

  if (mod2) {
    return *this * this->pow(power - 1);
  } else {
    BigInteger res = this->pow(powerHalf);
    return res * res;
  }
}
BigInteger::BigInteger(const std::vector<int> &buffer, bool m_isPositive)
    : buffer(buffer), m_isPositive(m_isPositive) {
  normalize();
}
BigInteger BigInteger::abs() const {
  if (isPositive())
    return *this;
  else
    return -(*this);
}
std::istream &operator>>(std::istream &in, BigInteger &bigInt) {
  std::string input;
  in >> input;
  bigInt = BigInteger();
  size_t start = 0;

  if (input[0] == '-') {
    ++start;
  }

  for (size_t i = start; i < input.size(); ++i) {
    if (input[i] < '0' || input[i] > '9')
      throw std::runtime_error("Incorrect character");

    bigInt += input[i] - '0';

    if (i < input.size() - 1)
      bigInt *= 10;
  }

  if (input[0] == '-')
    bigInt = -bigInt;

  return in;
}
