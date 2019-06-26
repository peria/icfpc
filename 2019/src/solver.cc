#include "solver.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "game.h"
#include "parser.h"

Solver::Solver(Game& game) : game_(game) {}

std::string Solver::solve() {
  // Testing behaviors
  Wrapper& w = game_.wrappers[0];
  std::string commands(
      "DQWWWWWWWWDDDSSSSSSSSSSSASSSSASSDDDAAASSSDSSSSSSSSSQED"
      "DDWWWWWWWWWASSSDFSSSDDDWWDAWWWWDDWWWWWWAWWWEEDEEWWDSWA"
      "ASSWWWDWWWDSSSFDWWWDSSSSEQDWWWDSSSSDSAQDSSSESSSSS");
  for (const char* p = commands.data(); *p;) {
    ActionCommand a = parse<ActionCommand>(p);
    std::cout << game_ << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    w.takeAction(new ActionCommand(a));
  }
  std::cout << game_ << "\n";

  return "";
}
