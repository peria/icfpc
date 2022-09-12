#pragma once

#include <memory>
#include <vector>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "block.h"
#include "color.h"
#include "image.h"

class Canvas {
 public:
  static constexpr int kDefaultWidth = 400;
  static constexpr int kDefaultHeight = 400;

  Canvas(int w,
         int h,
         const std::vector<Color>& cs,
         const std::vector<std::shared_ptr<Block>>& blocks)
      : image_(std::make_shared<Image>(w, h, cs)), blocks_(blocks) {}

  const std::vector<std::shared_ptr<Block>>& blocks() const { return blocks_; }
  std::vector<std::shared_ptr<Block>>& blocks() { return blocks_; }

  int width() const { return image_->width; }
  int height() const { return image_->height; }
  const Color& operator()(int x, int y) const { return (*image_)(x, y); }
  Color& operator()(int x, int y) { return (*image_)(x, y); }

 private:
  std::shared_ptr<Image> image_;
  std::vector<std::shared_ptr<Block>> blocks_;
};