#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "solver.h"

// https://tanakh.hatenablog.com/entry/20091028
#include "third_party/cmdline/cmdline.h"

std::string readFile(const std::string& filepath);

int main(int argc, char* argv[]) {
  cmdline::parser options;
  // Required
  options.add<std::string>("desc", 'd', "Task file to solve. .desc", true);
  options.add<std::string>("sol", 's', "Solution file to output. .sol", true);
  // Optional
  options.add<std::string>("buy", 'b', "Additional boosters to use. .buy",
                           false);
  options.add<std::string>(
      "buy-str", 0, "Additional boosters to use, in string directory.", false);
  options.add("help", 'h', "Print help");

  if (!options.parse(argc, argv) || options.exist("help")) {
    std::cerr << options.error_full() << options.usage();
    return 0;
  }

  std::string desc = readFile(options.get<std::string>("desc"));
  std::string buy;
  if (options.exist("buy")) {
    buy = readFile(options.get<std::string>("buy"));
  } else if (options.exist("buy-str")) {
    buy = options.get<std::string>("buy-str");
  }

  GameInitializer initializer(desc, buy);
  Game game(initializer);
  Solver solver(game);
  std::string commands = solver.solve();

  std::ofstream ofs(options.get<std::string>("sol"));
  ofs << commands;

  return 0;
}

std::string readFile(const std::string& filepath) {
  std::ifstream ifs(filepath);
  if (!ifs.is_open()) {
    std::cerr << "Cannot find " << filepath << "\n";
    return 0;
  }
  return std::string(std::istreambuf_iterator<char>(ifs),
                     std::istreambuf_iterator<char>());
}
