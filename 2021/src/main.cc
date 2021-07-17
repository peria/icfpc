#include <gflags/gflags.h>
#include <glog/logging.h>

#include <iostream>
#include <memory>

#include "problem.h"
#include "solution.h"
#include "solver.h"
#include "solvers/zero_dislikes.h"

int main(int argc, char** argv) {
  google::InitGoogleLogging(*argv);
  google::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 3) {
    std::cerr << "Usage:\n" << argv[0] << " problem.json solution.json\n";
    return 0;
  }

  Problem problem(argv[1]);
  std::unique_ptr<Solver> solver(new ZeroDislikes(problem));
  auto solution = solver->solve();
  if (solution)
    solution->saveFile(argv[2]);

  return 0;
}