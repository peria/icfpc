#include "bonus.h"

#include <glog/logging.h>
#include <ostream>
#include <string>

Bonus::Type Bonus::GetType(const std::string& name) {
  if (name == "GLOBALIST")
    return Bonus::Type::kGlobalist;
  if (name == "BREAK_A_LEG")
    return Bonus::Type::kBreakALeg;
  if (name == "WALLHACK")
    return Bonus::Type::kWallhack;
  if (name == "SUPERFLEX")
    return Bonus::Type::kSuperflex;
  CHECK(false);
}

std::ostream& operator<<(std::ostream& os, const Bonus::Type& type) {
  switch (type) {
  case Bonus::Type::kGlobalist:
    return os << "GLOBALIST";
  case Bonus::Type::kBreakALeg:
    return os << "BREAK_A_LEG";
  case Bonus::Type::kWallhack:
    return os << "WALLHACK";
  case Bonus::Type::kSuperflex:
    return os << "SUPERFLEX";
  }
  return os << "Unknown";
}
