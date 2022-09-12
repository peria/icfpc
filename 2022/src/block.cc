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

// static
void Block::Swap(std::shared_ptr<Block> lhs, std::shared_ptr<Block> rhs) {
  std::swap(lhs->x0_, rhs->x0_);
  std::swap(lhs->x1_, rhs->x1_);
  std::swap(lhs->y0_, rhs->y0_);
  std::swap(lhs->y1_, rhs->y1_);
}
