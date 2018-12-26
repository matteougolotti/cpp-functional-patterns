#include "../include/fp/collections.hpp";
#include "../include/fp/patterns.hpp";

#include <cmath>
#include <optional>

int factorial(int n) {
  return fp::match<int, int>(n)
    >= 0 > 1
    |      [] (int n) { return n * factorial(n-1); };
}

int fib(int n) {
  return fp::match<int, int>(n)
    >= 0 > 1
    >= 1 > 1
    |      [] (int n) { return fib (n - 1) + fib (n - 2); };
}

enum Number {
  ONE,
  TWO,
  THREE
};

std::optional<std::string> toString(Number n) {
  return fp::match<Number, std::optional<std::string>>(n)
    >= ONE   > std::optional<std::string>{"One"}
    >= TWO   > std::optional<std::string>{"Two"}
    >= THREE > std::optional<std::string>{"Three"}
    |          std::nullopt;
}