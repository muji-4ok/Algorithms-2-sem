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
// TODO: optimize - add
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
// TODO: optimize - subtract
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
bool BigInteger::isGreaterReversed(const std::vector<int> &left, const std::vector<int> &revRight) {
  if (left.size() < revRight.size()) {
    return false;
  } else if (left.size() > revRight.size()) {
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
// TODO: optimize - toString
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
  BigInteger mod;
  mod.buffer = BigInteger::divmod(buffer, divider.buffer);
  return mod;
}
std::vector<int> BigInteger::divmod(std::vector<int> &divisible, const std::vector<int> &divider) {
  // Reversed at first
  std::vector<int> result{};
  bool firstShift = true;
  int freeShifts = 2;

  while (!(isLess(divisible, divider))) {
    int i = divisible.size() - 1;
    // Reversed at first
    std::vector<int> divident{};

    while (isGreaterReversed(divider, divident)) {
      divident.push_back(divisible[i--]);

      // Cannot divide without shifting, so we "multiply" by zero
      // At first we don't care, because we have zeroes only
      if (!firstShift && freeShifts <= 0)
        result.push_back(0);

      --freeShifts;
    }

    firstShift = false;
    reverse(divident);

    int quotient = findQuotient(divident, divider);
    std::vector<int> multiple = multiplyByDigit(divider, quotient);
    addWithOffset(divisible, multiple, true, i + 1, false);

    result.push_back(quotient);

    int zeroCount = 0;

    while (divisible.size() > 1 && divisible.back() == 0) {
      divisible.pop_back();
      ++zeroCount;
    }

    if (isEmpty(divisible))
      ++zeroCount;

    bool exact = (multiple == divident);
    freeShifts = 1 + !exact;

    for (int j = 0; j < zeroCount - (int) multiple.size(); ++j)
      result.push_back(0);

    if (!isEmpty(divisible) && isLess(divisible, divider) && exact)
      result.push_back(0);
  }

  if (result.empty())
    result.push_back(0);

  reverse(result);

  std::swap(divisible, result);

  // Just in case
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
  int ret = buffer[0];

  if (buffer.size() > 1)
    ret += buffer[0] * RADIX;

  if (!isPositive())
    ret = -ret;

  return ret;
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

  if (leftSize <= 0 || rightSize <= 0 || leftStart >= static_cast<int>(left.size())
      || rightStart >= static_cast<int>(right.size()))
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
  bigInt = BigInteger();
  int start = 0;

  if (input[0] == '-') {
    ++start;
  }

  auto &SIZE =  BigInteger::RADIX_SIZE;
  int i = input.size() - 1;
  int j = 0;

  for (; i - SIZE + 1 >= start; i -= SIZE, ++j) {
    std::string sub = input.substr(i - SIZE + 1, SIZE);

    if ((int) bigInt.buffer.size() == j)
      bigInt.buffer.push_back(0);

    bigInt.buffer[j] = std::stoi(sub);
  }

  if ((int) bigInt.buffer.size() == j)
    bigInt.buffer.push_back(0);

  bigInt.buffer[j] = std::stoi(input.substr(max(start, i - SIZE + 1), i + 1));

  if (bigInt)
    bigInt.setPositive(input[0] != '-');

  bigInt.normalize();

  return in;
}
