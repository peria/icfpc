#include "server.h"

#include <fstream>

#include "base.h"

namespace {

const char* kGalaxyTextPath = "../data/galaxy.txt";
const char* kWwwPath = "../www";

}  // namespace

Server::Server() : galaxy_(kGalaxyTextPath), www_path_(kWwwPath) {
  server_.set_mount_point("/static/", (www_path_ / "static").c_str());
  server_.set_logger([](const auto& req, const auto& res) {
    if (res.status != 200)
      LOG(INFO) << req.path << " [" << res.status << "] : " << res.body;
  });

  using Req = const Request;
  using Res = Response;
  server_.Get("/", [this](Req& req, Res& res) { this->onLoadMain(req, res); });
  server_.Get("/click", [this](Req& req, Res& res) { this->onClickPicture(req, res); });
  server_.Post("/picture", [this](Req& req, Res& res) { this->onLoadPicture(req, res); });
}

void Server::runLoop() {
  LOG(INFO) << "open http://localhost:3333/";
  server_.listen("localhost", 3333);
}

void Server::onLoadMain(const Request& req, Response& res) {
  res.set_content(*readFile(www_path_ / "index.html"), "text/html");
}

void Server::onClickPicture(const Request& req, Response& res) {
  if (!req.has_param("x") || !req.has_param("y")) {
    res.status = 500;
    return;
  }

  const int64 x = std::stoll(req.get_param_value("x"));
  const int64 y = std::stoll(req.get_param_value("y"));
  LOG(INFO) << "Click (" << x << ", " << y << ")";

  std::string result = galaxy_.click(x, y);

  res.set_content(result, "application/json");
}

void Server::onLoadPicture(const Request& req, Response& res) {
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

