#include <iostream>
#include "BigInteger.h"
#include "Rational.h"
#include <ctime>
#include <cstdio>

BigInteger factorial(int n) {
  BigInteger result(1);

  for (int i = 1; i <= n; ++i) {
    result *= i;
    std::cout << result << '\n';
  }

  return result;
}

int main() {
  std::cout << factorial(100);
//  BigInteger b1;
//  BigInteger b2;
//  std::cin >> b1 >> b2;
//
//  std::cout << "+: " << (b1 + b2) << '\n';
//  std::cout << "*: " << (b1 * b2) << '\n';
//  std::cout << "/: " << (b1 / b2) << '\n';
//  std::cout << "%: " << (b1 % b2) << '\n';
//  Rational r1(b1, b2);
//
//  std::cout << r1.asDecimal(20) << '\n';
//  std::cout << r1.toString() << '\n';
//
//  BigInteger b3;
//  BigInteger b4;
//  std::cin >> b3 >> b4;
//
//  Rational r2(b3, b4);
//
//  std::cout << r2.asDecimal(20) << '\n';
//  std::cout << r2.toString() << '\n';
//
//  std::cout << "+: " << (r1 + r2) << '\n';
//  std::cout << "-: " << (r1 - r2) << '\n';
//  std::cout << "*: " << (r1 * r2) << '\n';
//  std::cout << "/: " << (r1 / r2) << '\n';
  /*
  std::srand(std::time(nullptr));

  for (int i = 0; i < 10000; ++i) {
    int size1 = std::rand() % 50 + 1;
    std::vector<int> buffer1{};

    for (int j = 0; j < size1; ++j)
//      buffer1.push_back(std::rand() % BigInteger::RADIX);
      buffer1.push_back(std::rand() % 2);

    int size2 = std::rand() % 50 + 1;
    std::vector<int> buffer2{};

    for (int j = 0; j < size2; ++j)
//      buffer2.push_back(std::rand() % BigInteger::RADIX);
      buffer2.push_back(std::rand() % 2);

    BigInteger b1(buffer1, std::rand() % 2);
    BigInteger b2(buffer2, std::rand() % 2);
    BigInteger b3 = b1 * b2;
    BigInteger b4 = b1 + b2;
    BigInteger b5 = b1 - b2;
    BigInteger b6 = b2 ? b1.abs() / b2.abs() : 0;
    BigInteger b7 = b2 ? b1.abs() % b2.abs() : 0;

    std::cout << "--------" << i << "--------\n";
    std::cout << "1: " << b1 << '\n';
    std::cout << "2: " << b2 << '\n';
    std::cout << "*: " << b3 << '\n';
    std::cout << "+: " << b4 << '\n';
    std::cout << "-: " << b5 << '\n';
    std::cout << "/: " << b6 << '\n';
    std::cout << "%: " << b7 << '\n';
  }

  return 0;
  */
}
