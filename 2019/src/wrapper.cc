#include "wrapper.h"

#include <cassert>

#include "action.h"

Wrapper::Wrapper(Game& g, const Point& p, int id, int birth)
    : game(g), pos(p), index(id), birth_time(birth) {
  manipulators.emplace_back(Point{1, 1});
  manipulators.emplace_back(Point{1, 0});
  manipulators.emplace_back(Point{1, -1});
}

void Wrapper::takeAction(const ActionCommand& cmd) {
  beforeAction();

  switch (cmd.action) {
  case Action::kDoNothing:
    action_history.emplace_back(new NopInfo());
    break;
  case Action::kMoveUp:
  case Action::kMoveDown:
  case Action::kMoveLeft:
  case Action::kMoveRight: {
    bool is_fast_wheel_active = (time_fast_wheel > 0);
    bool is_drill_active = (time_drill > 0);
    action_history.emplace_back(
        new MoveInfo(cmd.action, is_fast_wheel_active, is_drill_active));
    // TODO: Write actual behavior
    break;
  }
  case Action::kTurnCW:
  case Action::kTurnCCW:
    action_history.emplace_back(new TurnInfo(cmd.action));
    // TODO: Write actual behavior
    break;
  case Action::kUseFastWheel:
  case Action::kUseDrill:
  case Action::kUseBeacon:
  case Action::kUseCloning:
    action_history.emplace_back(new UseBoosterInfo(cmd.action));
    // TODO: Write actual behavior
    break;
  case Action::kAddManipulator: {
    assert(cmd.doesNeedPos());
    const Point& pos = cmd.As<ActionCommandWithPos>().pos;
    action_history.emplace_back(new AddManipulatorInfo(pos));
    // TODO: Write actual behavior
    break;
  }
  case Action::kTeleport:
    assert(cmd.doesNeedPos());
    const Point& pos = cmd.As<ActionCommandWithPos>().pos;
    action_history.emplace_back(new TeleportInfo(pos));
    // TODO: Write actual behavior
    break;
  }

  afterAction();
}

void Wrapper::beforeAction() {
  // Pick up items based on the last action.
}

void Wrapper::afterAction() {
  // Wrap cells
}
