#pragma once

#include <memory>
#include <optional>
#include <string>

#include "color.h"
#include "image.h"

class Block {
 public:
  Block(const std::string& id, int x0, int y0, int x1, int y1)
      : id_(id), x0_(x0), y0_(y0), x1_(x1), y1_(y1) {}
  Block(const std::string& id, int x0, int y0, int x1, int y1, Color color)
      : id_(id), x0_(x0), y0_(y0), x1_(x1), y1_(y1), color_(color) {}
  Block(const std::string& id,
        int x0,
        int y0,
        int x1,
        int y1,
        std::shared_ptr<Block> block);

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
    image_ = nullptr;
  }
  void setImage(std::shared_ptr<Image> img) {
    image_ = img;
    color_ = std::nullopt;
  }

  static void Swap(std::shared_ptr<Block> lhs, std::shared_ptr<Block> rhs);

 private:
  const std::string id_;
  int x0_;
  int y0_;
  int x1_;
  int y1_;
  std::optional<Color> color_;
  std::shared_ptr<Image> image_;
};