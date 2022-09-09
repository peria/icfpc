#include <gflags/gflags.h>
#include <glog/logging.h>

#include <filesystem>
#include <iostream>
#include <memory>

#include "empty_solver.h"
#include "problem.h"
#include "solver.h"

DEFINE_string(problems_dir,
              DEFAULT_PROBLEMS_DIR,
              "Path to the directory that contains problem files.");
DEFINE_string(solutions_dir,
              DEFAULT_SOLUTIONS_DIR,
              "Path to the directory that contains solution files.");

int main(int argc, char** argv) {
  google::InitGoogleLogging(*argv);
  google::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 2) {
    std::cerr << "Usage:\n" << argv[0] << " <problem id>\n";
    return 0;
  }

  std::filesystem::path problem_path =
      fmt::format("{}/{}.json", FLAGS_problems_dir, argv[1]);
  std::shared_ptr<Problem> problem = Problem::Load(problem_path);

  std::unique_ptr<Solver> solver(new EmptySolver);
  Solver::Args args;
  args.problem = problem;
  auto solution = solver->Solve(args);

  std::filesystem::path solution_path =
      fmt::format("{}/{}.isl", FLAGS_solutions_dir, argv[1]);
  solution.Save(solution_path);

  return 0;
}
