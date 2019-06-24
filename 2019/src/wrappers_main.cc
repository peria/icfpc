#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "solver.h"

// https://tanakh.hatenablog.com/entry/20091028
#include "cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser options;
  options.add<std::string>("desc", 'd', "Task file to solve. .desc", true);
  options.add<std::string>("sol", 's', "Solution file to output. .sol", false);
  options.add<std::string>("buy", 'b', "Additional boosters to use in. .buy", false);
  options.add("help", 'h', "Print help");

  if (!options.parse(argc, argv) || options.exist("help")) {
    std::cerr << options.error_full()
              << options.usage();
    return 0;
  }

  Solver solver(options.get<std::string>("desc"),
                options.get<std::string>("buy"));

  std::string commands = solver.solve();
  
  if (options.exist("sol")) {
    std::ofstream ofs(options.get<std::string>("sol"));
    ofs << commands;
  } else {
    std::cout << commands << "\n";
  }

  return 0;
}
