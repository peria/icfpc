#include "problem.h"

#include <gtest/gtest.h>
#include <set>
#include <string>
#include <vector>

class ProblemForTest : public Problem {
 public:
  using Problem::Generate;
  using Problem::GenerateIf0;
  using Problem::GenerateFold;
  using Problem::CombOperations1;
  using Problem::CombOperations2;
  using Problem::CombIf0;
  using Problem::CombFold;
  using Problem::old_expressions_;
  using Problem::young_expressions_;
  using Problem::new_expressions_;

  ProblemForTest(const int size, const uint32 bit_operations)
      : Problem(size, bit_operations) {}
};

class ProblemTest : public testing::Test {};

TEST_F(ProblemTest, Constructor) {
  ProblemForTest problem1(3, 0);
  EXPECT_EQ(3, problem1.new_expressions_.size());
  int sum1 = 0;
  for (Expression* expression : problem1.new_expressions_) {
    int val = expression->Eval(3, 4, 5);
    EXPECT_TRUE(val == 0 || val == 1 || val == 3);
    EXPECT_EQ(1 << 1, expression->GetCosts());
    sum1 += val;
  }
  EXPECT_EQ(4, sum1);

  ProblemForTest problem2(3, FOLD);
  EXPECT_EQ(5, problem2.new_expressions_.size());
  int sum2 = 0;
  for (Expression* expression : problem2.new_expressions_) {
    int val = expression->Eval(3, 4, 5);
    EXPECT_TRUE(val == 0 || val == 1 || val == 3 || val == 4 || val == 5);
    EXPECT_EQ(1 << 1, expression->GetCosts());
    sum2 += val;
  }
  EXPECT_EQ(13, sum2);

  ProblemForTest problem3(3, TFOLD);
  EXPECT_EQ(5, problem3.new_expressions_.size());
  int sum3 = 0;
  for (Expression* expression : problem3.new_expressions_) {
    int val = expression->Eval(3, 4, 5);
    EXPECT_TRUE(val == 0 || val == 1 || val == 3 || val == 4 || val == 5);
    EXPECT_EQ(1 << 1, expression->GetCosts());
    sum3 += val;
  }
  EXPECT_EQ(13, sum3);
}

TEST_F(ProblemTest, GetNextProgram) {
  struct TestCase {
    int size;
    uint64 operations;
    std::string expect;
    int n;
    uint64 inputs[5];
    uint64 outputs[5];
  } test_cases[] = {
    {4, SHL1, "(shl1 1)", 2, {1, 0}, {2, 2}},
    {3, SHL1, "", 2, {1, 3}, {2, 4}},
  };

  for (TestCase t : test_cases) {
    ProblemForTest problem(t.size, t.operations);
    std::vector<uint64> inputs;
    std::vector<uint64> outputs;
    for (int i = 0; i < t.n; ++i) {
      inputs.push_back(t.inputs[i]);
      outputs.push_back(t.outputs[i]);
    }

    EXPECT_EQ(t.expect, problem.GetNextProgram(inputs, outputs));
  }
}

TEST_F(ProblemTest, MergeCosts) {
  struct TestCase {
    int size;
    uint64 a, b, expect;
  } test_cases[] = {
    // NOTE: Don't forget |size| includes (lambda (x) e).
    {30, 1 << 1, 1 << 1, 1 << 2},
    {30, 3 << 1, 1 << 1, 3 << 2},
    {30, 3 << 1, 3 << 1, 7 << 2},
    {30, 0xd << 1, 9 << 1, 0x6d << 2},
    {8, 0xd << 1, 9 << 1, 0x2d << 2},
  };
  for (TestCase t : test_cases) {
    ProblemForTest problem(t.size, 0);
    EXPECT_EQ(t.expect, problem.MergeCosts(t.a, t.b))
      << "MergeCosts(" << t.a << ", " << t.b << ") in size " << t.size;
  }
}

TEST_F(ProblemTest, Generate) {
  ProblemForTest problem(3, 0);
  Problem::Generation& expressions = problem.new_expressions_;

  EXPECT_EQ(3, expressions.size());
  // Add a valuable expression.
  problem.Generate(NOT, 7, 1 << 2, X, *expressions.begin());
  EXPECT_EQ(4, expressions.size());
  // Add a constant expression.
  problem.Generate(SHL1, 7, 1 << 2, 0, *expressions.begin());
  EXPECT_EQ(5, expressions.size());
  // Add the same constant expression, which should not be added.
  problem.Generate(SHL1, 7, 1 << 2, 0, *expressions.begin());
  EXPECT_EQ(5, expressions.size());
}

TEST_F(ProblemTest, GenerateIf0) {
  ProblemForTest problem(3, 0);
  Expression* expr_0 = NULL;
  Expression* expr_1 = NULL;
  Expression* expr_x = NULL;
  for (Expression* ex : problem.new_expressions_) {
    problem.young_expressions_.insert(ex);
    problem.old_expressions_.insert(ex);
    if (ex->IsZero())
      expr_0 = ex;
    else if (ex->IsConstant())
      expr_1 = ex;
    else
      expr_x = ex;
  }
  EXPECT_TRUE(expr_0);
  EXPECT_TRUE(expr_1);
  EXPECT_TRUE(expr_x);
  problem.new_expressions_.clear();

  EXPECT_EQ(0, problem.new_expressions_.size());
  // Add a valuable expression.
  problem.GenerateIf0(1 << 4, X, expr_x, expr_x, expr_1);
  EXPECT_EQ(1, problem.new_expressions_.size());
  // Add another valuable expression.
  problem.GenerateIf0(1 << 4, X, expr_x, expr_1, expr_0);
  EXPECT_EQ(2, problem.new_expressions_.size());
  // Add the same expression, which should not be added.
  problem.GenerateIf0(1 << 4, X, expr_x, expr_1, expr_0);
  EXPECT_EQ(2, problem.new_expressions_.size());
  // Add a constant expression, which should not be added.
  problem.GenerateIf0(1 << 4, 0, expr_1, expr_1, expr_1);
  EXPECT_EQ(2, problem.new_expressions_.size());
}

TEST_F(ProblemTest, GenerateFold) {
  ProblemForTest problem(3, FOLD);
  Expression* expr_0 = NULL;
  Expression* expr_1 = NULL;
  Expression* expr_x = NULL;
  Expression* expr_y = NULL;
  Expression* expr_z = NULL;
  for (Expression* ex : problem.new_expressions_) {
    problem.young_expressions_.insert(ex);
    problem.old_expressions_.insert(ex);
    if (ex->IsZero())
      expr_0 = ex;
    else if (ex->IsConstant())
      expr_1 = ex;
    else if (ex->GetValues() & X)
      expr_x = ex;
    else if (ex->GetValues() & Y)
      expr_y = ex;
    else if (ex->GetValues() & Z)
      expr_z = ex;
  }
  EXPECT_TRUE(expr_0);
  EXPECT_TRUE(expr_1);
  EXPECT_TRUE(expr_x);
  EXPECT_TRUE(expr_y);
  EXPECT_TRUE(expr_z);
  problem.new_expressions_.clear();

  EXPECT_EQ(0, problem.new_expressions_.size());
  // Add a valuable expression.
  problem.GenerateFold(1 << 4, X, expr_x, expr_x, expr_1);
  EXPECT_EQ(1, problem.new_expressions_.size());
  // Add another valuable expression.
  // TODO: This expression should be a constant, whick is not counted.
  problem.GenerateFold(1 << 4, X, expr_x, expr_1, expr_z);
  EXPECT_EQ(2, problem.new_expressions_.size());
  // Add the same expression, which should not be added.
  problem.GenerateFold(1 << 4, X, expr_x, expr_1, expr_z);
  EXPECT_EQ(2, problem.new_expressions_.size());
  // Add a constant expression, which should not be added.
  problem.GenerateFold(1 << 4, 0, expr_1, expr_1, expr_1);
  EXPECT_EQ(2, problem.new_expressions_.size());
}

TEST_F(ProblemTest, CombOperations1) {
  struct TestCase {
    int size;
    uint32 operations;
    int expect_size[3];
  } test_cases[] = {
    {10, SHL1, {2, 2, 2}},
    {10, SHL1 | NOT, {5, 7, 13}},
    {10, OP2 | IF0 | FOLD, {0, 0, 0}},
  };
  for (TestCase t : test_cases) {
    ProblemForTest problem(t.size, t.operations);
    for (int i = 0; i < 3; ++i) {
      // Migrate new expressions into young & old expressions.
      problem.young_expressions_.clear();
      for (Expression* ex : problem.new_expressions_) {
        problem.young_expressions_.insert(ex);
        problem.old_expressions_.insert(ex);
      }
      problem.new_expressions_.clear();

      problem.CombOperations1();
      EXPECT_EQ(t.expect_size[i], problem.new_expressions_.size())
        << "for i = " << i;
    }
  }
}

TEST_F(ProblemTest, CombOperations2) {
  struct TestCase {
    int size;
    uint32 operations;
    int expect_size;
  } test_cases[] = {
    // |expect_size| may change as prunes are added.
    {10, OR, 5},
    {10, AND | OR, 10},
    {10, OP1 | IF0 | FOLD, 0},
  };
  for (TestCase t : test_cases) {
    ProblemForTest problem(t.size, t.operations);
    // Migrate new expressions into young & old expressions.
    problem.young_expressions_.clear();
    for (Expression* ex : problem.new_expressions_) {
      problem.young_expressions_.insert(ex);
      problem.old_expressions_.insert(ex);
    }
    problem.new_expressions_.clear();

    problem.CombOperations2();
    EXPECT_EQ(t.expect_size, problem.new_expressions_.size());
  }
}

TEST_F(ProblemTest, CombIf0) {
  ProblemForTest problem(10, IF0);

  // Migrate new expressions into young & old expressions.
  problem.young_expressions_.clear();
  for (Expression* ex : problem.new_expressions_) {
    problem.young_expressions_.insert(ex);
    problem.old_expressions_.insert(ex);
  }
  problem.new_expressions_.clear();

  problem.CombIf0();
  // |expect_size| may change as prunes are added.
  // 19 = 3^3 - 2^3(only constants)
  EXPECT_EQ(19, problem.new_expressions_.size());
}

TEST_F(ProblemTest, CombFold) {
  struct TestCase {
    int size;
    uint32 operations;
    int expect_size;
  } test_cases[] = {
    // |expect_size| may change as prunes are added.
    // 37 = 3^2(no YZ outside FOLD) * 5 - 2^3(only constants)
    {10, FOLD, 37},
    {10, TFOLD, 37},
  };
  for (TestCase t : test_cases) {
    ProblemForTest problem(t.size, t.operations);
    // Migrate new expressions into young & old expressions.
    problem.young_expressions_.clear();
    for (Expression* ex : problem.new_expressions_) {
      problem.young_expressions_.insert(ex);
      problem.old_expressions_.insert(ex);
    }
    problem.new_expressions_.clear();

    problem.CombFold();
    EXPECT_EQ(t.expect_size, problem.new_expressions_.size());
  }
}
