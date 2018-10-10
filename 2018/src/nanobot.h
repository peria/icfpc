#pragma once

#include "coordinate.h"

struct Nanobot {
  int takeSeed1();
  uint64 takeSeeds(int m);
  void fuse(Nanobot& s);

  int bid;
  Coordinate position = Coordinate(0, 0, 0);
  uint64 seeds;  // a bit |1<<(s-1)| figures a seed |s|.
  bool is_active = false;
};
