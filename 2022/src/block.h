#pragma once

#include <optional>
#include <string>

#include "color.h"
#include "memory.h"

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

 private:
  const std::string id_;
  int x0_;
  int y0_;
  int x1_;
  int y1_;
  std::optional<Color> color_;
  // std::optional<int> sx_;
  // std::optional<int> sy_;
};