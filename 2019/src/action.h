#pragma once

#include <cstdint>

#include "booster.h"
#include "point.h"

enum class Action : std::uint8_t {
  kMoveUp,          // W
  kMoveDown,        // S
  kMoveLeft,        // A
  kMoveRight,       // D
  kDoNothing,       // Z
  kTurnCW,          // E
  kTurnCCW,         // Q
  kAddManipulator,  // B(x,y)
  kUseFastWheel,    // F
  kUseDrill,        // L
  kUseBeacon,       // R
  kUseCloning,      // C
  kTeleport,        // T(x,y)
};

Booster actionToBooster(Action a);

struct ActionInfo {
  const Action action;

  ActionInfo(Action a);
  virtual ~ActionInfo() = default;

  virtual bool doesMove() const { return false; }
  virtual bool doesNothing() const { return false; }
  virtual bool doesTurn() const { return false; }
  virtual bool doesUseBooster() const { return false; }
  virtual bool doesTeleport() const { return false; }

  // Downcast to delived structs.
  template <typename T>
  T& As() {
    return *dynamic_cast<T*>(this);
  }
  template <typename T>
  const T& As() const {
    return *dynamic_cast<const T*>(this);
  }
};

struct MoveInfo final : public ActionInfo {
  MoveInfo(Action a, bool fast, bool drill);
  bool doesMove() const { return true; }

  const bool is_fast_wheel_active;
  const bool is_drill_active;
};

struct NopInfo final : public ActionInfo {
  NopInfo();
  bool doesNothing() const { return true; }
};

struct TurnInfo final : public ActionInfo {
  TurnInfo(Action a);
  bool doesTurn() const { return true; }
};

struct UseBoosterInfo : public ActionInfo {
  UseBoosterInfo(Action a);
  bool doesUseBooster() const final { return true; }
};

struct AddManipulatorInfo final : public UseBoosterInfo {
  AddManipulatorInfo(const Point& p);
  const Point pos;
};

struct TeleportInfo final : public ActionInfo {
  TeleportInfo(const Point& old_p, const Point& new_p);
  bool doesTeleport() const { return true; }
  const Point old_pos;
  const Point new_pos;
};

struct ActionCommand {
  const Action action;

  ActionCommand(Action a);
  virtual ~ActionCommand() = default;

  virtual bool doesNeedPos() const { return false; }

  // Downcast to delived structs.
  template <typename T>
  const T& As() const {
    return *dynamic_cast<const T*>(this);
  }
};

struct ActionCommandWithPos final : public ActionCommand {
  ActionCommandWithPos(Action a, const Point& p);
  bool doesNeedPos() const { return true; }
  const Point pos;
};
