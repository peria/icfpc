#pragma once

#include <memory>
#include <vector>

#include "color.h"

// Named "Painting" in the spec.
class Image {
 public:
  static int ComputeSimilarity(std::shared_ptr<Image> a,
                               std::shared_ptr<Image> b);

  Image(int w, int h, const std::vector<Color>& cs)
      : width(w), height(h), colors_(cs) {}
  Image(int w, int h) : width(w), height(h), colors_(w * h) {}

  const Color operator()(int x, int y) const { return colors_[y * width + x]; }
  Color& operator()(int x, int y) { return colors_[y * width + x]; }

  const std::vector<Color>& getSerialized() const { return colors_; }

  const int width;
  const int height;

 private:
  std::vector<Color> colors_;
};