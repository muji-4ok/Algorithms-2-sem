//
// Created by egork on 20.03.2020.
//

#include "BigInteger.h"
#include <iostream>

int main() {
  char operation;
  std::cin >> operation;
  BigInteger b1;
  BigInteger b2;
//  int i1;
//  int i2;
//  std::cin >> i1 >> i2;
  std::cin >> b1 >> b2;
//  BigInteger b1(i1);
//  BigInteger b2(i2);

  switch (operation) {
    case '+':
//      std::cout << (i1 + i2) << '\n';
      std::cout << (b1 + b2) << '\n';
      break;
    case '-':
//      std::cout << (i1 - i2) << '\n';
      std::cout << (b1 - b2) << '\n';
      break;
    case '*':
//      std::cout << (i1 * i2) << '\n';
      std::cout << (b1 * b2) << '\n';
      break;
    case '/':
//      std::cout << (i1 / i2) << '\n';
      std::cout << (b1 / b2) << '\n';
      break;
    case '%':
//      std::cout << (i1 % i2) << '\n';
      std::cout << (b1 % b2) << '\n';
      break;
  }
}