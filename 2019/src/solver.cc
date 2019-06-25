#include "solver.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "game.h"
#include "parser.h"

Solver::Solver(const std::string& desc, const std::string& buy)
    : game(desc, buy) {
  // Testing behaviors
  Wrapper& w = game.wrappers[0];
  std::string commands(
      "DQWWWWWWWWDDDSSSSSSSSSSSASSSSASSDDDAAASSSDSSSSSSSSSQED"
      "DDWWWWWWWWWASSSDFSSSDDDWWDAWWWWDDWWWWWWAWWWEEDEEWWDSWA"
      "ASSWWWDWWWDSSSFDWWWDSSSSEQDWWWDSSSSDSAQDSSSESSSSS");
  for (const char* p = commands.data(); *p;) {
    ActionCommand a = parse<ActionCommand>(p);
    std::cout << game << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    w.takeAction(ActionCommand(a));
  }
  std::cout << game << "\n";
}

std::string Solver::solve() {
  return "";
}
