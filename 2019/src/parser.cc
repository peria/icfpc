#include "parser.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "action.h"
#include "booster.h"
#include "point.h"

namespace {
using Polygon = std::vector<Point>;
using Polygons = std::vector<Polygon>;
}  // namespace

template <>
Point parse(const char*& p) {
  assert(*p == '(');
  int x = std::strtol(++p, const_cast<char**>(&p), 10);
  assert(*p == ',');
  int y = std::strtol(++p, const_cast<char**>(&p), 10);
  assert(*p == ')');
  ++p;
  return {x, y};
}

template <>
Polygon parse(const char*& p) {
  Polygon polygon;
  polygon.emplace_back(parse<Point>(p));
  while (*p == ',') {
    polygon.emplace_back(parse<Point>(++p));
  }
  return polygon;
}

template <>
Polygons parse(const char*& p) {
  Polygons polygons;
  if (*p == '(') {
    polygons.emplace_back(parse<Polygon>(p));
    while (*p == ';') {
      polygons.emplace_back(parse<Polygon>(++p));
    }
  }
  return polygons;
}

template <>
BoosterPos parse(const char*& p) {
  Booster b;
  switch (*p) {
  case 'B':
    b = Booster::kManipulator;
    break;
  case 'F':
    b = Booster::kFastWheel;
    break;
  case 'L':
    b = Booster::kDrill;
    break;
  case 'X':
    b = Booster::kSpawn;
    break;
  case 'C':
    b = Booster::kCloning;
    break;
  case 'R':
    b = Booster::kBeacon;
    break;
  default:
    assert(false);
  };
  Point pos = parse<Point>(++p);
  return {b, pos};
}

template <>
std::vector<BoosterPos> parse(const char*& p) {
  std::vector<BoosterPos> booster_pos;
  if (*p && std::strchr("BFLXCR", *p)) {
    booster_pos.emplace_back(parse<BoosterPos>(p));
    while (*p == ';') {
      booster_pos.emplace_back(parse<BoosterPos>(++p));
    }
  }
  return booster_pos;
}

template <>
ActionCommand parse(const char*& p) {
  Action a;
  switch (*p) {
  case 'W':
    a = Action::kMoveUp;
    break;
  case 'S':
    a = Action::kMoveDown;
    break;
  case 'D':
    a = Action::kMoveRight;
    break;
  case 'A':
    a = Action::kMoveLeft;
    break;
  case 'Z':
    a = Action::kDoNothing;
    break;
  case 'E':
    a = Action::kTurnCW;
    break;
  case 'Q':
    a = Action::kTurnCCW;
    break;
  case 'F':
    a = Action::kUseFastWheel;
    break;
  case 'L':
    a = Action::kUseDrill;
    break;
  case 'R':
    a = Action::kUseBeacon;
    break;
  case 'C':
    a = Action::kUseCloning;
    break;
  }
  ++p;
  return ActionCommand(a);
}
