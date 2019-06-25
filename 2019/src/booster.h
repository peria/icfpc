#pragma once

#include <cstdint>

#include "point.h"

enum class Booster : uint8_t {
  kManipulator = 0,
  kFastWheel = 1,
  kDrill = 2,
  kCloning = 3,
  kBeacon = 4,  // will be the target point of teleport.
  NumPortable = 5,
  kSpawn = 6,
};

bool isPortableBooster(const Booster& booster);
Booster charToBooster(const char c);

struct BoosterPos {
  Booster booster;
  Point pos;
};
