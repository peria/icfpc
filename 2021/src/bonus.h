#pragma once

#include <ostream>
#include <string>

#include "base.h"
#include "geo.h"

struct Bonus {
  enum class Type { kGlobalist, kBreakALeg, kWallhack, kSuperflex };

  static Type GetType(const std::string&);

  Point position;
  Type type;
  int problem;
};

std::ostream& operator<<(std::ostream&, const Bonus::Type&);
