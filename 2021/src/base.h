#pragma once

#include <cstdint>

using int32 = std::int32_t;
using int64 = std::int64_t;
using Integer = std::int64_t;

template <typename T>
struct TPoint {
  using P = TPoint<T>;
  T x;
  T y;

  P operator-(const P& rhs) const { return {x - rhs.x, y - rhs.y}; }
};

using Point = TPoint<Integer>;
