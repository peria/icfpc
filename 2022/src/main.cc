#include <gflags/gflags.h>
#include <glog/logging.h>

#include <filesystem>
#include <iostream>
#include <memory>

#include "evaluator.h"
#include "problem.h"
#include "solver.h"

DEFINE_string(problems_dir,
              DEFAULT_PROBLEMS_DIR,
              "Path to the directory that contains problem files.");
DEFINE_string(solutions_dir,
              DEFAULT_SOLUTIONS_DIR,
              "Path to the directory that contains solution files.");

void CommandEval(const int argc, char** argv);
void CommandSolve(const int argc, char** argv);

int main(int argc, char** argv) {
  google::InitGoogleLogging(*argv);
  google::ParseCommandLineFlags(&argc, &argv, true);

  if (argc < 2) {
    std::cerr << "Usage:\n" << argv[0] << " <command> <other options>\n";
    return 0;
  }

  if (std::strcmp(argv[1], "eval") == 0) {
    CommandEval(argc, argv);
  } else if (std::strcmp(argv[1], "solve") == 0) {
    CommandSolve(argc, argv);
  }

  return 0;
}

void CommandEval(const int argc, char** argv) {
  if (argc < 4) {
    std::cerr << "Usage:\n" << argv[0] << " eval <problem id> <isl file>\n";
    return;
  }
  const char* problem_id = argv[2];
  const char* isl_filepath = argv[3];

  std::filesystem::path problem_path =
      fmt::format("{}/{}.json", FLAGS_problems_dir, problem_id);
  auto&& problem = Problem::Load(problem_path);
  auto&& actions = Action::LoadIsl(isl_filepath);
  auto&& result = Evaluator::evaluate(problem, actions);
  LOG(INFO) << "# of actions: " << result.num_actions;
  LOG(INFO) << "Action cost : " << result.action_cost;
  LOG(INFO) << "Similarity  : " << result.similarity_cost;
  LOG(INFO) << "Total cost  : " << result.cost;
}

void CommandSolve(const int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage:\n" << argv[0] << " solve <problem id>\n";
    return;
  }
  const char* problem_id = argv[2];

  std::filesystem::path problem_path =
      fmt::format("{}/{}.json", FLAGS_problems_dir, problem_id);
  std::shared_ptr<Problem> problem = Problem::Load(problem_path);

  std::vector<std::shared_ptr<Action>> previous_actions;
  Solution solution;
  for (auto&& name : {"EmptySolver"}) {
    Solver::Args args;
    args.problem = problem;
    auto&& solver = SolverRegistry::getSolver(name);
    solution = solver->Solve(args);
  }
  std::filesystem::path solution_path =
      fmt::format("{}/{}.isl", FLAGS_solutions_dir, problem_id);
  solution.Save(solution_path);
}
