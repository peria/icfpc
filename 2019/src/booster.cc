#include "booster.h"

#include <iostream>

bool isPortableBooster(const Booster& booster) {
  switch (booster) {
  case Booster::kManipulator:
  case Booster::kFastWheel:
  case Booster::kDrill:
  case Booster::kCloning:
  case Booster::kBeacon:
    return true;
  default:
    return false;
  }
}

Booster charToBooster(const char c) {
  switch (c) {
  case 'B':
    return Booster::kManipulator;
  case 'F':
    return Booster::kFastWheel;
  case 'L':
    return Booster::kDrill;
  case 'C':
    return Booster::kCloning;
  case 'R':
    return Booster::kBeacon;
  default:
    std::cerr << "Unknown char for Booster: " << c << "\n";
  }
  // Do not reach here.
  return Booster::kManipulator;
}
