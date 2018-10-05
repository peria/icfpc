#include "nanobot.h"

int Nanobot::takeSeed1() {
  uint64 lsb = seeds & -seeds;
  seeds ^= lsb;
  return PopCnt(lsb - 1) + 1;
}

uint64 Nanobot::takeSeeds(int m) {
  DCHECK_GE(PopCnt(seeds), m);

  uint64 ret = 0;
  for (int i = 0; i < m; ++i) {
    uint64 lsb = seeds & -seeds;
    ret |= lsb;
    seeds ^= lsb;
  }
  return ret;
}
