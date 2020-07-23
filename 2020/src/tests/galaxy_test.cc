#include "galaxy.h"

#include <gtest/gtest.h>
#include <glog/logging.h>

namespace {

int64_t valueOf(Pointer<Expr> expr) {
  EXPECT_TRUE(expr->isNumber());
  return dynamic_pointer_cast<Number>(expr)->value;
}

}

TEST(GalaxyTest, Expr) {
  Galaxy galaxy;
  {
    auto expr = galaxy.process("foo");
    EXPECT_TRUE(expr->isAtom());
    EXPECT_EQ(expr->ToString(), "foo");
  }
  {
    auto expr = galaxy.process("ap");
    EXPECT_TRUE(expr->isAp());
    EXPECT_EQ(expr->ToString(), "ap");
  }
  {
    auto expr = galaxy.process("128");
    EXPECT_TRUE(expr->isNumber());
    EXPECT_EQ(expr->ToString(), "128");
  }

  {
    auto expr = galaxy.process("ap ap add 100");
    EXPECT_EQ(expr->ToString(), "ap ap add 100");
  }
}

TEST(GalaxyTest, BuildTree) {
  Galaxy galaxy;

  auto expr = galaxy.process("ap inc 1");
  EXPECT_EQ(expr->ToString(), "ap inc 1");
}

TEST(GalaxyTest, DISABLED_Compile) {
  Galaxy galaxy;

  auto expr = galaxy.process("ap add 1");
  EXPECT_EQ(expr->ToString(), "inc");
}

TEST(GalaxyTest, GalaxyPad) {
  Galaxy galaxy("../data/galaxy.txt");
}

TEST(GalaxyTest, BasicOperations) {
  struct TestData {
    std::string input;
    int64_t expect;
  } test_cases[] = {
    {"ap ap add 1 2", 3},
    {"ap ap add 2 1", 3},
    {"ap ap add 0 1", 1},
    {"ap ap add 2 3", 5},
    {"ap ap add 3 5", 8},
    {"ap ap mul 4 2", 8},
    {"ap ap mul 3 4", 12},
    {"ap ap mul 3 -2", -6},
    {"ap ap div 4 2", 2},
    {"ap ap div 4 3", 1},
    {"ap ap div 4 4", 1},
    {"ap ap div 4 5", 0},
    {"ap ap div 5 2", 2},
    {"ap ap div 6 -2", -3},
    {"ap ap div 5 -3", -1},
    {"ap ap div -5 3", -1},
    {"ap ap div -5 -3", 1},
  };
  Galaxy galaxy;
  for (auto test_case : test_cases) {
    auto expr = galaxy.process(test_case.input);
    EXPECT_EQ(valueOf(expr), test_case.expect)
      << "For input : " << test_case.input;
  }
}
