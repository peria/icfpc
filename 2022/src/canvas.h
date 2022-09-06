#pragma once

#include <vector>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "block.h"
#include "color.h"
#include "image.h"
#include "memory.h"

class Canvas {
 public:
  static constexpr int kDefaultWidth = 400;
  static constexpr int kDefaultHeight = 400;

  Canvas(int w,
         int h,
         const std::vector<Color>& cs,
         const std::vector<std::shared_ptr<Block>>& blocks)
      : image_(std::make_shared<Image>(w, h, cs)), blocks_(blocks) {
    if (blocks.size() == 0) {
      blocks_.push_back(std::make_shared<Block>("0", 0, 0, w, h));
    }
  }

  const Color& operator()(int x, int y) const { return (*image_)(x, y); }
  Color& operator()(int x, int y) { return (*image_)(x, y); }
  const std::vector<std::shared_ptr<Block>>& blocks() const { return blocks_; }
  std::vector<std::shared_ptr<Block>>& blocks() { return blocks_; }

 private:
  std::shared_ptr<Image> image_;
  std::vector<std::shared_ptr<Block>> blocks_;
};