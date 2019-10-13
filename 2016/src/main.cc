#include <fstream>
#include <iostream>
#include <string>

#include <gmpxx.h>

#include "problem.h"

#include "third_party/cmdline/cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser cmd;
  cmd.add<std::string>("problem", 'p', "problem file", true /*mandatory*/, "");
  cmd.parse_check(argc, argv);    

  Problem problem;
  problem.loadFile(cmd.get<std::string>("problem"));

  return 0;
}
