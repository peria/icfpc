#pragma once

#include "coordinate.h"
#include "trace.h"
#include "matrix.h"

struct Nanobot {
  int takeSeed1();
  uint64 takeSeeds(int m);
  void fuse(Nanobot& s);

  // Append trace to get |dst| in |matrix| field. Returns true if this bot can reach |dst|.
  bool goTo(const Matrix& matrix, const Coordinate& dst);

  int bid;
  Coordinate position = Coordinate(0, 0, 0);
  uint64 seeds;  // a bit |1<<(s-1)| figures a seed |s|.
  bool is_active = false;

  Trace trace;
};
