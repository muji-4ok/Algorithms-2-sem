//
// Created by egork on 22.03.2020.
//
#include "Rational.h"
#include <iostream>

int main() {
  char operation;
  std::cin >> operation;
  BigInteger a1;
  BigInteger a2;
  std::cin >> a1 >> a2;
  BigInteger b1;
  BigInteger b2;
  std::cin >> b1 >> b2;
  Rational r1(a1, a2);
  Rational r2(b1, b2);

  switch (operation) {
    case '+':
      std::cout << (r1 + r2) << '\n';
      break;
    case '-':
      std::cout << (r1 - r2) << '\n';
      break;
    case '*':
      std::cout << (r1 * r2) << '\n';
      break;
    case '/':
      std::cout << (r1 / r2) << '\n';
      break;
    case '<':
      std::cout << (r1 < r2) << '\n';
      break;
    case '>':
      std::cout << (r1 > r2) << '\n';
      break;
    case 'l':
      std::cout << (r1 <= r2) << '\n';
      break;
    case 'g':
      std::cout << (r1 >= r2) << '\n';
      break;
    case '=':
      std::cout << (r1 == r2) << '\n';
      break;
    case '!':
      std::cout << (r1 != r2) << '\n';
      break;
    case 'm':
      std::cout << (-r1) << '\n';
      break;
  }
}
