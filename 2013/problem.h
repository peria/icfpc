#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "base.h"
#include "expression.h"

class Problem {
 public:
  typedef std::tuple<Expression*, Expression*, Expression*> Expression3;
  typedef std::set<Expression*> Generation;
  typedef Generation::iterator Iterator;

  Problem(const int size, const uint32 bit_operators);
  ~Problem();

  // Returns a program which returns expected values for all input values.
  // NOTE: This method is the main routine, so it may take very long time.
  std::string GetNextProgram(const std::vector<uint64>& inputs,
                             const std::vector<uint64>& expects);

  // Returns costs of operation with two costs operation arguments.
  uint64 MergeCosts(uint64 a, uint64 b) const;

 protected:
  // If it is needed, generates an Expression object and registers it to
  // be used in the next loop.
  void Generate(const Operation operation,
                const uint64 result,
                const uint64 bit_costs,
                const uint8 bit_value,
                Expression* op1 = NULL,
                Expression* op2 = NULL,
                Expression* op3 = NULL);

  // Wrapper of Generate() method especially for if0 operation.
  void GenerateIf0(const uint64 bit_costs,
                   const uint8 bit_values,
                   Expression* expression1,
                   Expression* expression2,
                   Expression* expression3);

  // Wrapper of Generate() method especially for fold operation.
  void GenerateFold(const uint64 bit_costs,
                    const uint8 bit_values,
                    Expression* expression1,
                    Expression* expression2,
                    Expression* expression3);

  // Generate expression trees, using expressions in |children|.
  int64 CombOperations1();
  int64 CombOperations2();
  int64 CombIf0();
  int64 CombFold();

  // Stack area
  // g_pool, g_child and g_baby owns the instance.
  // A pointer must be unique in those 3 groups.
  Generation old_expressions_;
  Generation young_expressions_;
  Generation new_expressions_;

  // Quick reference for constant values.
  std::map<uint64, Expression*> constants_; // [value]->expression
  // Avoid duplicated if0 and fold operations.
  std::set<Expression3> if0_set_;
  std::set<Expression3> fold_set_;

  // Bit of size.
  const uint64 bit_max_size_;
  // Bitmask of allowed sizes in searching.
  const uint64 bit_sizes_;
  // Usable operations.
  const uint64 bit_operations_;
};

#endif  // PROBLEM_H_
