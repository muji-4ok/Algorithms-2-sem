#include <iostream>
#include "BigInteger.h"
#include "Rational.h"

BigInteger factorial(int n) {
  BigInteger result(1);

  for (int i = 1; i <= n; ++i) {
    result *= i;
    std::cout << result << '\n';
  }

  return result;
}

int main() {
  BigInteger b1;
  BigInteger b2;
  std::cin >> b1 >> b2;
//
//  std::cout << "+: " << (b1 + b2) << '\n';
//  std::cout << "*: " << (b1 * b2) << '\n';
//  std::cout << "/: " << (b1 / b2) << '\n';
//  std::cout << "%: " << (b1 % b2) << '\n';
  Rational r1(b1, b2);

  std::cout << r1.asDecimal(20) << '\n';
  std::cout << r1.toString() << '\n';

  BigInteger b3;
  BigInteger b4;
  std::cin >> b3 >> b4;

  Rational r2(b3, b4);

  std::cout << r2.asDecimal(20) << '\n';
  std::cout << r2.toString() << '\n';

  std::cout << "+: " << (r1 + r2) << '\n';
  std::cout << "-: " << (r1 - r2) << '\n';
  std::cout << "*: " << (r1 * r2) << '\n';
  std::cout << "/: " << (r1 / r2) << '\n';

  return 0;
}
