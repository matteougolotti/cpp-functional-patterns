#include <vector>

#include <gtest/gtest.h>
#include "../include/fp/collections.hpp"

namespace fp::test {

  TEST(Collections, ConstructorEmptyCollection) {
    fp::collection<int> c{};

    ASSERT_EQ(0, c.size());
  }

  TEST(Collections, ConstructorManyElements) {
    fp::collection<int> c{ 1, 2, 3 };

    ASSERT_EQ(3, c.size());
    ASSERT_EQ(1, c[0]);
    ASSERT_EQ(2, c[1]);
    ASSERT_EQ(3, c[2]);
  }

  TEST(Collections, ConstructFromVector) {
    std::vector<int> v{ 1, 2, 3 };
    fp::collection<int> c{ v };

    ASSERT_EQ(v.size(), c.size());
    ASSERT_EQ(v[0], c[0]);
    ASSERT_EQ(v[1], c[1]);
    ASSERT_EQ(v[2], c[2]);
  }

  TEST(Collections, ConstructFromList) {
    std::list<int> l{ 1, 2, 3 };
    fp::collection<int> c{ l };

    ASSERT_EQ(l.size(), c.size());
    ASSERT_EQ(1, c[0]);
    ASSERT_EQ(2, c[1]);
    ASSERT_EQ(3, c[2]);
  }

  TEST(Collections, ConstructFromIterator) {
    std::vector<int> v{ 1, 2, 3 };
    fp::collection<int> c{ v.begin(), v.end() };

    ASSERT_EQ(v.size(), c.size());
    ASSERT_EQ(v[0], c[0]);
    ASSERT_EQ(v[1], c[1]);
    ASSERT_EQ(v[2], c[2]);
  }

  TEST(Collections, Head) {
    fp::collection<char> c{ 'a', 'b', 'c' };

    ASSERT_EQ(c[0], c.head());
  }

  TEST(Collections, Tail) {
    fp::collection<char> c{ 'a', 'b', 'c' };
    const auto t = c.tail();

    ASSERT_EQ(c.size() - 1, t.size());
    ASSERT_EQ(c[1], t[0]);
    ASSERT_EQ(c[2], t[1]);
  }

  TEST(Collections, Each) {
    fp::collection<int> c{ 1, 2, 3 };
    std::vector<int> v;
    c.each([&] (int n) -> void { v.push_back(n); });

    ASSERT_EQ(v[0], c[0]);
    ASSERT_EQ(v[1], c[1]);
    ASSERT_EQ(v[2], c[2]);
  }

  TEST(Collections, Filter) {
    fp::collection<int> c{ 1, 2, 3 };
    
    const auto evens = c.filter([] (int n) -> int { return n % 2 == 0; });

    ASSERT_EQ(1, evens.size());
    ASSERT_EQ(2, evens[0]);
  }

  TEST(Collections, Slice) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto s = c.slice(1, 3);
    
    ASSERT_EQ(2, s.size());
    ASSERT_EQ(2, s[0]);
    ASSERT_EQ(3, s[1]);
  }

  TEST(Collections, Count) {
    fp::collection<int> c{ 1, 2, 3 };

    int even = c.count([] (int n) -> bool { return n % 2 == 0; });

    ASSERT_EQ(1, even);
  }

  TEST(Collections, Sort) {
    fp::collection<int> c{ 1, 2, 3 };
    
    const auto desc = c.sort([] (int a, int b) -> bool { return a > b; });

    ASSERT_EQ(c.size(), desc.size());
    ASSERT_EQ(c[0], desc[2]);
    ASSERT_EQ(c[1], desc[1]);
    ASSERT_EQ(c[2], desc[0]);
  }

  TEST(Collections, Map) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto negatives = c.map([] (int n) -> int { return -n; });

    ASSERT_EQ(c.size(), negatives.size());
    ASSERT_EQ(-c[0], negatives[0]);
    ASSERT_EQ(-c[1], negatives[1]);
    ASSERT_EQ(-c[2], negatives[2]);
  }

  TEST(Collections, Pmap) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto plusone = c.pmap([] (int n) -> int { return n + 1; });

    ASSERT_EQ(c.size(), plusone.size());
    ASSERT_EQ(c[0] + 1, plusone[0]);
    ASSERT_EQ(c[1] + 1, plusone[1]);
    ASSERT_EQ(c[2] + 1, plusone[2]);
  }

  TEST(Collections, Reduce) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto sum = c.reduce(std::plus<int>());

    ASSERT_EQ(6, sum);
  }

  TEST(Collections, Rightreduce) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto sum = c.rightreduce(std::plus<int>());

    ASSERT_EQ(6, sum);
  }

  TEST(Collections, Fold) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto res = c.fold(std::plus<int>(), 4);

    ASSERT_EQ(10, res);
  }

  TEST(Collections, Foldr) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto res = c.foldr(std::plus<int>(), 4);

    ASSERT_EQ(10, res);
  }

  TEST(Collections, Concat) {
    fp::collection<int> a{ 1 };
    fp::collection<int> b{ 2, 3 };

    const auto c = a.concat(b);

    ASSERT_EQ(a.size() + b.size(), c.size());
    ASSERT_EQ(a[0], c[0]);
    ASSERT_EQ(b[0], c[1]);
    ASSERT_EQ(b[1], c[2]);
  }

}