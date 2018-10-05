#include "base.h"

int PopCnt(uint64 bits) {
  bits = (bits & 0x5555555555555555ull) + ((bits >> 1) & 0x5555555555555555ull);
  bits = (bits & 0x3333333333333333ull) + ((bits >> 2) & 0x3333333333333333ull);
  bits = (bits & 0x0f0f0f0f0f0f0f0full) + ((bits >> 4) & 0x0f0f0f0f0f0f0f0full);
  return (bits * 0x0101010101010101ull) >> (64 - 8);
}
