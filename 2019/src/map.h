#pragma once

#include <string>

// Bit flags
enum CellType {
  kNone = 0,
  kNonWrapped = 1 << 0,
  kManipulator = 1 << 1,
  kFastWheel = 1 << 2,
  kDrill = 1 << 3,
  kSpawn = 1 << 4,
  kCloning = 1 << 5,
  kBeacon = 1 << 6,
  kResetBeacon = 1 << 7,  // Can't be taken
  kObstacke = 1 << 8,

  // Takable boosters
  kBoosters = kManipulator | kFastWheel | kDrill | kCloning | kBeacon,
};

class Map {
public:
  Map(const std::string& desc_file);
};
