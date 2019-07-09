#include "solver.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "algorithm/go_to_nearest_non_wrapped.h"
#include "game.h"
#include "point.h"

namespace {

// Returns directed Point, assuming |p| is for right direction.
Point forDirection(const Point& p, const int d) {
  switch (d) {
  case 0: return p;
  case 1: return {-p.y, p.x};
  case 2: return {-p.x, -p.y};
  case 3: return {p.y, -p.x};
  }
  return {0, 0};
}

}  // namespace

Solver::Solver(Game& game) : game_(game) {}

std::string Solver::solve() {
  static const Action kMoveForward[] = {
    Action::kMoveRight, Action::kMoveUp, Action::kMoveLeft, Action::kMoveDown,
  };

  Wrapper& w = game_.wrappers[0];
  while (true) {
    // Wrapper::Direction = >^<v
    // std::cout << game_;
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Map& map = game_.map;
    const Point& pos = w.pos;
    const int dir = static_cast<int>(w.direction);

    // #>? : Forward
    if ((map.get(pos + forDirection(Point {-1, 0}, dir)) & CellType::kObstacle) &&
        !(map.get(pos + forDirection(Point {1, 0}, dir)) & CellType::kObstacle)) {
      w.takeAction(new ActionCommand(kMoveForward[dir]));
      continue;
    }

    // ?.
    //  ?
    //  >  : Turn left
    if ((map.get(pos + forDirection(Point {0, 2}, dir)) & CellType::kNonWrapped) &&
        !(map.get(pos + forDirection(Point {-1, 2}, dir)) & CellType::kObstacle) &&
        !(map.get(pos + forDirection(Point {0, 1}, dir)) & CellType::kObstacle)) {
      w.takeAction(new ActionCommand(Action::kTurnCCW));
      continue;
    }

    //   .
    // >?.  : Forward
    //   .
    if (!(map.get(pos + forDirection(Point {1, 0}, dir)) & CellType::kObstacle) &&
        ((map.get(pos + forDirection(Point {2, 0}, dir)) & CellType::kNonWrapped) ||
         (map.get(pos + forDirection(Point {2, 1}, dir)) & CellType::kNonWrapped) ||
         (map.get(pos + forDirection(Point {2, -1}, dir)) & CellType::kNonWrapped))) {
      w.takeAction(new ActionCommand(kMoveForward[dir]));
      continue;
    }

    //  >  : Turn right
    //  ? 
    // ...
    if (!(map.get(pos + forDirection(Point {0, -1}, dir)) & CellType::kObstacle) &&
        ((map.get(pos + forDirection(Point {0, -2}, dir)) & CellType::kNonWrapped) ||
         (map.get(pos + forDirection(Point {1, -2}, dir)) & CellType::kNonWrapped) ||
         (map.get(pos + forDirection(Point {-1, -2}, dir)) & CellType::kNonWrapped))) {
      w.takeAction(new ActionCommand(Action::kTurnCW));
      continue;
    }

    if (!algorithm::goToNearestNonWrapped(w, game_.map))
      break;
  }
  return game_.getCommand();
}
