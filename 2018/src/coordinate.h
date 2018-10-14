#pragma once

#include <cmath>

#include "base.h"

struct Coordinate {
  struct Hash {
    int operator()(const Coordinate& c) const {
      return (((c.x << 8) + c.y) << 8) + c.z;
    }
  };

  Coordinate(int x, int y, int z) : x(x), y(y), z(z) {}
  Coordinate(const Coordinate&) = default;
  Coordinate(int h) : Coordinate(h >> 16, (h >> 8) & 0xff, h & 0xff) {}

  int mLen() const { return std::abs(x) + std::abs(y) + std::abs(z); }
  int cLen() const {
    return std::max(std::abs(x), std::max(std::abs(y), std::abs(z)));
  }
  bool isLinear() const {
    return ((x | y) == 0 || (y | z) == 0 || (z | x) == 0) && (x | y | z);
  }

  Coordinate& operator+=(const Coordinate& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  Coordinate operator+(const Coordinate& other) const {
    return Coordinate(x + other.x, y + other.y, z + other.z);
  }
  Coordinate& operator-=(const Coordinate& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  Coordinate operator-(const Coordinate& other) const {
    return Coordinate(x - other.x, y - other.y, z - other.z);
  }
  bool operator==(const Coordinate& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
  bool operator!=(const Coordinate& other) const {
    return !operator==(other);
  }

  int x = 0;
  int y = 0;
  int z = 0;
};

std::ostream& operator<<(std::ostream& ost, const Coordinate& c);

struct SLD : public Coordinate {
  SLD(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK(isLinear());
    DCHECK_LE(cLen(), 5);
  }

  // Bit encode
  uint8 a() const {
    if (x)
      return 0b01;
    if (y)
      return 0b10;
    if (z)
      return 0b11;
    NOT_IMPLEMENTED();
    return 0;
  }
  uint8 i() const { return x + y + z + 5; }
};

struct LLD : public Coordinate {
  LLD(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK(isLinear());
    DCHECK_LE(cLen(), 15);
  }

  // Bit encode
  uint8 a() const {
    if (x)
      return 0b01;
    if (y)
      return 0b10;
    if (z)
      return 0b11;
    NOT_IMPLEMENTED();
    return 0;
  }
  uint8 i() const { return x + y + z + 15; }
};

struct ND : public Coordinate {
  ND(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK_GT(mLen(), 0);
    DCHECK_LE(mLen(), 2);
    DCHECK_EQ(1, cLen());
  }

  uint8 nd() const { return (x + 1) * 9 + (y + 1) * 3 + (z + 1); }
};

struct FD : public Coordinate {
  FD(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK_GT(cLen(), 0);
    DCHECK_LE(cLen(), 30);
  }
};
