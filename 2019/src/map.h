#pragma once

#include <string>
#include <cassert>
#include <vector>
#include <ostream>

#include "point.h"

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
  kSetBeacon = 1 << 7,  // Can't be taken
  kObstacle = 1 << 8,

  // Portable boosters
  kBoosters = kManipulator | kFastWheel | kDrill | kCloning | kBeacon,
};

class Map {
 public:
  using T = int;
  Map() = default;
  Map(const int width, const int height, CellType value = CellType::kObstacle);

  T& operator()(int x, int y) { return data_[x * kHeight + y]; }
  const T& operator()(int x, int y) const { return data_[x * kHeight + y]; }
  T& operator()(const Point& p) { return data_[p.x * kHeight + p.y]; }
  const T& operator()(const Point& p) const {
    return data_[p.x * kHeight + p.y];
  }

  T get(int x, int y) const {
    if (!isInside(x, y)) return CellType::kObstacle;
    return operator()(x, y);
  }
  T get(const Point& p) const {
    if (!isInside(p)) return CellType::kObstacle;
    return operator()(p);
  }

  bool isInside(int x, int y) const {
    return 0 <= x && x < kWidth && 0 <= y && y < kHeight;
  }
  bool isInside(const Point& p) const { return isInside(p.x, p.y); }

  // Wrap the point.
  void wrap(const Point& p) {
    assert(isInside(p));
    int& c = (*this)(p);
    if (c & CellType::kNonWrapped) {
      c &= ~CellType::kNonWrapped;
    }
    // Drill breaks the obstacle.
    if (c & CellType::kObstacle) {
      c &= ~CellType::kObstacle;
    }
  }

  std::string toString() const;

  int kWidth = 0;
  int kHeight = 0;

 private:
  std::vector<T> data_;
};

std::ostream& operator<<(std::ostream&, const Map&);
