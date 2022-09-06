#pragma once

#include <vector>

#include "color.h"

// Named "Painting" in the spec.
class Image {
 public:
  Image(int w, int h, const std::vector<Color>& cs)
      : width(w), height(h), colors_(cs) {}

  const Color operator()(int x, int y) const { return colors_[y * width + x]; }
  Color& operator()(int x, int y) { return colors_[y * width + x]; }

  const int width;
  const int height;

 private:
  std::vector<Color> colors_;
};