#include "problem.h"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "base.h"
#include "expression.h"

namespace {
const uint64 kUndef = 1;
}  // namespace

Problem::Problem(const int size, const uint32 bit_operations)
    : bit_max_size_(1ULL << (size - 1)),
      bit_sizes_((1ULL << size) - 1),
      bit_operations_(bit_operations) {
  // Output program have "lambda", whose size is 1, outside of this Program
  // class, so we expect internal size is in range [1, size).

  // Generate constant and values.
  Generate(ZERO, 0, 1 << 1, 0);      // '0'
  Generate(ONE,  1, 1 << 1, 0);      // '1'
  Generate(X, kUndef, 1 << 1, X);    // 'X'
  // Register y and z, only if fold operation is used.
  if (bit_operations & (FOLD | TFOLD)) {
    Generate(Y, kUndef, 1 << 1, Y);  // 'Y'
    Generate(Z, kUndef, 1 << 1, Z);  // 'Z'
  }
}

Problem::~Problem() {
  for (Expression* e : young_expressions_)
    old_expressions_.insert(e);
  for (Expression* e : new_expressions_)
    old_expressions_.insert(e);

  for (Expression* e : old_expressions_)
    delete(e);
}

void Problem::Generate(const Operation operation,
                       const uint64 result,
                       const uint64 bit_costs,
                       const uint8 bit_values,
                       Expression* expression1,
                       Expression* expression2,
                       Expression* expression3) {
  if ((operation & OP2) && operation == expression1->operation() && 
      operation == expression2->operation())
    return;

  // If |e| has values, register a new operation.
  if (!Expression::IsConstant(bit_values)) {
    new_expressions_.insert(
        new Expression(operation, result, bit_costs, bit_values,
                       expression1, expression2, expression3));
    return;
  }

  if (constants_.find(result) == constants_.end()) {
    Expression* e = new Expression(operation, result, bit_costs, bit_values,
                                   expression1, expression2, expression3);
    constants_[result] = e;
    new_expressions_.insert(e);
  }
}

void Problem::GenerateIf0(const uint64 bit_costs,
                          const uint8 bit_values,
                          Expression* expression1,
                          Expression* expression2,
                          Expression* expression3) {
  Expression3 idx(expression1, expression2, expression3);
  if (if0_set_.find(idx) != if0_set_.end())
    return;
  if0_set_.insert(idx);

  // (if0 0 y z) => y, (if0 NONZERO y z) => z
  if (expression1->IsZero() || expression1->IsNonZero())
    return;
  // (if0 x y y) => y
  if (expression2 == expression3)
    return;
  // (if0 x x 0) => 0
  if (expression1 == expression2 && expression3->IsZero())
    return;
  // (if0 x 0 1) => (and x 1) if "and" is available
  if ((bit_operations_ & AND) && expression2->IsZero() &&
      expression3->IsConstant() && expression3->result() == 1)
    return;
  
  uint64 result = expression1->result() ?
      expression3->result() : expression2->result();
  Generate(IF0, result, bit_costs, bit_values,
           expression1, expression2, expression3);
}

void Problem::GenerateFold(const uint64 bit_costs,
                           const uint8 bit_values,
                           Expression* expression1,
                           Expression* expression2,
                           Expression* expression3) {
  // Do not use Y nor Z outside fold operation.
  uint8 out_values = expression1->GetValues() | expression2->GetValues();
  if (out_values & (Y | Z))
    return;

  Expression3 idx(expression1, expression2, expression3);
  if (fold_set_.find(idx) != fold_set_.end())
    return;
  fold_set_.insert(idx);

  // TODO: Compute |result| when no X appear in operands.
  Generate(FOLD, kUndef, bit_costs, bit_values | FOLD,
           expression1, expression2, expression3);
}

std::string Problem::GetNextProgram(const std::vector<uint64>& inputs,
                                    const std::vector<uint64>& expects) {
  while (!new_expressions_.empty()) {
    young_expressions_.clear();
    while (!new_expressions_.empty()) {
      auto iter = new_expressions_.begin();
      Expression* program = *iter;
      new_expressions_.erase(iter);
    
      young_expressions_.insert(program);
      old_expressions_.insert(program);
      if (program->IsValid(inputs, expects))
        return program->ToString();
    }

    if (bit_operations_ & OP1)
      CombOperations1();
    if (bit_operations_ & OP2)
      CombOperations2();
    if (bit_operations_ & IF0)
      CombIf0();
    if (bit_operations_ & (FOLD | TFOLD))
      CombFold();

    std::cerr << "Expressions: " << old_expressions_.size() << " - "
              << young_expressions_.size() << " - "
              << new_expressions_.size() << "\n";
    for (Expression* prg : new_expressions_)
      std::cout << prg->ToString() << "\n";
  }

  return "";
}

uint64 Problem::MergeCosts(uint64 a, uint64 b) const {
  uint64 ret = 0;
  for (; a; a &= a - 1)
    ret |= b * (a & (-a));
  return ret & bit_sizes_;
}

int64 Problem::CombOperations1() {
  for (Expression* ex : young_expressions_) {
    const uint64  bit_costs = (ex->GetCosts() << 1) & bit_sizes_;
    if (!bit_costs)
      continue;

    uint32 values = ex->GetValues();
    uint64 result = ex->result();
    if (bit_operations_ & NOT) {
      bool add_not = true;
      if (ex->operation() == NOT)
        add_not = false;
      if (add_not)
        Generate(NOT, ~result, bit_costs, values, ex);
    }

    if (bit_operations_ & SHL1) {
      Generate(SHL1, result << 1, bit_costs, values, ex);
    }

    if (bit_operations_ & SHR1) {
      Generate(SHR1, result >> 1, bit_costs, values, ex);
    }

    if (bit_operations_ & SHR4) {
      Generate(SHR4, result >> 4, bit_costs, values, ex);
    }

    if (bit_operations_ & SHR16) {
      Generate(SHR16, result >> 16, bit_costs, values, ex);
    }
  }
  return 0;
}

int64 Problem::CombOperations2() {
  for (Expression* e1 : young_expressions_) {
    const uint64  bit_cost1 = e1->GetCosts();
    if (e1->IsZero())
      continue;
    for (Expression* e2 : old_expressions_) {
      const uint64  bit_costs =
          (MergeCosts(bit_cost1, e2->GetCosts()) << 1) & bit_sizes_;
      if (!bit_costs)
        continue;
      if (e2->IsZero())
        continue;
      if (young_expressions_.find(e2) != young_expressions_.end() && e1 < e2)
        continue;

      uint32 values = e1->GetValues() | e2->GetValues();
      uint64 r1 = e1->result();
      uint64 r2 = e2->result();
      if (bit_operations_ & AND) {
        bool add_and = true;
        if (e1 == e2)
          add_and = false;
        if (add_and)
          Generate(AND, r1 & r2, bit_costs, values, e1, e2);
      }

      if (bit_operations_ & OR) {
        bool add_or = true;
        if (e1 == e2)
          add_or = false;
        if (add_or)
          Generate(OR, r1 | r2, bit_costs, values, e1, e2);
      }

      if (bit_operations_ & XOR) {
        bool add_xor = true;
        if (e1 == e2)  // (xor e e) => 0
          add_xor = false;
        if (add_xor)
          Generate(XOR, r1 ^ r2, bit_costs, values, e1, e2);
      }

      if (bit_operations_ & PLUS) {
        bool add_plus = true;
        if (e1 == e2 && (bit_operations_ & SHL1))
          add_plus = false;
        if (add_plus)
          Generate(PLUS, r1 + r2, bit_costs, values, e1, e2);
      }
    }
  }

  return 0;
}

int64 Problem::CombIf0() {
  for (Expression* e1 : young_expressions_) {
    const uint64  cost1 = e1->GetCosts();
    for (Expression* e2 : old_expressions_) {
      const uint64  cost12 = MergeCosts(cost1, e2->GetCosts());
      for (Expression* e3 : old_expressions_) {
        const uint64  costs =
            (MergeCosts(cost12, e3->GetCosts()) << 1) & bit_sizes_;
        if (!costs)
          continue;

        uint32 values = e1->GetValues() | e2->GetValues() | e3->GetValues();
        GenerateIf0(costs, values, e1, e2, e3);
        GenerateIf0(costs, values, e1, e3, e2);
        GenerateIf0(costs, values, e2, e1, e3);
        GenerateIf0(costs, values, e2, e3, e1);
      }
    }
  }

  return 0;
}

int64 Problem::CombFold() {
  for (Expression* e1 : young_expressions_) {
    const uint64  cost1 = e1->GetCosts();
    for (Expression* e2 : old_expressions_) {
      const uint64  cost12 = MergeCosts(cost1, e2->GetCosts());
      for (Expression* e3 : old_expressions_) {
        const uint64  costs =
            (MergeCosts(cost12, e3->GetCosts()) << 2) & bit_sizes_;
        if (!costs)
          continue;

        uint32 values = e1->GetValues() | e2->GetValues() | e3->GetValues();
        GenerateFold(costs, values, e1, e2, e3);
        GenerateFold(costs, values, e1, e3, e2);
        GenerateFold(costs, values, e2, e1, e3);
        GenerateFold(costs, values, e2, e3, e1);
        GenerateFold(costs, values, e3, e1, e2);
        GenerateFold(costs, values, e3, e2, e1);
      }
    }
  }

  return 0;
}
