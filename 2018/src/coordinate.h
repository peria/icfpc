#pragma once

#include <cmath>

#include "base.h"

struct Coordinate {
  Coordinate(int x, int y, int z) : x(x), y(y), z(z) {}
  Coordinate(const Coordinate&) = default;

  int mLen() const { return std::abs(x) + std::abs(y) + std::abs(z); }
  int cLen() const {
    return std::max(std::abs(x), std::max(std::abs(y), std::abs(z)));
  }

  bool isLinear() const {
    return ((x | y) == 0 || (y | z) == 0 || (z | x) == 0) && (x | y | z);
  }
  virtual bool isSLD() const { return isLinear() && cLen() <= 5; }
  virtual bool isLLD() const { return isLinear() && cLen() <= 15; }
  virtual bool isND() const { return 0 < mLen() && mLen() <= 2 && cLen() == 1; }
  virtual bool isFD() const { return 0 < cLen() && cLen() <= 30; }

  int hash() const { return (((x << 8) + y) << 8) + z; }

  // Bit encode
  uint8 a() const {
    DCHECK(isSLD() || isLLD());
    if (x)
      return 0b01;
    if (y)
      return 0b10;
    if (z)
      return 0b11;

    NOT_IMPLEMENTED();
    return 0;
  }
  uint8 i() const {
    if (isSLD())
      return x + y + z + 5;
    if (isLLD())
      return x + y + z + 15;

    NOT_IMPLEMENTED();
    return 0;
  }
  uint8 nd() const {
    DCHECK(isND());
    return (x + 1) * 9 + (y + 1) * 3 + (z + 1);
  }

  int x = 0;
  int y = 0;
  int z = 0;
};

struct SLD : public Coordinate {
  SLD(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK(Coordinate::isSLD());
  }
  bool isSLD() const override { return true; }
  bool isLLD() const override { return false; }
  bool isND() const override { return false; }
  bool isFD() const override { return false; }
};

struct LLD : public Coordinate {
  LLD(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK(Coordinate::isLLD());
  }
  bool isSLD() const override { return false; }
  bool isLLD() const override { return true; }
  bool isND() const override { return false; }
  bool isFD() const override { return false; }
};

struct ND : public Coordinate {
  ND(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK(Coordinate::isND());
  }
  bool isSLD() const override { return false; }
  bool isLLD() const override { return false; }
  bool isND() const override { return true; }
  bool isFD() const override { return false; }
};

struct FD : public Coordinate {
  FD(int x, int y, int z) : Coordinate(x, y, z) {
    DCHECK(Coordinate::isFD());
  }
  bool isSLD() const override { return false; }
  bool isLLD() const override { return false; }
  bool isND() const override { return false; }
  bool isFD() const override { return true; }
};
