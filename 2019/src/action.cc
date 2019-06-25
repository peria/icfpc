#include "action.h"

#include <cassert>
#include <iostream>

Booster actionToBooster(Action a) {
  switch (a) {
  case Action::kAddManipulator:
    return Booster::kManipulator;
  case Action::kUseFastWheel:
    return Booster::kFastWheel;
  case Action::kUseDrill:
    return Booster::kDrill;
  case Action::kUseBeacon:
    return Booster::kBeacon;
  case Action::kUseCloning:
    return Booster::kCloning;
  default:
    std::cerr << "Trying to use unknown booster in action: "
              << static_cast<int>(a) << "\n";
    return Booster::kInvalid;
  }
}

ActionInfo::ActionInfo(Action a) : action(a) {}

MoveInfo::MoveInfo(Action a, bool fast, bool drill)
    : ActionInfo(a), is_fast_wheel_active(fast), is_drill_active(drill) {
  assert(a == Action::kMoveUp || a == Action::kMoveDown ||
         a == Action::kMoveLeft || a == Action::kMoveRight);
}

NopInfo::NopInfo() : ActionInfo(Action::kDoNothing) {}

TurnInfo::TurnInfo(Action a) : ActionInfo(a) {
  assert(a == Action::kTurnCW || a == Action::kTurnCCW);
}

UseBoosterInfo::UseBoosterInfo(Action a) : ActionInfo(a) {
  assert(a == Action::kUseFastWheel || a == Action::kUseDrill ||
         a == Action::kUseBeacon || a == Action::kUseCloning ||
         a == Action::kAddManipulator);
}

AddManipulatorInfo::AddManipulatorInfo(const Point& p)
    : UseBoosterInfo(Action::kAddManipulator), pos(p) {}

TeleportInfo::TeleportInfo(const Point& old_p, const Point& new_p)
    : ActionInfo(Action::kTeleport), old_pos(old_p), new_pos(new_p) {}

ActionCommand::ActionCommand(Action a) : action(a) {}

ActionCommandWithPos::ActionCommandWithPos(Action a, const Point& p)
    : ActionCommand(a), pos(p) {}
