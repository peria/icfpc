#include "expression.h"

#include <gtest/gtest.h>
#include <string>

namespace {
Expression g_zero(ZERO, 0, 1 << 1, 0);
Expression g_one(ONE, 1, 1 << 1, 0);
Expression g_x(X, 0, 1 << 1, X);
Expression g_y(Y, 0, 1 << 1, Y);
Expression g_z(Z, 0, 1 << 1, Z);

Expression* kZero = &g_zero;
Expression* kOne = &g_one;
Expression* kX = &g_x;
Expression* kY = &g_y;
Expression* kZ = &g_z;
}  // namespace

class ExpressionTest : public testing::Test {};

TEST_F(ExpressionTest, EvalOperation0) {
  struct TestCase {
    Operation operation;
    uint64 result;
    uint8 bit_values;
    uint64 x, y, z;
    uint64 expect;
  } test_cases[] = {
    {ZERO, 0, 0,   0,   0,   0,   0},
    {ONE,  1, 0,   0,   0,   0,   1},
    {X,    0, X, 100,   0,   0, 100},
    {Y,    0, Y,   0, 200,   0, 200},
    {Z,    0, Z,   0,   0, 500, 500},
    {ZERO, 0, 0,   3,   1,   4,   0},
    {ONE,  1, 1,   1,   5,   9,   1},
    {X,    0, X,   2,   6,   5,   2},
    {Y,    0, Y,   3,   5,   8,   5},
    {Z,    0, Z,   9,   7,   9,   9},
  };
  for (const TestCase& test_case : test_cases) {
    Expression expression(test_case.operation, test_case.result,
                          1 << 1, test_case.bit_values);
    EXPECT_EQ(test_case.expect,
              expression.Eval(test_case.x, test_case.y, test_case.z));
  }
}

TEST_F(ExpressionTest, EvalOperation1) {
  struct TestCase {
    Operation operation;
    uint8 bit_values;
    Expression* operand1;
    uint64 x;
    uint64 expect;
  } test_cases[] = {
    // Set X in bit_values to force computing.
    {NOT,   X, kZero, 0, ~0ULL},
    {NOT,   X, kOne, 0, ~1ULL},
    {SHL1,  X, kOne, 0, 2},
    {SHR1,  X, kOne, 0, 0},  // 1 >> 4 == 0
    {SHR4,  X, kOne, 0, 0},  // 1 >> 16 == 0
    {SHR16, X, kOne, 0, 0},
    {SHR1,  X, kX,   0xffffULL, 0x7fffULL},
    {SHR4,  X, kX,   0xffULL << 5, 0xff << 1},
    {SHR16, X, kX,   0xffULL << 20, 0xffULL << 4},
    // Do not compute and return 7 in |result| as imm, because it is const.
    {NOT, 0, kOne, 0, 7},
  };
  for (const TestCase& test_case : test_cases) {
    Expression expression(test_case.operation, 7, 1 << 2,
                          test_case.bit_values, test_case.operand1);
    EXPECT_EQ(test_case.expect, expression.Eval(test_case.x, 0, 0));
  }
}

TEST_F(ExpressionTest, EvalOperation2) {
  struct TestCase {
    Operation operation;
    uint8 bit_values;
    uint64 x;
    uint64 expect;
  } test_cases[] = {
    {AND, X, 0xff, 1},
    {OR, X, 0x32, 0x33},
    {OR, X, 0x33, 0x33},
    {XOR, X, 0x32, 0x33},
    {XOR, X, 0x33, 0x32},
    {PLUS, X, 0xff, 0x100},
    {AND, 0, 0xff, 7},
  };
  for (const TestCase& test_case : test_cases) {
    Expression expression(test_case.operation, 7, 1 << 3,
                          test_case.bit_values, kOne, kX);
    EXPECT_EQ(test_case.expect, expression.Eval(test_case.x, 0, 0));
  }
}

TEST_F(ExpressionTest, EvalOperationIf0) {
  struct TestCase {
    Expression* operand1;
    uint8 bit_values;
    uint64 x;
    uint64 expect;
  } test_cases[] = {
    {kOne, X, 0, 0},
    {kZero, X, 0, 1},
    {kX, X, 0xff, 0},
    {kX, X, 0, 1},
    {kX, 0, 0xff, 7},
  };
  for (const TestCase& test_case : test_cases) {
    Expression expression(IF0, 7, 1 << 4, test_case.bit_values,
                          test_case.operand1, kOne, kZero);
    EXPECT_EQ(test_case.expect, expression.Eval(test_case.x, 0, 0));
  }
}

TEST_F(ExpressionTest, EvalOperationFold) {
  Expression plus(PLUS, 0, 1 << 3, VALUES, kY, kZ);
  Expression nonzero(OR, 0, 1 << 3, X, kX, kOne);

  struct TestCase {
    Expression* operand1;
    Expression* operand2;
    Expression* operand3;
    uint8 bit_values;
    uint64 x;
    uint64 expect;
  } test_cases[] = {
    {kOne, kZero, kZero, X, 0, 0},
    {kOne, kZero, kOne, X, 0, 1},
    {kX, kZero, &plus, X, 0x12345678, 0x114},
    {kX, kX, &plus, X, 0x12345678, 0x1234578c},
  };
  for (const TestCase& test_case : test_cases) {
    Expression expression(FOLD, 7, 1 << 4, test_case.bit_values,
                          test_case.operand1, test_case.operand2,
                          test_case.operand3);
    EXPECT_EQ(test_case.expect, expression.Eval(test_case.x, 0, 0));
  }
}

TEST_F(ExpressionTest, ToString) {
  // Operation0
  {
    EXPECT_EQ("0", kZero->ToString());
    EXPECT_EQ("1", kOne->ToString());
    EXPECT_EQ("x", kX->ToString());
    EXPECT_EQ("y", kY->ToString());
    EXPECT_EQ("z", kZ->ToString());
  }
  // Operation1
  {
    Expression ex_not(NOT, 0, 0, X, kX);
    Expression ex_shl1(SHL1, 0, 0, X, kX);
    Expression ex_shr1(SHR1, 0, 0, X, kX);
    Expression ex_shr4(SHR4, 0, 0, X, kX);
    Expression ex_shr16(SHR16, 0, 0, X, kX);
    EXPECT_EQ("(not x)", ex_not.ToString());
    EXPECT_EQ("(shl1 x)", ex_shl1.ToString());
    EXPECT_EQ("(shr1 x)", ex_shr1.ToString());
    EXPECT_EQ("(shr4 x)", ex_shr4.ToString());
    EXPECT_EQ("(shr16 x)", ex_shr16.ToString());
  }
  // Operation2
  {
    Expression ex_and(AND, 0, 0, X, kX, kOne);
    Expression ex_or(OR, 0, 0, X, kX, kOne);
    Expression ex_xor(XOR, 0, 0, X, kX, kOne);
    Expression ex_plus(PLUS, 0, 0, X, kX, kOne);
    EXPECT_EQ("(and x 1)", ex_and.ToString());
    EXPECT_EQ("(or x 1)", ex_or.ToString());
    EXPECT_EQ("(xor x 1)", ex_xor.ToString());
    EXPECT_EQ("(plus x 1)", ex_plus.ToString());
  }
  // others
  {
    Expression ex_plus(PLUS, 0, 0, VALUES, kY, kZ);
    Expression ex_if0(IF0, 0, 0, VALUES, kX, kZero, kOne);
    Expression ex_fold(FOLD, 0, 0, VALUES, kX, kZero, &ex_plus);
    EXPECT_EQ("(if0 x 0 1)", ex_if0.ToString());
    EXPECT_EQ("(fold x 0 (lambda (y z) (plus y z)))", ex_fold.ToString());
  }
}

TEST_F(ExpressionTest, DISABLED_IsNonZero) {
  // TODO: Implement it
}

TEST_F(ExpressionTest, DISABLED_IsValid) {
  // TODO: Implement it
}
