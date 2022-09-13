#include "block.h"

#include <glog/logging.h>

#include "color.h"

Block::Block(const std::string& id,
             int x0,
             int y0,
             int x1,
             int y1,
             std::shared_ptr<Block> block)
    : id_(id), x0_(x0), y0_(y0), x1_(x1), y1_(y1) {
  if (block->color().has_value()) {
    color_ = block->color().value();
    return;
  }
  image_ = std::make_shared<Image>(x1 - x0, y1 - y0);
  for (int y = y0; y < y1; ++y) {
    for (int x = x0; x < x1; ++x) {
      (*this)(x, y) = (*block)(x, y);
    }
  }
}

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
