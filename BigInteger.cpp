//
// Created by egork on 11.03.2020.
//

#include "BigInteger.h"

BigInteger::BigInteger(const std::vector<int> &buffer, bool isPositive)
    : buffer(buffer), m_isPositive(isPositive) {
  normalize();
}
void BigInteger::normalize() {
  int i = buffer.size() - 1;

  while (buffer[i] == 0 && i > 0) {
    buffer.pop_back();
    --i;
  }

  if (buffer.back() == 0)
    m_isPositive = true;
}
int BigInteger::subtractDigits(int a, int b, int &carry) {
  assert(a >= 0 && b >= 0 && carry >= 0);
  int res = a - b - carry;
  carry = res < 0;
  return (res + RADIX) % RADIX;
}
int BigInteger::addDigits(int a, int b, int &carry) {
  assert(a >= 0 && b >= 0 && carry >= 0);
  int res = a + b + carry;
  carry = res / RADIX;
  return res % RADIX;
}
int BigInteger::multiplyDigits(int a, int b, int &carry) {
  assert(a >= 0 && b >= 0 && carry >= 0);
  long long res = static_cast<long long>(a) * b + carry;
  carry = static_cast<int>(res / RADIX);
  return static_cast<int>(res % RADIX);
}
// TODO: optimize - add
void BigInteger::add(const BigInteger &other) {
  int carry = 0;
  size_t i = 0;
  buffer.reserve(max(buffer.size(), other.buffer.size()));

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
// TODO: optimize - subtract
void BigInteger::subtract(const BigInteger &other) {
  int carry = 0;
  size_t i = 0;
  const BigInteger *bigger = this;
  const BigInteger *smaller = &other;

  if (bigger->isPositiveLess(*smaller)) {
    std::swap(bigger, smaller);
    m_isPositive = !m_isPositive;
  }

  buffer.reserve(bigger->buffer.size());

  for (; i < smaller->buffer.size(); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = subtractDigits(bigger->buffer[i], smaller->buffer[i], carry);
  }

  // Last one might have carry
  if (carry) {
    buffer[i] = subtractDigits(bigger->buffer[i], 0, carry);
    ++i;
  }

  for (; i < bigger->buffer.size(); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = bigger->buffer[i];
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
  if (n < 0)
    n *= -1;

  for (int i = 0; n > 0; n /= RADIX, ++i)
    buffer[i] = n % RADIX;
}
// TODO: optimize - toString
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
// TODO: optimize - divmod
BigInteger BigInteger::divmod(const BigInteger &divider) {
  assert(this->isPositive() && divider.isPositive() && divider);
  std::vector<int> resultRevBuffer;

  while (*this >= divider) {
    int i = this->buffer.size() - 1;
    std::vector<int> revBuffer;

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
  if (buffer.size() == 1)
    return buffer[0] * (isPositive() ? 1 : -1);
  else
    return ((static_cast<long long>(buffer[1]) << RADIX_BITS) + buffer[0])
        * (isPositive() ? 1 : -1);
}
BigInteger::operator bool() const {
  return buffer.size() > 1 || buffer[0];
}
// TODO: optimize - multiply
BigInteger operator*(const BigInteger &left, const BigInteger &right) {
  assert(!left.buffer.empty() && !right.buffer.empty());
  BigInteger result;
  result.m_isPositive = !(left.isPositive() ^ right.isPositive());
  result.buffer = BigInteger::multiply(left.buffer, right.buffer);
  result.normalize();
  return result;
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
int BigInteger::fastMod2() const {
  return buffer[0] % 2;
}
std::vector<int> BigInteger::add(const std::vector<int> &left,
                                 const std::vector<int> &right,
                                 int leftStart,
                                 int leftEnd,
                                 int rightStart,
                                 int rightEnd) {
  if (leftEnd == -1)
    leftEnd = left.size() - 1;

  if (rightEnd == -1)
    rightEnd = right.size() - 1;

  int leftSize = leftEnd - leftStart + 1;
  int rightSize = rightEnd - rightStart + 1;

  std::vector<int> result(max(leftSize, rightSize), 0);
  int carry = 0;
  int i = 0;

  for (; i < max(leftSize, rightSize); ++i) {
    int l = leftStart + i <= leftEnd ? left[leftStart + i] : 0;
    int r = rightStart + i <= rightEnd ? right[rightStart + i] : 0;
    result[i] = addDigits(l, r, carry);
  }

  if (carry > 0) {
    if (static_cast<int>(result.size()) <= i)
      result.emplace_back();

    result[i] = carry;
  }

  return result;
}
std::vector<int> BigInteger::multiply(const std::vector<int> &left,
                                      const std::vector<int> &right,
                                      int leftStart,
                                      int leftEnd,
                                      int rightStart,
                                      int rightEnd) {
  if (leftEnd == -1)
    leftEnd = left.size() - 1;

  if (rightEnd == -1)
    rightEnd = right.size() - 1;

  int leftSize = leftEnd - leftStart + 1;
  int rightSize = rightEnd - rightStart + 1;

  if (leftSize == 1) {
    return multiplyByDigit(right, left[leftStart], rightStart, rightEnd);
  } else if (rightSize == 1) {
    return multiplyByDigit(left, right[rightStart], leftStart, leftEnd);
  } else {
    int index = max(leftSize, rightSize) / 2;

    std::vector<int> bigMultiple = multiply(left,
                                            right,
                                            leftStart + index,
                                            leftEnd,
                                            rightStart + index,
                                            rightEnd);
    std::vector<int> smallMultiple = multiply(left,
                                              right,
                                              leftStart,
                                              leftStart + index - 1,
                                              rightStart,
                                              rightStart + index - 1);
    std::vector<int> leftSum = add(left, left,
                                   leftStart, leftStart + index - 1,
                                   leftStart + index, leftEnd);
    std::vector<int> rightSum = add(right, right,
                                    rightStart, rightStart + index - 1,
                                    rightStart + index, rightEnd);
    std::vector<int> mediumMultiple = multiply(leftSum, rightSum);
    addWithOffset(mediumMultiple, smallMultiple, true);
    addWithOffset(mediumMultiple, bigMultiple, true);

    std::vector<int> result = smallMultiple;
    addWithOffset(result, bigMultiple, true, index * 2);
    addWithOffset(result, mediumMultiple, true, index);

    return result;
  }
}
std::vector<int> BigInteger::multiplyByDigit(const std::vector<int> &vec,
                                             int d,
                                             int start,
                                             int end) {
  if (end == -1)
    end = vec.size() - 1;

  std::vector<int> result;
//  result.reserve(vec.size());
  int carry = 0;
  int i = start;

  for (; i <= end; ++i)
    result.emplace_back(multiplyDigits(vec[i], d, carry));

  if (carry > 0)
    result.emplace_back(carry);

  normalize(result);

  return result;
}
void BigInteger::addWithOffset(std::vector<int> &left,
                               const std::vector<int> &right,
                               bool subtract,
                               int offset) {
  int carry = 0;
  size_t i = offset;

  for (; i < max(left.size(), right.size()); ++i) {
    if (left.size() <= i)
      left.push_back(0);

    int r = i < right.size() ? right[i] : 0;

    if (subtract)
      left[i] = subtractDigits(left[i], r, carry);
    else
      left[i] = addDigits(left[i], r, carry);
  }

  if (carry > 0) {
    if (left.size() <= i)
      left.emplace_back();

    left[i] = carry;
  }

  normalize(left);
}
void BigInteger::normalize(std::vector<int> &vec) {
  int i = vec.size() - 1;

  while (vec[i] == 0 && i > 0) {
    vec.pop_back();
    --i;
  }
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
