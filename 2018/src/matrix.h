#pragma once

#include <string>
#include <vector>

#include "base.h"
#include "coordinate.h"

enum class Voxel : uint8 { kFull, kEmpty };

struct Matrix {
  Matrix(int r) : R(r), voxels_(r * r * r) {}
  Matrix(const std::string& filename);

  Voxel& operator()(int x, int y, int z) {
    return voxels_[(x * R + y) * R + z];
  }
  const Voxel& operator()(int x, int y, int z) const {
    return voxels_[(x * R + y) * R + z];
  }
  Voxel& operator()(const Coordinate& c) { return operator()(c.x, c.y, c.z); }
  const Voxel& operator()(const Coordinate& c) const {
    return operator()(c.x, c.y, c.z);
  }

  bool isValid() const { return R > 0; }
  bool isWellFormed() const;
  bool isGrounded(int x, int y, int z) const;
  bool isGrounded(const Coordinate& c) const {
    return isGrounded(c.x, c.y, c.z);
  }

  bool isInRange(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0)
      return false;
    if (R <= x || R <= y || R <= z)
      return false;
    return true;
  }
  bool isInRange(const Coordinate& c) const {
    return isInRange(c.x, c.y, c.z);
  }
  bool isAllGrounded() const;

  int R;

 private:
  std::vector<Voxel> voxels_;
};
