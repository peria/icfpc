#include "go_to_nearest_non_wrapped.h"

#include <unordered_map>
#include <queue>

#include "point.h"
#include "map.h"

namespace std {
template<>
class hash<Point> {
public:
  size_t operator()(const Point& p) const { return (p.x << 16) + p.y; }
};
}  // namespace std

namespace algorithm {
namespace {

// Directions. Upper 4 bits is 'to', lower 4 bits is 'from'.
using Direction = uint8_t;
std::unordered_map<Point, Direction> direction_map;

constexpr Direction kUp    = 1;
constexpr Direction kDown  = 2;
constexpr Direction kLeft  = 3;
constexpr Direction kRight = 4;
constexpr Direction kFromMask = 0xf;
constexpr Direction kToMask = 0xf0;

inline Direction setFrom(const Direction dir, const Direction from) {
  return (dir & kToMask) | from;
}
inline Direction getFrom(const Direction& dir) {
  return dir & kFromMask;
}
inline Direction setTo(const Direction dir, const Direction to) {
  return (dir & kFromMask) | (to << 4);
}
inline Direction getTo(const Direction dir) {
  return (dir >> 4) & kFromMask;
}

// Returns true if a non-wrapped cell is found.
bool searchNearestNonWrapped(const Point& init_pos, const Map& map) {
  direction_map.clear();
  
  std::queue<Point> q;
  q.push(init_pos);
  direction_map[init_pos] = 0;
  Point goal {-1, -1};
  while (!q.empty()) {
    Point from = q.front();
    q.pop();
    static const struct {
      Point dp;
      Direction from;
    } dps[] = {
      {{-1, 0}, kLeft},
      {{1, 0}, kRight},
      {{0, -1}, kDown},
      {{0, 1}, kUp},
    };
    for (auto& dp : dps) {
      Point p = from - dp.dp;
      if (!map.isInside(p))
        continue;
      // No assumption of drill.
      if (direction_map.count(p))
        continue;
      if (map(p) & CellType::kObstacle)
        continue;

      direction_map[p] = setFrom(0, dp.from);
      if (map(p) & CellType::kNonWrapped) {
        goal = p;
        break;
      }
      q.push(p);
    }
    if (goal != Point {-1, -1})
      break;
  }
  if (goal == Point {-1, -1})
    return false;

  // Set |to| direction
  for (Point p = goal; p != init_pos;) {
    switch (getFrom(direction_map[p])) {
    case kUp:    ++p.y; direction_map[p] = setTo(direction_map[p], kDown); break;
    case kDown:  --p.y; direction_map[p] = setTo(direction_map[p], kUp); break;
    case kLeft:  --p.x; direction_map[p] = setTo(direction_map[p], kRight); break;
    case kRight: ++p.x; direction_map[p] = setTo(direction_map[p], kLeft); break;
    default:
      assert(false);
    }
  }

  return true;
}

}  // namespace

bool goToNearestNonWrapped(Wrapper& w, Map& map) {
  if (!searchNearestNonWrapped(w.pos, map))
    return false;

  while (Direction dir = getTo(direction_map[w.pos])) {
    switch (dir) {
    case kUp:
      w.takeAction(new ActionCommand(Action::kMoveUp));
      break;
    case kDown:
      w.takeAction(new ActionCommand(Action::kMoveDown));
      break;
    case kLeft:
      w.takeAction(new ActionCommand(Action::kMoveLeft));
      break;
    case kRight:
      w.takeAction(new ActionCommand(Action::kMoveRight));
      break;
    }
  }

  return true;
}

}  // namespace algorithm

