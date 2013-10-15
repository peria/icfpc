#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <string>
#include <vector>

#include "base.h"

enum Operation {
  ZERO   = 1 << 0,
  ONE    = 1 << 1,
  X      = 1 << 2,
  Y      = 1 << 3,
  Z      = 1 << 4,
  VALUES = X | Y | Z,  // Operations without arguments
  OP0    = ZERO | ONE | VALUES,  // Operations without arguments
  NOT    = 1 << 5,
  SHL1   = 1 << 6,
  SHR1   = 1 << 7,
  SHR4   = 1 << 8,
  SHR16  = 1 << 9,
  OP1    = NOT | SHL1 | SHR1 | SHR4 | SHR16, // Operations with 1 argument
  AND    = 1 << 10,
  OR     = 1 << 11,
  XOR    = 1 << 12,
  PLUS   = 1 << 13,
  OP2    = AND | OR | XOR | PLUS,  // Operations with 2 arguments
  IF0    = 1 << 14,
  FOLD   = 1 << 15,
  TFOLD  = 1 << 16,
};


class Expression {
 public:
  Expression(const Operation operation,
             const uint64 result,
             const uint64 bit_costs,
             const uint8 bit_values,
             Expression* expression1 = NULL,
             Expression* expression2 = NULL,
             Expression* expression3 = NULL);

  // Evaluate with input values.
  uint64 Eval(uint64 x, uint64 y = 0, uint64 z = 0) const;

  // Constract a string to describe the structure of Expression.
  const std::string ToString() const;

  // Returns true if Expression returns non zero value for any arguments.
  bool IsNonZero();

  // Returns true if |this| returns expected values for all input values.
  bool IsValid(const std::vector<uint64>& inputs,
               const std::vector<uint64>& expects);


  // Inline methods --------------------------------

  // Update cost for it and its aliases.
  inline uint64 MergeCost(uint64 cost) {
    return bit_costs_ |= cost;
  }

  // Returns if |this| returns a costant value for any arguments.
  inline bool IsConstant() const {
    return IsConstant(bit_values_);
  }
  static bool IsConstant(const uint8 bit_values) {
    return (bit_values & VALUES) == 0;
  }

  // Returns true if Expression returns zero for any arguments.
  inline bool IsZero() {
    return IsConstant() && (result_ == 0);
  }

  // Accessors ----------------------------------
  inline uint64 result() { return result_; }
  inline uint32 operation() { return operation_; }

  inline uint64 GetCosts() { return bit_costs_; }
  inline void SetCosts(uint64 bit_costs) { bit_costs_ = bit_costs; }
  inline void UpdateCosts(uint64 bit_costs) { bit_costs_ |= bit_costs; }

  inline uint8 GetValues() { return bit_values_; }
  inline void SetValues(uint8 bit_values) { bit_values_ = bit_values; }
  inline void UpdateValues(uint8 bit_values) { bit_values_ |= bit_values; }


 private:
  // Figures which operation |this| express.
  // Must have 1 bit with Operator type.
  const uint32 operation_;
  // Pointers to the operands of operation.
  const Expression* expression1_;
  const Expression* expression2_;
  const Expression* expression3_;
// If |this| returns a constant value for any arguments, shows it.
  const uint64 result_;

  // Bit flags to figure how much can |this| tree and its aliases cost.
  uint64 bit_costs_;
  // Bit flags to figure which values are used under |this| operation.
  uint8  bit_values_;
};

#endif  // EXPRESSION_H_
