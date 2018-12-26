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

  TEST(Collections, Map) {
    fp::collection<int> c{ 1, 2, 3 };

    const auto negatives = c.map([] (int n) -> int { return -n; });

    ASSERT_EQ(c.size(), negatives.size());
    ASSERT_EQ(-c[0], negatives[0]);
    ASSERT_EQ(-c[1], negatives[1]);
    ASSERT_EQ(-c[2], negatives[2]);
  }

}