#include "geo.h"

#include "base.h"

bool IsCrossed(const Segment& s0, const Segment& s1) {
  auto f = [](const Segment& s, const Point& p) {
    return (s.second.y - s.first.y) * (p.x - s.first.x) +
           (s.second.x - s.first.x) * (p.y - s.first.y);
  };

  auto c0 = f(s0, s1.first);
  auto c1 = f(s0, s1.second);
  if (c0 == 0 || c1 == 0)
    return false;
  if ((c0 > 0) == (c1 > 0))
    return false;

  auto c2 = f(s1, s0.first);
  auto c3 = f(s1, s0.second);
  if (c2 == 0 || c3 == 0)
    return false;
  return (c2 > 0) != (c3 > 0);
}

bool IsCrossed(const DSegment& s0, const Segment& s1) {
  auto f = [](const DSegment& s, const Point& p) {
    return (s.second.y - s.first.y) * (p.x - s.first.x) +
           (s.second.x - s.first.x) * (p.y - s.first.y);
  };
  auto g = [](const Segment& s, const DPoint& p) {
    return (s.second.y - s.first.y) * (p.x - s.first.x) +
           (s.second.x - s.first.x) * (p.y - s.first.y);
  };

  auto c0 = f(s0, s1.first);
  auto c1 = f(s0, s1.second);
  if (std::abs(c0) == 0 || std::abs(c1) == 0)
    return false;
  if ((c0 > 0) == (c1 > 0))
    return false;

  auto c2 = g(s1, s0.first);
  auto c3 = g(s1, s0.second);
  if (std::abs(c2) == 0 || std::abs(c3) == 0)
    return false;
  return (c2 > 0) != (c3 > 0);
}
