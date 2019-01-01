[![cppstd](https://img.shields.io/badge/C++-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)

C++ functional patterns
===

This library aims at bringing some of the elegent syntax of functional programming languages such as Haskell, OCaml and Scala to C++.

To use it just copy the header files and include them in your project.

fp::collection
---

The library provides a generic collection class, which exposes several higher order functions common in functional programming languages.


```
fp::collection<int> c{};  // Empty collection

std::vector<int> v { 1, 2, 3 };
fp::collection<int> cv { v };  // Vector initialization

std::list<int> l { 1, 2, 3 };
fp::colleciton<int> cl { l };  // List initialization

std::set<int> s;
fp::collection ci { s.begin(), s.end() };  // Iterator initialization

fp::collection<int> c { 1, 2, 3, 4 };  // Brace enclosed initializer list

```

Higher order functions
---

The library implements several higher order functions to operate on collections

```
// Map
std::vector<int> positives { 1, 2, 3 };
auto negative = fp::collection<int> { positives }
               .map([] (int n) -> int { return -n; })
               .vector();

// Left reduce
std::vector<int> v { 1, 2, 3, 4 };
int sum = fp::collection<int>{v}
          .reducel(std::plus<int>());

// Filter
std::vector<int> numbers { 1, 2, 3, 4, 5 };
auto even = fp::collection<int> { numbers }
            .filter([] (int n) { return n % 2 == 0; })
            .vector();

// Pipelining
std::vector<int> numbers { 1, 2, 3, 4, 5 };
auto sortedEven = fp::collection<int> { numbers }
                  .filter([] (int n) { return n % 2 == 0; })
                  .sort([] (int first, int second) { return first < second; })
                  .vector();
```

Pattern matching
---

The library offers functional languages style pattern matching syntax based on values comparison.

```
// Pattern matching on primitive types
int number { 3 };
std::string label = fp::match<int, std::string>(number)
  >= 0 > "Zero"
  >= 1 > "One"
  >= 2 > "Two"
  >= 3 > "Three"
  >= 4 > "Four"
  |      "Invalid number";

// Pattern matching with optionals
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

// Fibonacci numbers
int fib(int n) {
  return fp::match<int, int>(n)
    >= 0 > 1
    >= 1 > 1
    |      [] (int n) { return fib (n - 1) + fib (n - 2); };
}
 
```

Run tests (requires Google Test)
---
```
cd test
g++ -std=c++17 -stdlib=libc++  test/collectionsTest.cpp patternsTest.cpp main.cpp  -lgtest -lpthread -o main
./main
```


Limitations
---
This library is experimental, and provided as a prototype. Feel free to report any issue or suggestion for improvement.
