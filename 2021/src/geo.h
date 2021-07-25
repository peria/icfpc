#pragma once

#include "base.h"

template <typename T>
struct PointTrait {
  using P = PointTrait<T>;
  T x;
  T y;

  P operator-(const P& rhs) const { return {x - rhs.x, y - rhs.y}; }
};
using Point = PointTrait<Integer>;
using DPoint = PointTrait<double>;

template <typename T>
using SegmentTrait = std::pair<const PointTrait<T>&, const PointTrait<T>&>;
using Segment = SegmentTrait<Integer>;
using DSegment = SegmentTrait<double>;

template <typename T>
T GetD2(const PointTrait<T>& u, const PointTrait<T>& v) {
  PointTrait<T> d = u - v;
  return d.x * d.x + d.y * d.y;
}

template <typename T>
T GetD2(const SegmentTrait<T>& s) {
  return GetD2(s.first, s.second);
}

bool IsCrossed(const Segment& s0, const Segment& s1);
bool IsCrossed(const DSegment& s0, const Segment& s1);