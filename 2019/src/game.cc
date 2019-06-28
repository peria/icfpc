#include "game.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "booster.h"
#include "map.h"
#include "parser.h"
#include "point.h"

namespace {

using Polygon = std::vector<Point>;
using Polygons = std::vector<Polygon>;

// Fills inside of |polygon| with |value|, on |data|.
void fillPolygon(const Polygon& polygon, CellType value, Map& map) {
  std::vector<std::vector<int>> vertical_lines(map.kWidth);
  for (int i = 0; i < polygon.size(); ++i) {
    int j = (i + 1) % polygon.size();
    const Point& p0 = polygon[i];
    const Point& p1 = polygon[j];
    if (p0.x != p1.x) {
      int xmin = std::min(p0.x, p1.x);
      int xmax = std::max(p0.x, p1.x);
      for (int x = xmin; x < xmax; ++x) {
        vertical_lines[x].push_back(p0.y);
      }
    }
  }
  for (int x = 0; x < map.kWidth; ++x) {
    auto& xlines = vertical_lines[x];
    assert(xlines.size() % 2 == 0);
    sort(xlines.begin(), xlines.end());
    for (int i = 0; i < xlines.size(); i += 2) {
      for (int y = xlines[i]; y < xlines[i + 1]; ++y) {
        map(x, y) = value;
      }
    }
  }
}

}  // namespace

GameInitializer::GameInitializer(const std::string& desc,
                                 const std::string& buy) {
  const char* ptr = desc.data();
  Polygon map_polygon = parse<Polygon>(ptr);
  assert(*ptr == '#');
  wrapper_pos = parse<Point>(++ptr);
  assert(*ptr == '#');
  Polygons obstacles = parse<Polygons>(++ptr);
  assert(*ptr == '#');
  std::vector<BoosterPos> boosters = parse<std::vector<BoosterPos>>(++ptr);

  // Map
  int height = 0;
  int width = 0;
  for (auto& p : map_polygon) {
    width = std::max(width, p.x);
    height = std::max(height, p.y);
  }
  map = Map(width, height, CellType::kObstacle);
  fillPolygon(map_polygon, CellType::kNonWrapped, map);
  for (auto& obst : obstacles) {
    fillPolygon(obst, CellType::kObstacle, map);
  }

  // Booster
  for (auto& booster_pos : boosters) {
    const Booster& b = booster_pos.booster;
    const Point& pos = booster_pos.pos;
    switch (b) {
    case Booster::kManipulator:
      map(pos) |= CellType::kManipulator;
      break;
    case Booster::kFastWheel:
      map(pos) |= CellType::kFastWheel;
      break;
    case Booster::kDrill:
      map(pos) |= CellType::kDrill;
      break;
    case Booster::kCloning:
      map(pos) |= CellType::kCloning;
      break;
    case Booster::kBeacon:
      map(pos) |= CellType::kBeacon;
      break;
    case Booster::kSpawn:
      map(pos) |= CellType::kSpawn;
      break;
    default:
      std::cerr << "Unknown booster type: " << static_cast<int>(b) << "\n";
      assert(false);
    }
  }
  for (int i = 0; i < static_cast<int>(Booster::NumPortable); ++i) {
    num_boosters[i] = 0;
  }
  // Additional boosters
  for (char c : buy) {
    Booster b = charToBooster(c);
    assert(isPortableBooster(b));
    ++num_boosters[static_cast<int>(b)];
  }
}

Game::Game(const GameInitializer& initializer)
    : map(initializer.map), num_boosters(initializer.num_boosters) {
  wrappers.emplace_back(*this, initializer.wrapper_pos, 0, 0);
  for (int i = 0; i < static_cast<int>(Booster::NumPortable); ++i) {
    num_boosters[i] = initializer.num_boosters[i];
  }
}

void Game::pickUpBooster(const Point& pos) {
  if ((map(pos) & CellType::kBoosters) == 0)
    return;
  if (map(pos) & CellType::kManipulator) {
    map(pos) &= ~CellType::kManipulator;
    ++num_boosters[static_cast<int>(Booster::kManipulator)];
  } else if (map(pos) & CellType::kFastWheel) {
    map(pos) &= ~CellType::kFastWheel;
    ++num_boosters[static_cast<int>(Booster::kFastWheel)];
  } else if (map(pos) & CellType::kDrill) {
    map(pos) &= ~CellType::kDrill;
    ++num_boosters[static_cast<int>(Booster::kDrill)];
  } else if (map(pos) & CellType::kCloning) {
    map(pos) &= ~CellType::kCloning;
    ++num_boosters[static_cast<int>(Booster::kCloning)];
  } else if (map(pos) & CellType::kBeacon) {
    map(pos) &= ~CellType::kBeacon;
    ++num_boosters[static_cast<int>(Booster::kBeacon)];
  }
}

void Game::replayFromInit(GameInitializer& initializer) {
  // Reset to the initial state.
  map = initializer.map;
  wrappers[0].reset(initializer.wrapper_pos);
  for (int i = 0; i < num_boosters.size(); ++i) {
    num_boosters[i] = initializer.num_boosters[i];
  }
  time = 0;

  // Run wrappers.
  for (bool valid = true; valid;) {
    for (auto& w : wrappers) {
      valid = valid && w.replayAction(time);
    }
    ++time;
  }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
  const Map& map = game.map;
  std::string str(map.toString());
  for (auto& w : game.wrappers) {
    int x = w.pos.x;
    int y = w.pos.y;
    int dy = map.kHeight - 1 - y;
    str[dy * (map.kWidth + 1) + x] = '@';
    for (auto& m : w.manipulators) {
      int mx = x + m.x;
      int my = y + m.y;
      int dmy = map.kHeight - 1 - my;
      if (map.isInside(mx, my) && str[dmy * (map.kWidth + 1) + mx] != '#') {
        str[dmy * (map.kWidth + 1) + mx] = 'o';
      }
    }
  }

  int t = game.time;
  for (auto& w : game.wrappers) {
    t = std::max(t, w.getTime());
  }

  os << "Time: " << t << "\n" << str << "Boosters:";
  for (int i = 0; i < static_cast<int>(Booster::NumPortable); ++i) {
    os << " " << ("BFLCR"[i]) << "(" << game.num_boosters[i] << ")";
  }
  os << "\n";

  return os;
}
