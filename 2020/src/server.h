#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

#include "httplib.h"

#include "galaxy.h"

class Server {
  using Request = httplib::Request;
  using Response = httplib::Response;

 public:
  Server();
  void runLoop();

 private:
  void onLoadMain(const Request& req, Response& res);

  static std::optional<std::string> readFile(std::filesystem::path file_path);

  Galaxy galaxy_;
  const std::filesystem::path www_path_;
  httplib::Server server_;
};
