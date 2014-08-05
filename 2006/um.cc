#include "um.h"

#include <cstdint>

namespace {

struct Operator {
  static const Platter kMask = 0xFU << 28;
  static const Platter kMove = 0x0U << 28;
  static const Platter kIndex = 0x1U << 28;
  static const Platter kAmend = 0x2U << 28;
  static const Platter kAdd = 0x3U << 28;
  static const Platter kMult = 0x4U << 28;
  static const Platter kDiv = 0x5U << 28;
  static const Platter kNotAnd = 0x6U << 28;
  static const Platter kHalt = 0x7U << 28;
  static const Platter kAlloc = 0x8U << 28;
  static const Platter kAband = 0x9U << 28;
  static const Platter kOutput = 0xAU << 28;
  static const Platter kInput = 0xBU << 28;
  static const Platter kLoad = 0xCU << 28;
  static const Platter kImmidiate = 0xDU << 28;
};

}  // namespace

UM::UM(Program program)
  : zero_array_(std::move(program)), pc_(0) {}

UM::~UM() {}

void UM::Run() {
  while (Step(program_[pc_]))
    ++pc_;
}

bool UM::Step(const Platter& operation) {
  Platter* reg_a = &register_[(operation >> 6) & 7];
  Platter* reg_b = &register_[(operation >> 3) & 7];
  Platter* reg_c = &register_[(operation >> 0) & 7];
  switch (operation & Operator::kMask) {
  case Operator::kMove: {
    if (*reg_c)
      *reg_a = *reg_b;
    break;
  }
  case Operator::kIndex: {
    *reg_a = memory_[*reg_b][*reg_c];
    break;
  }
  case Operator::kAmend: {
    memory_[*reg_b][*reg_c] = *reg_a;
    break;
  }
  case Operator::kAdd: {
    *reg_a = *reg_b + *reg_c;
    break;
  }
  case Operator::kMult: {
    *reg_a = *reg_b * *reg_c;
    break;
  }
  case Operator::kDiv: {
    *reg_a = *reg_b / *reg_c;
    break;
  }
  case Operator::kNotAnd: {
    *reg_a = ~(*reg_b & *reg_c);
    break;
  }
  case Operator::kHalt: {
    return false;
  }
  case Operator::kAlloc: {
    
    break;
  }
  case Operator::kAband:
    break;
  case Operator::kOputput:
    break;
  case Operator::kInput:
    break;
  case Operator::kLoad:
    break;
  case Operator::kImmidate:
    break;
  }
  return true;
}


