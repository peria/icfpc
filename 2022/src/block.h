#pragma once

#include <memory>
#include <optional>
#include <string>

#include "color.h"
#include "image.h"

class Block {
 public:
  Block(const std::string& id,
        int x0,
        int y0,
        int x1,
        int y1,
        std::optional<Color> color = std::nullopt)
      : id_(id), x0_(x0), y0_(y0), x1_(x1), y1_(y1), color_(color) {}

  const std::string& id() const { return id_; }
  int x0() const { return x0_; }
  int x1() const { return x1_; }
  int y0() const { return y0_; }
  int y1() const { return y1_; }
  std::optional<Color> color() const { return color_; }
  Color& operator()(int x, int y);

  int area() const { return (y1_ - y0_) * (x1_ - x0_); }

  void setColor(std::optional<Color> c) {
    color_ = c;
    image_.reset();
  }
  void setImage(std::shared_ptr<Image> img) {
    image_ = img;
    color_ = std::nullopt;
  }

  void SwapWith(std::shared_ptr<Block> rhs) {
    std::swap(color_, rhs->color_);
    std::swap(image_, rhs->image_);
  }

 private:
  const std::string id_;
  const int x0_;
  const int y0_;
  const int x1_;
  const int y1_;
  std::optional<Color> color_;
  std::shared_ptr<Image> image_;
};