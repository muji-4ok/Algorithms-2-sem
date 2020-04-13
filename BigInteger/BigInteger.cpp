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
void BigInteger::add(const BigInteger &other) {
  int carry = 0;
  size_t i = 0;
  buffer.reserve(max(buffer.size(), other.buffer.size()));

  for (; i < max(buffer.size(), other.buffer.size()); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = addDigits(buffer[i], i < other.buffer.size() ? other.buffer[i] : 0, carry);
  }

  if (carry > 0) {
    assert(buffer.size() == i);
    buffer.push_back(carry);
  }
}
void BigInteger::subtract(const BigInteger &other) {
  int carry = 0;
  size_t i = 0;
  const BigInteger *bigger = this;
  const BigInteger *smaller = &other;

  if (BigInteger::isLess(bigger->buffer, smaller->buffer)) {
    std::swap(bigger, smaller);
    m_isPositive = !m_isPositive;
  }

  buffer.reserve(bigger->buffer.size());

  for (; i < bigger->buffer.size(); ++i) {
    if (buffer.size() <= i)
      buffer.emplace_back();

    buffer[i] = subtractDigits(bigger->buffer[i],
                               i < smaller->buffer.size() ? smaller->buffer[i] : 0,
                               carry);
  }

  assert(carry == 0);

  normalize();
}
BigInteger &BigInteger::operator+=(const BigInteger &other) {
  if (this == &other) {
    BigInteger copy(*this);

    if (m_isPositive == other.m_isPositive)
      copy.add(other);
    else
      copy.subtract(other);

    *this = std::move(copy);
  } else {
    if (m_isPositive == other.m_isPositive)
      add(other);
    else
      subtract(other);
  }

  return *this;
}
BigInteger &BigInteger::operator-=(const BigInteger &other) {
  if (this == &other) {
    BigInteger copy(*this);

    if (m_isPositive == other.m_isPositive)
      copy.subtract(other);
    else
      copy.add(other);

    *this = std::move(copy);
  } else {
    if (m_isPositive == other.m_isPositive)
      subtract(other);
    else
      add(other);
  }

  return *this;
}
bool BigInteger::isPositive() const {
  return m_isPositive;
}
bool BigInteger::isLess(const std::vector<int> &left, const std::vector<int> &right) {
  if (left.size() < right.size()) {
    return true;
  } else if (left.size() > right.size()) {
    return false;
  } else {
    for (int i = left.size() - 1; i >= 0; --i)
      if (left[i] < right[i])
        return true;
      else if (left[i] > right[i])
        return false;

    return false;
  }
}
bool BigInteger::isGreaterReversed(const std::vector<int> &left,
                                   const std::vector<int> &revRight,
                                   bool skipZeros) {
  int zeroCount = 0;

  if (skipZeros)
    for (int i = 0; i < (int) revRight.size() - 2 && revRight[i] == 0; ++i)
      ++zeroCount;

  if (left.size() < revRight.size() - zeroCount) {
    return false;
  } else if (left.size() > revRight.size() - zeroCount) {
    return true;
  } else {
    for (int i = left.size() - 1; i >= 0; --i)
      if (left[i] < revRight[revRight.size() - 1 - i])
        return false;
      else if (left[i] > revRight[revRight.size() - 1 - i])
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
    return BigInteger::isLess(left.buffer, right.buffer);
  } else {
    return BigInteger::isLess(right.buffer, left.buffer);
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
  // Very inefficient...
//  return left <= right && left >= right;
  return left.buffer == right.buffer && left.m_isPositive == right.m_isPositive;
}
bool operator!=(const BigInteger &left, const BigInteger &right) {
  return !(left == right);
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
std::string BigInteger::toStringFixedSize(bool skipLeadingZeros, int size) const {
  std::string result;

  if (!isPositive())
    result += '-';

  int i = buffer.size() - 1;

  if (!skipLeadingZeros) {
    for (; i > 0 && !buffer[i]; --i) {
      size -= RADIX_SIZE;
      result += std::string(RADIX_SIZE, '0');
    }
  }

  std::string firstNonZero = std::to_string(buffer[i]);

  if (!skipLeadingZeros) {
    int zerosCount = size - firstNonZero.size() - RADIX_SIZE * i;

    if (zerosCount)
      result += std::string(zerosCount, '0');
  }

  result += firstNonZero;

  for (int j = i - 1; j >= 0; --j) {
    std::string str = std::to_string(buffer[j]);

    if (RADIX_SIZE - str.size())
      result += std::string(RADIX_SIZE - str.size(), '0');

    result += str;
  }

  return result;
}
std::string BigInteger::toString() const {
  std::string result;

  if (!isPositive())
    result += '-';

  result += std::to_string(buffer.back());

  for (int i = buffer.size() - 2; i >= 0; --i) {
    std::string str = std::to_string(buffer[i]);
    result += std::string(RADIX_SIZE - str.size(), '0') + str;
  }

  return result;
}
BigInteger BigInteger::divmod(const BigInteger &divider) {
  assert(divider);

  // If this == &divider
  BigInteger copy(*this);
  BigInteger::divmod(copy.buffer, divider.buffer);
  copy.m_isPositive =
      BigInteger::isEmpty(copy.buffer) ? true : !(isPositive() ^ divider.isPositive());
  BigInteger mod = (*this) - (copy * divider);
  *this = std::move(copy);

  return mod;
}
void BigInteger::nonNormalDiv(const BigInteger &divider) {
  BigInteger::divmod(buffer, divider.buffer, false);
}
std::vector<int> BigInteger::divmod(std::vector<int> &divisible,
                                    const std::vector<int> &divider,
                                    bool _normalize) {
  std::vector<int> result;
  std::vector<int> divident;
  int freeShifts = divider.size();

  for (int i = divisible.size() - 1; i >= 0; --i) {
    divident.push_back(divisible[i]);

    if (isGreaterReversed(divider, divident, true)) {
      // Isn't divisible
      // The very first one
      if (freeShifts <= 0)
        result.push_back(0);
    } else {
      reverse(divident);
      normalize(divident);

      int quotient = findQuotient(divident, divider);
      std::vector<int> multiple = multiplyByDigit(divider, quotient);
      addWithOffset(divisible, multiple, true, i, false);
      result.push_back(quotient);

      divident.clear();

      for (int j = divisible.size() - 1; j >= i; --j)
        divident.push_back(divisible[j]);
    }

    --freeShifts;
  }

  reverse(result);

  if (divisible.empty())
    divisible.push_back(0);

  if (result.empty())
    result.push_back(0);

  std::swap(divisible, result);

  if (_normalize)
    normalize(divisible);

  normalize(result);

  return result;
}
int BigInteger::findQuotient(const std::vector<int> &divident, const std::vector<int> &divider) {
  // Using binary search
  int low = 0;
  int high = RADIX;

  while (high - low > 1) {
    int median = (low + high) / 2;
    std::vector<int> multiple = multiplyByDigit(divider, median);

    if (isLess(divident, multiple))
      high = median;
    else
      low = median;
  }

  return low;
}
BigInteger &BigInteger::operator/=(const BigInteger &other) {
  this->divmod(other);
  return *this;
}
BigInteger &BigInteger::operator%=(const BigInteger &other) {
  BigInteger mod = this->divmod(other);
  *this = std::move(mod);
  return *this;
}
BigInteger::operator bool() const {
  return buffer.size() > 1 || buffer[0];
}
BigInteger operator*(const BigInteger &left, const BigInteger &right) {
  assert(!left.buffer.empty() && !right.buffer.empty());
  BigInteger result;
  result.buffer = BigInteger::multiply(left.buffer, right.buffer);
  result.m_isPositive =
      (BigInteger::isEmpty(result.buffer)) ? true : !(left.isPositive() ^ right.isPositive());
  result.normalize();
  return result;
}
BigInteger &BigInteger::operator*=(const BigInteger &other) {
  *this = *this * other;
  return *this;
}
BigInteger BigInteger::operator-() const {
  BigInteger result(*this);
  result.setPositive(!m_isPositive);

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
  if (*this)
    m_isPositive = value;
}
BigInteger BigInteger::pow(size_t power) const {
  if (!power) {
    return 1;
  }

  if (power % 2) {
    return *this * this->pow(power - 1);
  } else {
    BigInteger res = this->pow(power / 2);
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

  assert(leftStart >= 0);
  assert(rightStart >= 0);
  assert(0 <= leftEnd && leftEnd < (int) left.size());
  assert(0 <= rightEnd && rightEnd < (int) right.size());
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

  assert(leftStart >= 0);
  assert(rightStart >= 0);
  assert(0 <= leftEnd && leftEnd < (int) left.size());
  assert(0 <= rightEnd && rightEnd < (int) right.size());
  int leftSize = leftEnd - leftStart + 1;
  int rightSize = rightEnd - rightStart + 1;

  if (leftSize <= 0 || leftStart >= (int) left.size())
    return {0};

  if (rightSize <= 0 || rightStart >= (int) right.size())
    return {0};

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
                                              min(leftEnd, leftStart + index - 1),
                                              rightStart,
                                              min(rightEnd, rightStart + index - 1));
    std::vector<int> leftSum = add(left,
                                   left,
                                   leftStart,
                                   min(leftEnd, leftStart + index - 1),
                                   leftStart + index,
                                   leftEnd);
    std::vector<int> rightSum = add(right,
                                    right,
                                    rightStart,
                                    min(rightEnd, rightStart + index - 1),
                                    rightStart + index,
                                    rightEnd);
    std::vector<int> mediumMultiple = multiply(leftSum, rightSum);
    addWithOffset(mediumMultiple, smallMultiple, true);
    addWithOffset(mediumMultiple, bigMultiple, true);

    std::vector<int> result = smallMultiple;
    addWithOffset(result, bigMultiple, false, index * 2);
    addWithOffset(result, mediumMultiple, false, index);

    return result;
  }
}
std::vector<int> BigInteger::multiplyByDigit(const std::vector<int> &vec,
                                             int d,
                                             int start,
                                             int end) {
  if (end == -1)
    end = vec.size() - 1;

  if (start > end)
    return {0};

  std::vector<int> result;
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
                               int offset,
                               bool _normalize) {
  // Again, assuming that left is >= right even with offset applied, when subtracting
  int carry = 0;
  int i = 0;
  int maxSize = max((int) left.size(), (int) right.size() + offset);

  for (; i < maxSize; ++i) {
    while ((int) left.size() <= i)
      left.push_back(0);

    int r = (0 <= (i - offset) && (i - offset) < (int) right.size()) ? right[i - offset] : 0;

    if (subtract)
      left[i] = subtractDigits(left[i], r, carry);
    else
      left[i] = addDigits(left[i], r, carry);
  }

  if (carry > 0) {
    assert((int) left.size() == i);
    assert(!subtract);

    left.push_back(carry);
  }

  if (_normalize)
    normalize(left);
}
void BigInteger::normalize(std::vector<int> &vec) {
  int i = vec.size() - 1;

  while (vec[i] == 0 && i > 0) {
    vec.pop_back();
    --i;
  }
}
bool BigInteger::isEmpty(const std::vector<int> &vec) {
  return vec.size() == 1 && vec[0] == 0;
}
std::istream &operator>>(std::istream &in, BigInteger &bigInt) {
  std::string input;
  in >> input;
  bigInt = BigInteger(input);

  return in;
}
BigInteger::BigInteger(const std::string &str) {
  assert(!str.empty());
  int start = 0;

  if (str[0] == '-') {
    ++start;
  }

  auto &SIZE = BigInteger::RADIX_SIZE;
  int i = str.size() - 1;
  int j = 0;

  for (; i - SIZE + 1 >= start; i -= SIZE, ++j) {
    std::string sub = str.substr(i - SIZE + 1, SIZE);

    if ((int) buffer.size() == j)
      buffer.push_back(0);

    buffer[j] = std::stoi(sub);
  }

  if (i > start - 1) {
    if ((int) buffer.size() == j)
      buffer.push_back(0);

    buffer[j] = std::stoi(str.substr(max(start, i - SIZE + 1), i - start + 1));
  }

  normalize();
  setPositive(str[0] != '-');
}
