#include <gflags/gflags.h>
#include <glog/logging.h>

#include <filesystem>
#include <iostream>

#include "memory.h"
#include "problem.h"
#include "solver.h"

DEFINE_string(problems_dir,
              DEFAULT_PROBLEMS_DIR,
              "Path to the directory that contains problem files.");

int main(int argc, char** argv) {
  google::InitGoogleLogging(*argv);
  google::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 2) {
    std::cerr << "Usage:\n" << argv[0] << " <problem id>\n";
    return 0;
  }

  std::filesystem::path problem_path =
      fmt::format("{}/{}", FLAGS_problems_dir, argv[1]);
  std::shared_ptr<Problem> problem = Problem::Load(problem_path);
  std::unique_ptr<Solver> solver(new Solver);
  auto actions = solver->Solve(problem);

  for (auto&& action : actions) {
    std::cout << action->toString() << "\n";
  }

  return 0;
}
