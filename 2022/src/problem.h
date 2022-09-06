#pragma once

#include <filesystem>
#include <string>

#include "canvas.h"
#include "image.h"
#include "memory.h"

class Problem {
 public:
  static std::shared_ptr<Problem> Load(const std::filesystem::path& filepath);
  Problem(std::shared_ptr<Canvas> canvas, std::shared_ptr<Image> target)
      : canvas_(canvas), target_(target) {}

  std::shared_ptr<Canvas> canvas() { return canvas_; }
  std::shared_ptr<Image> target() { return target_; }

 private:
  std::shared_ptr<Canvas> canvas_;
  std::shared_ptr<Image> target_;
};
