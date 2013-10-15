#include "expression.h"

#include <cassert>
#include <string>
#include <vector>

#include "base.h"

Expression::Expression(const Operation operation,
                       const uint64 result,
                       const uint64 bit_costs,
                       const uint8 bit_values,
                       Expression* expression1,
                       Expression* expression2,
                       Expression* expression3)
  : operation_(operation), expression1_(expression1),
    expression2_(expression2), expression3_(expression3),
    result_(result), bit_costs_(bit_costs), bit_values_(bit_values) {}

uint64 Expression::Eval(uint64 x, uint64 y, uint64 z) const {
  if (IsConstant())
    return result_;

  switch (operation_) {
    case ZERO:
      return 0;
    case ONE:
      return 1;
    case X:
      return x;
    case Y:
      return y;
    case Z:
      return z;
    case NOT:
      return ~expression1_->Eval(x, y, z);
    case SHL1:
      return expression1_->Eval(x, y, z) << 1;
    case SHR1:
      return expression1_->Eval(x, y, z) >> 1;
    case SHR4:
      return expression1_->Eval(x, y, z) >> 4;
    case SHR16:
      return expression1_->Eval(x, y, z) >>16;
    case AND:
      return expression1_->Eval(x, y, z) & expression2_->Eval(x, y, z);
    case OR:
      return expression1_->Eval(x, y, z) | expression2_->Eval(x, y, z);
    case XOR:
      return expression1_->Eval(x, y, z) ^ expression2_->Eval(x, y, z);
    case PLUS:
      return expression1_->Eval(x, y, z) + expression2_->Eval(x, y, z);
  }

  switch (operation_) {
    case IF0:  // (if0 e1 e2 e3)
      return expression1_->Eval(x, y, z) ?
        expression3_->Eval(x, y, z) : expression2_->Eval(x, y, z);
    case FOLD: { // (fold e1 e2 (lambda (y z) e3))
      uint64 v1 = expression1_->Eval(x, y, z);
      uint64 val = expression2_->Eval(x, y ,z);
      for (int i = 0; i < 64; i += 8)
        val = expression3_->Eval(x, ((v1 >> i) & 0xff), val);
      return val;
    }
    default:
      assert("No reach here");
  }  // switch
  assert("No reach here");
  return 0;
}

const std::string Expression::ToString() const {
  switch (operation_) {
    case ZERO:
      return "0";
    case ONE:
      return "1";
    case X:
      return "x";
    case Y:
      return "y";
    case Z:
      return "z";
    case NOT:
      return "(not " + expression1_->ToString() + ")";
    case SHL1:
      return "(shl1 " + expression1_->ToString() + ")";
    case SHR1:
      return "(shr1 " + expression1_->ToString() + ")";
    case SHR4:
      return "(shr4 " + expression1_->ToString() + ")";
    case SHR16:
      return "(shr16 " + expression1_->ToString() + ")";
    case AND:
      return "(and " + expression1_->ToString() + " " +
          expression2_->ToString() + ")";
    case OR:
      return "(or " + expression1_->ToString() + " " +
        expression2_->ToString() + ")";
    case XOR:
      return "(xor " + expression1_->ToString() + " " +
        expression2_->ToString() + ")";
    case PLUS:
      return "(plus " + expression1_->ToString() + " " +
        expression2_->ToString() + ")";
    case IF0:
      return "(if0 " + expression1_->ToString() + " " +
        expression2_->ToString() + " " + expression3_->ToString() + ")";
    case FOLD:
      return "(fold " + expression1_->ToString() + " " +
        expression2_->ToString() + " (lambda (y z) " +
        expression3_->ToString() + "))";
    default:
      assert("No reach here");
      return " * ";
  }
}

bool Expression::IsValid(const std::vector<uint64>& inputs,
                         const std::vector<uint64>& expects) {
  for (size_t i = 0; i < inputs.size(); ++i) {
    if (Eval(inputs[i]) != expects[i])
      return false;
  }
  return true;
}

bool Expression::IsNonZero() {
  if (IsConstant())
    return !IsZero();
  return false;
}
