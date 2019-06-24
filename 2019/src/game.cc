#include "game.h"

#include <string>

Game::Game(const std::string& desc_file, const std::string& buy_file)
  : map(desc_file) {
}
