C++ functional patterns
===

This library aims at bringing some of the elegent syntax of functional programming languages such as Haskell, OCaml and Scala to C++.

To use it just copy the header files and include them in your project.

fp::collection
---

The library provides a generic collection class, which exposes several higher order functions common in functional programming languages.


```
// Empty collection
fp::collection<int> c{};

// Vector initialization
std::vector<int> v { 1, 2, 3 };
fp::collection<int> cv { v };

// List initialization
std::list<int> l { 1, 2, 3 };
fp::colleciton<int> cl { l };

// Iterator initialization
std::set<int> s;
fp::collection ci { s.begin(), s.end() };

// Brace enclosed initializer list
fp::collection c { 1, 2, 3, 4 };

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
// Pattern mathing on primitive types
int number { 3 };
std::string label = fp::match<int, std::string>(number)
  >= 0 > "Zero"
  >= 1 > "One"
  >= 2 > "Two"
  >= 3 > "Three"
  >= 4 > "Four"
  |      "Invalid number";

// Pattern matching on enums with lazy evaluated lambdas
enum Number {
    ONE,
    TWO,
    THREE
}

auto number = Number::TWO;
std::string label = fp::match<Number, std::string>(number)
  >= ONE   > "One"
  >= TWO   > [] (Number n) -> std::string { return "Two"; }
  >= THREE > "Three"
  |          [] (Number n) -> std::string { throw InvalidValueException(); };

// Fibonacci numbers
int fib(int n) {
  return fp::match<int, int>(n)
    >= 0 > 1
    >= 1 > 1
    |      [] (int n) { return fib (n - 1) + fib (n - 2); };
}
 
```

Limitations
---
This library is experimental, and provided as a prototype. Feel free to report any issue or suggestion for improvement.