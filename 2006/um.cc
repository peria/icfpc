#include "um.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>

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
  static const Platter kFree = 0x9U << 28;  // Abandon
  static const Platter kOutput = 0xAU << 28;
  static const Platter kInput = 0xBU << 28;
  static const Platter kLoad = 0xCU << 28;
  static const Platter kImmediate = 0xDU << 28;
};

}  // namespace

UM::UM(Program program) : pc_(0), memory_base_(1) {
  memory_.resize(100);
  memory_[0] = std::move(program);
}

UM::~UM() {}

void UM::Run() {
  while (true) {
    const Platter& operation = memory_[0][pc_ + 1];
    ++pc_;
    if (!Step(operation))
      break;
  }
}

bool UM::Step(const Platter& operation) {
  int reg_a_id = (operation >> 6) & 7;
  int reg_b_id = (operation >> 3) & 7;
  int reg_c_id = (operation >> 0) & 7;

  Platter* reg_a = &registers_[reg_a_id];
  Platter* reg_b = &registers_[reg_b_id];
  Platter* reg_c = &registers_[reg_c_id];

  switch (operation & Operator::kMask) {
  case Operator::kMove: {
    if (*reg_c)
      *reg_a = *reg_b;
    break;
  }
  case Operator::kIndex: {
    *reg_a = memory_[*reg_b][*reg_c + 1];
    break;
  }
  case Operator::kAmend: {
    memory_[*reg_a][*reg_b + 1] = *reg_c;
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
    const size_t size = *reg_c;
    std::unique_ptr<Platter[]> array(new Platter[size + 1]);
    std::memset(array.get(), 0, (size + 1) * sizeof(Platter));
    array[0] = size;
    
    memory_[memory_base_] = std::move(array);
    *reg_b = memory_base_;

    ++memory_base_;
    if (memory_base_ >= memory_.size())
      memory_.resize(memory_base_ * 2);
    break;
  }
  case Operator::kFree: {
    memory_[*reg_c].reset();
    break;
  }
  case Operator::kOutput: {
    std::putchar(static_cast<char>(*reg_c));
    break;
  }
  case Operator::kInput: {
    unsigned char c = 255;
    if (!std::feof(stdin))
      c = std::getchar();
    *reg_c = c;
    break;
  }
  case Operator::kLoad: {
    if (*reg_b) {
      size_t size = memory_[*reg_b][0];
      memory_[0].reset(new Platter[size + 1]);
      Platter* src = memory_[*reg_b].get();
      Platter* dst = memory_[0].get();
      std::memcpy(dst, src, (size + 1) * sizeof(Platter));
    }
    pc_ = *reg_c;
    break;
  }
  case Operator::kImmediate: {
    reg_a = &registers_[(operation >> 25) & 7];
    *reg_a = operation & ((1U << 25) - 1);
    break;
  }
  default: {
    assert(false);
    return false;
  }
  }

  return true;
}
