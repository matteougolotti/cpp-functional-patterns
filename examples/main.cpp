#include "../collections.hpp";
#include "../patterns.hpp";

#include <cmath>

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
