#pragma once

#include <string>
#include <ostream>

#include "base.h"

struct Bonus {
  enum class Type { kGlobalist, kBreakALeg, kWallhack, kSuperflex };

  static Type GetType(const std::string&);

  Point position;
  Type type;
  int problem;
};

std::ostream& operator<<(std::ostream&, const Bonus::Type&);
