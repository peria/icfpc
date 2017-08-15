#include "offline_server.h"

#include <fstream>
#include <cstdio>
#include <string>

#include "core/json.h"
#include "core/protocol.h"
#include "core/settings.h"
#include "core/game.h"
#include "server/client.h"

OfflineServer::OfflineServer(int argc, const char* argv[])
    : Server(argc, argv) {
  std::ifstream ifs(argv[1]);
  std::istreambuf_iterator<char> iter(ifs);
  std::istreambuf_iterator<char> last;
  std::string map_json_str(iter, last);
  map_.reset(new Map(Json::parse(map_json_str)));

  int num_punters = argc - 2;
  settings_.reset(new Settings());
  game_.reset(new Game(num_punters, *map_, *settings_));
  for (int i = 0; i < num_punters; ++i) {
    clients_.emplace_back(argv[i + 2], game_->punter(i), *game_);
  }
}

void OfflineServer::run() {
  const int num_rivers = game_->num_rivers();
  for (int i = 0; i < num_rivers; ++i) {
    int punter_id = game_->playing_punter();
    Client& client = clients_[punter_id];
    Json message = client.communicate();
    game_->move(message);
  }
  // report scoring to all clients
  Json scores = game_->getScores();
  for (int i = 0; i < game_->num_punters(); ++i) {
    clients_[i].reportScores(scores);
  }
}
