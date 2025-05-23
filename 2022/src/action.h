#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <glog/logging.h>

#include "block.h"

// Named "Move" in the spec.
struct Action {
  enum class Type {
    kHorizontalCut,
    kVerticalCut,
    kPointCut,
    kColor,
    kSwap,
    kMerge,

    kComment,  // Out of spec.
  };

  virtual bool isCut() const { return false; }
  virtual Type type() const = 0;
  virtual int baseCost() const = 0;
  virtual std::string toString() const = 0;

  virtual void setBlockId(const std::string& /*id*/) {
    CHECK(false) << "This action is not for one input block: (" << (int)type()
                 << ") [" << (int)(Type::kColor) << ": Color]";
  }

  static std::vector<std::shared_ptr<Action>> LoadIsl(
      const std::filesystem::path& file_path);
};

struct CutAction : public Action {
  CutAction(const std::string& id) : block_id(id) {}

  bool isCut() const final { return true; }
  void setBlockId(const std::string& id) final { block_id = id; }

  std::string block_id;
};

struct LineCutAction : public CutAction {
  static constexpr int kBaseCost = 7;
  LineCutAction(const std::string& id) : CutAction(id) {}

  int baseCost() const final { return kBaseCost; }
};

struct HorizontalCutAction final : public LineCutAction {
  HorizontalCutAction(const std::string& id, int y) : LineCutAction(id), y(y) {}

  Type type() const override { return Type::kHorizontalCut; }
  std::string toString() const override {
    return fmt::format("cut [{}] [Y] [{}]", block_id, y);
  }

  int y;
};

struct VerticalCutAction final : public LineCutAction {
  VerticalCutAction(const std::string& id, int x) : LineCutAction(id), x(x) {}

  Type type() const override { return Type::kVerticalCut; }
  std::string toString() const override {
    return fmt::format("cut [{}] [X] [{}]", block_id, x);
  }

  int x;
};

struct PointCutAction final : public CutAction {
  static constexpr int kBaseCost = 10;
  PointCutAction(const std::string& id, int x, int y)
      : CutAction(id), x(x), y(y) {}

  Type type() const override { return Type::kPointCut; }
  int baseCost() const override { return kBaseCost; }
  std::string toString() const override {
    return fmt::format("cut [{}] [{}, {}]", block_id, x, y);
  }

  int x;
  int y;
};

struct ColorAction final : public Action {
  static constexpr int kBaseCost = 5;
  ColorAction(const std::string& id, const Color& c) : block_id(id), color(c) {}

  Type type() const override { return Type::kColor; }
  int baseCost() const override { return kBaseCost; }
  std::string toString() const override {
    return fmt::format("color [{}] [{}, {}, {}, {}]", block_id, color.r(),
                       color.g(), color.b(), color.a());
  }
  void setBlockId(const std::string& id) final { block_id = id; }

  std::string block_id;
  Color color;
};

struct SwapAction final : public Action {
  static constexpr int kBaseCost = 3;
  SwapAction(const std::string& id0, const std::string& id1)
      : block_ids({id0, id1}) {}

  Type type() const override { return Type::kSwap; }
  int baseCost() const override { return kBaseCost; }
  std::string toString() const override {
    return fmt::format("swap [{}] [{}]", block_ids[0], block_ids[1]);
  }

  std::array<std::string, 2> block_ids;
};

struct MergeAction final : public Action {
  static constexpr int kBaseCost = 1;
  MergeAction(const std::string& id0, const std::string& id1)
      : block_ids({id0, id1}) {}

  Type type() const override { return Type::kMerge; }
  int baseCost() const override { return kBaseCost; }
  std::string toString() const override {
    return fmt::format("merge [{}] [{}]", block_ids[0], block_ids[1]);
  }

  std::array<std::string, 2> block_ids;
};

// Out of spec.
struct CommentAction final : public Action {
  static constexpr int kBaseCost = 0;
  CommentAction(const std::string& comment) : comment(comment) {}

  Type type() const override { return Type::kComment; }
  int baseCost() const override { return kBaseCost; }
  std::string toString() const override { return fmt::format("# {}", comment); }

  const std::string comment;
};
