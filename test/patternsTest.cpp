#include <functional>
#include <optional>

#include <gtest/gtest.h>

#include "../include/fp/patterns.hpp"

namespace fp::test {

  TEST(Patterns, TestInts) {

    auto f = [] (int n) -> std::string {
      return fp::match<int, std::string>(n)
        >= 0 > "zero"
        >= 1 > "one"
        >= 2 > "two"
        |      "Error";
    };

    ASSERT_EQ("zero", f(0));
    ASSERT_EQ("one", f(1));
    ASSERT_EQ("two", f(2));
    ASSERT_EQ("Error", f(9));
  }

  TEST(Patterns, TestExcetions) {

    auto f = [] (int n) -> std::string {
      return fp::match<int, std::string>(n)
        >= 0 > "zero"
        >= 1 > "one"
        | [] (int n) -> std::string { throw std::runtime_error("Invalid value provided"); };
    };

    ASSERT_EQ("zero", f(0));
    ASSERT_EQ("one", f(1));
    ASSERT_THROW(f(2), std::runtime_error);
  }

  TEST(Patterns, TestOptionals) {

    auto f = [] (int n) -> std::optional<std::string> {
      return fp::match<int, std::optional<std::string>>(n)
        >= 0 > std::optional<std::string>{ "zero" }
        >= 1 > std::optional<std::string>{ "one" }
        | std::nullopt;
    };

    ASSERT_EQ("zero", f(0).value());
    ASSERT_EQ("one", f(1).value());
    ASSERT_EQ(std::nullopt, f(5));
  }

}