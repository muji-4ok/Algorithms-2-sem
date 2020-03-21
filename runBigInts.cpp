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
  std::cin >> b1 >> b2;

  switch (operation) {
    case '+':
      std::cout << (b1 + b2) << '\n';
      break;
    case '-':
      std::cout << (b1 - b2) << '\n';
      break;
    case '*':
      std::cout << (b1 * b2) << '\n';
      break;
    case '/':
      std::cout << (b1 / b2) << '\n';
      break;
    case '%':
      std::cout << (b1 % b2) << '\n';
      break;
  }
}