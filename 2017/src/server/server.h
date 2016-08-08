#pragma once

class Server {
 public:
  Server(int argc, const char* argv[]);

  virtual void run() = 0;
};
