#include "map.h"

#include <cctype>
#include <vector>
#include <sstream>

namespace {

char cellToChar(int cell) {
  if (cell & CellType::kObstacle) {
    return '#';
  }

  char c = '.';
  if (cell & CellType::kManipulator) {
    c = 'B';
  } else if (cell & CellType::kFastWheel) {
    c = 'F';
  } else if (cell & CellType::kDrill) {
    c = 'L';
  } else if (cell & CellType::kSpawn) {
    c = 'X';
  } else if (cell & CellType::kCloning) {
    c = 'C';
  } else if (cell & CellType::kBeacon) {
    c = 'R';
  } else if (cell & CellType::kResetBeacon) {
    c = '+';
  }
  if (cell & CellType::kNonWrapped)
    return c;
  return std::tolower(c);
}

}  // namespace

Map::Map(const int width, const int height, CellType value)
    : kWidth(width), kHeight(height), data_(width * height, value) {}

std::string Map::toString() const {
  std::ostringstream oss;
  for (int y = kHeight - 1; y >= 0; --y) {
    for (int x = 0; x < kWidth; ++x) {
      oss << cellToChar(operator()(x, y));
    }
    oss << "\n";
  }
  return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Map& map) {
  os << map.toString();
  return os;
}
