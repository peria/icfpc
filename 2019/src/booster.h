#pragma once

#include <cstdint>

#include "point.h"

enum class Booster : uint8_t {
  kManipulator,
  kFastWheel,
  kDrill,
  kSpawn,
  kCloning,
  kBeacon,  // will be the target point of teleport.
  N,
};

struct BoosterPos {
  Booster booster;
  Point pos;
};
