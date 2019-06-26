#include "wrapper.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "action.h"
#include "game.h"
#include "map.h"

Wrapper::Wrapper(Game& g, const Point& p, int id, int birth)
    : game(g), pos(p), index(id), birth_time(birth) {
  manipulators.emplace_back(Point{1, 1});
  manipulators.emplace_back(Point{1, 0});
  manipulators.emplace_back(Point{1, -1});

  moveAndPaint(pos);
}

void Wrapper::takeAction(const ActionCommand& cmd) {
  beforeAction();

  switch (cmd.action) {
  case Action::kDoNothing:
    last_action.reset(new NopInfo());
    break;
  case Action::kMoveUp:
  case Action::kMoveDown:
  case Action::kMoveLeft:
  case Action::kMoveRight: {
    bool is_fast_wheel_active = (time_fast_wheel > 0);
    bool is_drill_active = (time_drill > 0);
    Map& map = game.map;
    last_action.reset(
        new MoveInfo(cmd.action, is_fast_wheel_active, is_drill_active));
    int id = static_cast<int>(cmd.action) - static_cast<int>(Action::kMoveUp);
    static constexpr Point dp[] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
    Point p = pos + dp[id];
    assert(map.isInside(p) &&
           (is_drill_active || (map(p) & CellType::kObstacle) == 0));
    moveAndPaint(p);

    if (is_fast_wheel_active) {
      p += dp[id];
      if (map.isInside(p) &&
          (is_drill_active || (map(p) & CellType::kObstacle) == 0)) {
        moveAndPaint(p);
      }
    }
    break;
  }
  case Action::kTurnCW:
  case Action::kTurnCCW:
    last_action.reset(new TurnInfo(cmd.action));
    if (cmd.action == Action::kTurnCW) {
      for (auto& m : manipulators) {
        m = Point{m.y, -m.x};
      }
    } else {
      for (auto& m : manipulators) {
        m = Point{-m.y, m.x};
      }
    }
    moveAndPaint(pos);
    break;
  case Action::kUseFastWheel:
  case Action::kUseDrill:
  case Action::kUseBeacon:
  case Action::kUseCloning: {
    last_action.reset(new UseBoosterInfo(cmd.action));
    int booster_id = static_cast<int>(actionToBooster(cmd.action));
    assert(game.num_boosters[booster_id]);
    --game.num_boosters[booster_id];
    useBoosterAction(cmd.action);
    break;
  }
  case Action::kAddManipulator: {
    const Point& p = cmd.As<ActionCommandWithPos>().pos;
    assert(pos != p);
    assert(find(manipulators.begin(), manipulators.end(), p) ==
           manipulators.end());

    last_action.reset(new AddManipulatorInfo(p));
    int manip_id = static_cast<int>(Booster::kManipulator);
    assert(game.num_boosters[manip_id]);
    --game.num_boosters[manip_id];

    bool succeed = false;
    for (auto& m : manipulators) {
      Point d = p - m;
      if (std::abs(d.x) + std::abs(d.y) == 1) {
        succeed = true;
        manipulators.push_back(p);
        break;
      }
    }
    assert(succeed);
    break;
  }
  case Action::kTeleport:
    const Point& p = cmd.As<ActionCommandWithPos>().pos;
    assert(game.map(p) & CellType::kSetBeacon);
    last_action.reset(new TeleportInfo(pos, p));
    pos = p;
    break;
  }

  afterAction();
}

void Wrapper::beforeAction() {
  // Pick up items based on the last action.
  if (!last_action)
    return;

  if (last_action->doesMove() || last_action->doesTeleport()) {
    game.pickUpBooster(pos);
  }
  if (last_action->doesMove() &&
      last_action->As<MoveInfo>().is_fast_wheel_active) {
    if (last_action->action == Action::kMoveUp) {
      game.pickUpBooster(pos + Point{0, -1});
    } else if (last_action->action == Action::kMoveDown) {
      game.pickUpBooster(pos + Point{0, 1});
    } else if (last_action->action == Action::kMoveLeft) {
      game.pickUpBooster(pos + Point{1, 0});
    } else if (last_action->action == Action::kMoveRight) {
      game.pickUpBooster(pos + Point{-1, 0});
    }
  }
}

void Wrapper::afterAction() {
  // Time duration decrease.
  if (time_fast_wheel > 0)
    --time_fast_wheel;
  if (time_drill > 0)
    --time_drill;
}

namespace {

std::vector<Point> enumerateCellsToCheck(const Point& offset) {
  std::vector<Point> res;
  Point dp{std::abs(offset.x), std::abs(offset.y)};
  Point dir{0 < offset.x ? 1 : -1, 0 < offset.y ? 1 : -1};
  Point p{0, 0};
  int err = 0;
  while (p != offset) {
    res.push_back(p);
    int err1 = err + dp.y;
    int err2 = err - dp.x;
    if (std::abs(err1) == std::abs(err2)) {
      p += dir;
      err += dp.y - dp.x;
    } else if (std::abs(err1) < std::abs(err2)) {
      p.x += dir.x;
      err = err1;
    } else {
      p.y += dir.y;
      err = err2;
    }
  }
  return res;
}

}  // namespace

void Wrapper::moveAndPaint(const Point& dest) {
  Map& map = game.map;

  pos = dest;
  map.wrap(pos);
  for (auto& m : manipulators) {
    bool is_blocked = false;
    Point mpos = pos + m;
    if (!map.isInside(mpos) || (map(mpos) & CellType::kObstacle)) {
      continue;
    }
    for (auto& dp : enumerateCellsToCheck(m)) {
      const Point p = pos + dp;
      if (map(p) & CellType::kObstacle) {
        is_blocked = true;
        break;
      }
    }
    if (!is_blocked) {
      map.wrap(mpos);
    }
  }
}

void Wrapper::useBoosterAction(Action a) {
  switch (a) {
  case Action::kUseFastWheel:
    time_fast_wheel += 50 + 1;  // 1 is decreased in afterAction()
    break;
  case Action::kUseDrill:
    time_drill += 30 + 1;  // 1 is decreased in afterAction()
  case Action::kUseBeacon:
    game.map(pos) &= CellType::kSetBeacon;
  case Action::kUseCloning: {
    int index = game.wrappers.size();
    int birth = game.time + 1;
    game.wrappers.emplace_back(game, pos, index, birth);
    break;
  }
  default:
    std::cerr << "Unexpected action: " << static_cast<int>(a) << "\n";
  }
}
