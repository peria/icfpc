#pragma once

#include <memory>
#include <vector>

#include "core/map.h"
#include "core/settings.h"
#include "core/game.h"
#include "server/server.h"
#include "server/client.h"

// Simulate offline mode server.
class OfflineServer : public Server {
 public:
  OfflineServer(int argc, const char* argv[]);

  void run() override;

 private:
  std::unique_ptr<Game> game_;
  std::unique_ptr<Map> map_;
  std::unique_ptr<Settings> settings_;
  std::vector<Client> clients_;
};
