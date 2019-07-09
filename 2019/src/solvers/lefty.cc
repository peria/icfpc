#include "solver.h"

#include <iostream>

#include "algorithm/go_to_nearest_non_wrapped.h"
#include "game.h"
#include "point.h"

Solver::Solver(Game& game) : game_(game) {}

std::string Solver::solve() {
  static const Point kExpectNonWrapped[][3] = {
    {{2, 1}, {2, 0}, {2, -1}},
    {{1, 2}, {0, 2}, {-1, 2}},
    {{-2, 1}, {-2, 0}, {-2, -1}},
    {{1, -2}, {0, -2}, {-1, -2}},
  };
  static const Point kExpectNonObstacle[] = {
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}
  };

  Wrapper& w = game_.wrappers[0];
  while (true) {
    // Wrapper::Direction = >^<v
    Map& map = game_.map;

    {
      int dir = static_cast<int>(w.direction);
      const Point (&to_check)[3] = kExpectNonWrapped[dir];
      const Point& pos = w.pos;
      static const Action kMoveDir[] = {
        Action::kMoveRight, Action::kMoveUp, Action::kMoveLeft, Action::kMoveDown,
      };
      if (((map.get(pos + kExpectNonObstacle[dir]) & CellType::kObstacle) == 0) &&
          ((map.get(pos + to_check[0]) | map.get(pos + to_check[1]) |
            map.get(pos + to_check[2])) & CellType::kNonWrapped)) {
        w.takeAction(new ActionCommand(kMoveDir[dir]));
        continue;
      }
    }
    {
      int dir = (static_cast<int>(w.direction) + 1) % 4;
      const Point (&to_check)[3] = kExpectNonWrapped[dir];
      const Point& pos = w.pos;
      if (((map.get(pos + kExpectNonObstacle[dir]) & CellType::kObstacle) == 0) &&
          ((map.get(pos + to_check[0]) | map.get(pos + to_check[1]) |
            map.get(pos + to_check[2])) & CellType::kNonWrapped)) {
        w.takeAction(new ActionCommand(Action::kTurnCCW));
        continue;
      }
    }

    if (!algorithm::goToNearestNonWrapped(w, game_.map))
      break;
  }
  return game_.getCommand();
}
