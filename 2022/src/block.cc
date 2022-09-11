#include "block.h"

#include <glog/logging.h>

#include "color.h"

Color& Block::operator()(int x, int y) {
  if (color_.has_value()) {
    return color_.value();
  }

  CHECK(image_);
  CHECK_LE(x0(), x);
  CHECK_LT(x, x1());
  CHECK_LE(y0(), y);
  CHECK_LT(y, y1());
  return (*image_)(x - x0(), y - y0());
}
