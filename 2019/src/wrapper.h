#pragma once

#include <vector>

#include "point.h"

class Game;

class Wrapper {
 public:
  Wrapper(Game& game, const Point& p, int index, int birth_time);

  Game& game;
  Point pos;
  // Relative position of manipulators from the wrapper.
  std::vector<Point> manipulators;
  const int index;
  const int birth_time;
};
