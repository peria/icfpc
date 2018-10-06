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

void Nanobot::fuse(Nanobot& s) {
  DCHECK(s.is_active);
  seeds |= (s.seeds | (1ull << (s.bid - 1)));
  s.is_active = false;
}
