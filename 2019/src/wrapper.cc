#include "wrapper.h"

Wrapper::Wrapper(Game& g, const Point& p, int id, int birth)
  : game(g) , pos(p), index(id), birth_time(birth) {
  manipulators.emplace_back(Point {1, 1});
  manipulators.emplace_back(Point {1, 0});
  manipulators.emplace_back(Point {1, -1});
}
