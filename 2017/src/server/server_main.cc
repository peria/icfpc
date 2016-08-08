#include <iostream>

#include "server.h"
#include "offline_server.h"

// Simulate offline mode server.
// Usage:
//   $ ./server --map=map.json ai_1 ai_2 ...
int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::fprintf(stderr, R"(Usage:
 $ ./%s map.json ai1.exe ai2.exe ...
)", argv[0]);
    return 0;
  }

  std::unique_ptr<Server> server;
  server.reset(new OfflineServer(argc, argv));

  server->run();

  return 0;
}
