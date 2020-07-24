#include "server.h"

#include <fstream>

#include <glog/logging.h>

namespace {

const char* kGalaxyTextPath = "../data/galaxy.txt";
const char* kWwwPath = "../www";

}

Server::Server() : galaxy_(kGalaxyTextPath), www_path_(kWwwPath) {
  server_.set_mount_point("/static/", (www_path_ / "static").c_str());
  server_.set_logger([](const auto& req, const auto& res) {
    if (res.status != 200)
      LOG(INFO) << req.path << " [" << res.status << "] : " << res.body;
  });

  server_.Get("/", [this](const Request& req, Response& res) { this->onLoadMain(req, res); });
}

void Server::runLoop() {
  LOG(INFO) << "open http://localhost:3333/";
  server_.listen("localhost", 3333);
}

void Server::onLoadMain(const Request& req, Response& res) {
  res.set_content(*readFile(www_path_ / "index.html"), "text/html");
}

std::optional<std::string> Server::readFile(std::filesystem::path file_path) {
  if (!std::filesystem::is_regular_file(file_path)) {
    return std::nullopt;
  }

  std::ifstream ifs(file_path.c_str(), std::ios::binary);
  std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  return content;
}

