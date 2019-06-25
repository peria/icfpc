#include "map.h"

#include <vector>

namespace {

char cellToChar(int cell) {
  if (cell & CellType::kObstacle)
    return '#';
  if (cell & CellType::kNonWrapped) {
    return '.';
  }
  return ' ';
}

}  // namespace

Map::Map(const int width, const int height, CellType value)
    : kWidth(width), kHeight(height), data_(width * height, value) {}

std::ostream& operator<<(std::ostream& os, const Map& map) {
  for (int y = map.kHeight - 1; y >= 0; --y) {
    for (int x = 0; x < map.kWidth; ++x) {
      os << cellToChar(map(x, y));
    }
    os << "\n";
  }
  return os;
}
