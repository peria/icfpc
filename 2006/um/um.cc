#include "um.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>

void Memory::ConvertEndian() {
  // Test environment's endian.
  Platter p = 1;
  std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(&p);
  if (bytes[0] == 0)
    return;

  static constexpr Platter kMask1 = 0x00FF00FFU;
  static constexpr Platter kMask2 = 0x0000FFFFU;

  for (int i = 0; i < size; ++i) {
    Platter& platter = array[i];
    platter = ((platter >> 8) & kMask1) | ((platter & kMask1) << 8);
    platter = ((platter >> 16) & kMask2) | ((platter & kMask2) << 16);
  }
}

namespace {

enum Operator : Platter {
  kMask = 0xFU << 28,
  kMove = 0x0U << 28,
  kIndex = 0x1U << 28,
  kAmend = 0x2U << 28,
  kAdd = 0x3U << 28,
  kMult = 0x4U << 28,
  kDiv = 0x5U << 28,
  kNotAnd = 0x6U << 28,
  kHalt = 0x7U << 28,
  kAlloc = 0x8U << 28,
  kFree = 0x9U << 28,  // Abandon
  kOutput = 0xAU << 28,
  kInput = 0xBU << 28,
  kLoad = 0xCU << 28,
  kImmediate = 0xDU << 28,
  kFullBit = std::numeric_limits<Platter>::max(),
  kImmMask = (1U << 25) - 1,
};

}  // namespace

UM::UM(Memory&& program) : pc_(0) {
  memory_.reserve(100);
  memory_.emplace_back(std::move(program));
}

void UM::Run() {
  while (Step(memory_[0].array[pc_++])) {
  }
}

bool UM::Step(const Platter& operation) {
  int reg_a_id = (operation >> 6) & 7;
  int reg_b_id = (operation >> 3) & 7;
  int reg_c_id = (operation >> 0) & 7;

  Platter& reg_a = registers_[reg_a_id];
  Platter& reg_b = registers_[reg_b_id];
  Platter& reg_c = registers_[reg_c_id];

  switch (operation & Operator::kMask) {
  case Operator::kMove: {
    if (reg_c)
      reg_a = reg_b;
    break;
  }
  case Operator::kIndex: {
    reg_a = memory_[reg_b].array[reg_c];
    break;
  }
  case Operator::kAmend: {
    memory_[reg_a].array[reg_b] = reg_c;
    break;
  }
  case Operator::kAdd: {
    reg_a = reg_b + reg_c;
    break;
  }
  case Operator::kMult: {
    reg_a = reg_b * reg_c;
    break;
  }
  case Operator::kDiv: {
    reg_a = reg_b / reg_c;
    break;
  }
  case Operator::kNotAnd: {
    reg_a = ~(reg_b & reg_c);
    break;
  }
  case Operator::kHalt: {
    return false;
  }
  case Operator::kAlloc: {
    const size_t size = reg_c;
    reg_b = memory_.size();
    memory_.emplace_back(std::make_unique<Platter[]>(size), size);
    std::memset(memory_.back().array.get(), 0, size * sizeof(Platter));
    break;
  }
  case Operator::kFree: {
    memory_[reg_c].array.reset();
    memory_[reg_c].size = 0;
    break;
  }
  case Operator::kOutput: {
    std::putchar(static_cast<char>(reg_c));
    break;
  }
  case Operator::kInput: {
    if (std::feof(stdin))
      reg_c = Operator::kFullBit;
    else
      reg_c = std::getchar();
    break;
  }
  case Operator::kLoad: {
    if (reg_b) {
      size_t size = memory_[reg_b].size;
      memory_[0].array.reset(new Platter[size]);
      memory_[0].size = size;

      Platter* src = memory_[reg_b].array.get();
      Platter* dst = memory_[0].array.get();
      std::memcpy(dst, src, size * sizeof(Platter));
    }
    pc_ = reg_c;
    break;
  }
  case Operator::kImmediate: {
    registers_[(operation >> 25) & 7] = operation & Operator::kImmMask;
    break;
  }
  default: {
    assert(false);
    return false;
  }
  }

  return true;
}
