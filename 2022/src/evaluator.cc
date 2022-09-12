#include "evaluator.h"

#include <cstdint>
#include <memory>
#include <vector>

#include <fmt/format.h>
#include <glog/logging.h>

#include "action.h"
#include "block.h"
#include "canvas.h"
#include "color.h"
#include "problem.h"

Evaluator::Result Evaluator::evaluate(
    std::shared_ptr<Problem> problem,
    const std::vector<std::shared_ptr<Action>>& actions) {
  auto canvas = problem->canvas();

  int next_id = 0;
  // Holds references to leaf blocks, and only they are actionable.
  std::map<std::string, std::shared_ptr<Block>> blocks;
  // Holds references to all blocks, even if it was destroyed by an action.
  std::map<std::string, std::shared_ptr<Block>> all_blocks;
  auto register_block = [&blocks, &all_blocks](std::shared_ptr<Block> block) {
    blocks[block->id()] = block;
    all_blocks[block->id()] = block;
  };
  auto unregister_block = [&blocks](std::shared_ptr<Block> block) {
    blocks.erase(block->id());
  };

  for (auto block : canvas->blocks()) {
    register_block(block);
    next_id =
        std::max<int>(next_id, std::strtoll(block->id().c_str(), nullptr, 10));
  }
  auto block = blocks.begin()->second;

  ++next_id;
  for (auto action : actions) {
    switch (action->type()) {
    case Action::Type::kColor: {
      auto act = std::dynamic_pointer_cast<ColorAction>(action);
      auto block = blocks[act->block_id];
      CHECK(block) << act->block_id << " is not found.";
      block->setColor(act->color);
      break;
    }
    case Action::Type::kHorizontalCut: {
      auto act = std::dynamic_pointer_cast<HorizontalCutAction>(action);
      auto block = blocks[act->block_id];
      CHECK(block) << act->block_id << " is not found.";
      CHECK_LE(block->y0(), act->y);
      CHECK_LT(act->y, block->y1());
      unregister_block(block);

      auto block0 = std::make_shared<Block>(
          fmt::format("{}.0", block->id()), block->x0(), block->y0(),
          block->x1(), act->y, block->color());
      auto block1 = std::make_shared<Block>(fmt::format("{}.1", block->id()),
                                            block->x0(), act->y, block->x1(),
                                            block->y1(), block->color());
      register_block(block0);
      register_block(block1);
      break;
    }
    case Action::Type::kVerticalCut: {
      auto act = std::dynamic_pointer_cast<VerticalCutAction>(action);
      auto block = blocks[act->block_id];
      CHECK(block) << act->block_id << " is not found.";
      CHECK_LE(block->x0(), act->x);
      CHECK_LT(act->x, block->x1());
      unregister_block(block);

      auto block0 = std::make_shared<Block>(fmt::format("{}.0", block->id()),
                                            block->x0(), block->y0(), act->x,
                                            block->y1(), block->color());
      auto block1 = std::make_shared<Block>(fmt::format("{}.1", block->id()),
                                            act->x, block->y0(), block->x1(),
                                            block->y1(), block->color());
      register_block(block0);
      register_block(block1);
      break;
    }
    case Action::Type::kPointCut: {
      auto act = std::dynamic_pointer_cast<PointCutAction>(action);
      auto block = blocks[act->block_id];
      CHECK(block) << act->block_id << " is not found.";
      CHECK_LE(block->y0(), act->y);
      CHECK_LT(act->y, block->y1());
      CHECK_LE(block->x0(), act->x);
      CHECK_LT(act->x, block->x1());
      unregister_block(block);

      auto block0 =
          std::make_shared<Block>(fmt::format("{}.0", block->id()), block->x0(),
                                  block->y0(), act->x, act->y, block->color());
      auto block1 = std::make_shared<Block>(fmt::format("{}.1", block->id()),
                                            act->x, block->y0(), block->x1(),
                                            act->y, block->color());
      auto block2 = std::make_shared<Block>(fmt::format("{}.2", block->id()),
                                            act->x, act->y, block->x1(),
                                            block->y1(), block->color());
      auto block3 =
          std::make_shared<Block>(fmt::format("{}.3", block->id()), block->x0(),
                                  act->y, act->x, block->y1(), block->color());
      register_block(block0);
      register_block(block1);
      register_block(block2);
      register_block(block3);
      break;
    }
    case Action::Type::kMerge: {
      auto act = std::dynamic_pointer_cast<MergeAction>(action);
      auto block0 = blocks[act->block_ids[0]];
      CHECK(block0) << act->block_ids[0] << " is not found.";
      auto block1 = blocks[act->block_ids[1]];
      CHECK(block1) << act->block_ids[1] << " is not found.";
      unregister_block(block0);
      unregister_block(block1);
      if (block0->x0() > block1->x0() || block0->y1() > block1->y1()) {
        std::swap(block0, block1);
      }
      CHECK((block0->x0() == block1->x0() && block0->x1() == block1->x1() &&
             block0->y1() == block1->y0()) ||
            (block0->y0() == block1->y0() && block0->y1() == block1->y1() &&
             block0->x1() == block1->x0()))
          << "Block " << block0->id() << " [" << block0->x0() << ", "
          << block0->y0() << ", " << block0->x1() << ", " << block0->y1()
          << "] and block " << block1->id() << " [" << block1->x0() << ", "
          << block1->y0() << ", " << block1->x1() << ", " << block1->y1()
          << "] can't be merged.";
      auto block =
          std::make_shared<Block>(fmt::format("{}", next_id++), block0->x0(),
                                  block0->y0(), block1->x1(), block1->y1());
      if (block0->color().has_value() && block1->color().has_value() &&
          block0->color().value() == block1->color().value()) {
        block->setColor(block0->color().value());
      } else {
        int width = block->x1() - block->x0();
        int height = block->y1() - block->y0();
        block->setImage(std::make_shared<Image>(width, height));
        for (int y = block0->y0(); y < block0->y1(); ++y) {
          for (int x = block0->x0(); x < block0->x1(); ++x) {
            (*block)(x, y) = (*block0)(x, y);
          }
        }
        for (int y = block1->y0(); y < block0->y1(); ++y) {
          for (int x = block1->x0(); x < block0->x1(); ++x) {
            (*block)(x, y) = (*block1)(x, y);
          }
        }
      }
      register_block(block);
      break;
    }
    case Action::Type::kSwap: {
      auto act = std::dynamic_pointer_cast<SwapAction>(action);
      auto block0 = blocks[act->block_ids[0]];
      CHECK(block0) << act->block_ids[0] << " is not found.";
      auto block1 = blocks[act->block_ids[1]];
      CHECK(block1) << act->block_ids[1] << " is not found.";
      Block::Swap(block0, block1);
      break;
    }
    case Action::Type::kComment:
      LOG(INFO) << "# "
                << std::dynamic_pointer_cast<CommentAction>(action)->comment;
      break;
    }
  }
  LOG(INFO) << "Processing is done.";

  Result result;
  std::int64_t action_cost = 0;
  double canvas_area = canvas->height() * canvas->width();
  for (auto action : actions) {
    int block_area = 0;
    switch (action->type()) {
    case Action::Type::kColor: {
      auto act = std::dynamic_pointer_cast<ColorAction>(action);
      block_area = all_blocks[act->block_id]->area();
      break;
    }
    case Action::Type::kHorizontalCut:
    case Action::Type::kVerticalCut:
    case Action::Type::kPointCut: {
      auto act = std::dynamic_pointer_cast<CutAction>(action);
      block_area = all_blocks[act->block_id]->area();
      break;
    }
    case Action::Type::kMerge: {
      auto act = std::dynamic_pointer_cast<MergeAction>(action);
      block_area = std::max(all_blocks[act->block_ids[0]]->area(),
                            all_blocks[act->block_ids[1]]->area());
      break;
    }
    case Action::Type::kSwap: {
      auto act = std::dynamic_pointer_cast<SwapAction>(action);
      block_area = all_blocks[act->block_ids[0]]->area();
      break;
    }
    case Action::Type::kComment:
      block_area = -1;
      continue;
    }
    if (block_area >= 0) {
      action_cost += std::round(action->baseCost() * canvas_area / block_area);
    }
  }
  result.action_cost = action_cost;

  // Merge leaf blocks' images into one.
  auto image = std::make_shared<Image>(canvas->width(), canvas->height());
  for (auto& [id, block] : blocks) {
    for (int y = block->y0(); y < block->y1(); ++y) {
      for (int x = block->x0(); x < block->x1(); ++x) {
        (*image)(x, y) = (*block)(x, y);
      }
    }
  }
  double similarity_cost = result.similarity_cost =
      Image::ComputeSimilarity(problem->target(), image);

  result.cost = result.action_cost + result.similarity_cost;

  int num_actions = 0;
  for (auto action : actions) {
    if (action->baseCost() > 0) {
      ++num_actions;
    }
  }
  result.num_actions = num_actions;

  return result;
}