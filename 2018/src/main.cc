#include "main.h"

#include <glog/logging.h>

#include <chrono>
#include <fstream>
#include <memory>

#include "matrix.h"
#include "solver.h"
#include "state.h"
// 3rd party
#include "cmdline.h"

namespace {

cmdline::parser ParseOption(int, char*[]);
using MS = std::chrono::milliseconds;
using Clock = std::chrono::system_clock;

}  // namespace

int main(int argc, char* argv[]) {
  cmdline::parser options = ParseOption(argc, argv);
  Solver::RegisterSolvers();

  Matrix src(options.get<std::string>("src"));
  Matrix tgt(options.get<std::string>("tgt"));

  if (!src.isValid() && tgt.isValid()) {
    src = Matrix(tgt.R);
  } else if (src.isValid() && !tgt.isValid()) {
    tgt = Matrix(src.R);
  }
  CHECK_EQ(src.R, tgt.R);

  std::unique_ptr<Solver> solver(
      Solver::GetSolver(options.get<std::string>("solver")));
  auto start_time = Clock::now();
  Trace trace = solver->solve(src, tgt);
  auto end_time = Clock::now();
  trace.dump(options.get<std::string>("trace"));

  time_t cpu_time =
      std::chrono::duration_cast<MS>(end_time - start_time).count();
  if (options.exist("label"))
    LOG(INFO) << options.get<std::string>("label") << " " << cpu_time << "ms";
  else
    LOG(INFO) << cpu_time << "ms";
  if (!trace.empty() && options.exist("info")) {
    State state(src);
    state.trace = std::move(trace);
    state.execute();

    std::ofstream ofs(options.get<std::string>("info"));
    ofs << "{"
        << "\"R\":" << src.R << ","
        << "\"solver\":\"" << solver->name() << "\","
        << "\"time\":" << state.time << ","
        << "\"energy\":" << state.energy << ","
        << "\"cpu_time\":" << cpu_time << ","
        << "\"timestamp\":" << Clock::to_time_t(end_time) << "}\n";
    ofs.close();
  }

  _exit(0);
}

namespace {

cmdline::parser ParseOption(int argc, char* argv[]) {
  cmdline::parser parser;

  parser.add<std::string>("solver", 0, "Name of the solver to use.", false, "");
  parser.add<std::string>("src", 's',
                          "Source model file. No need to specify for assembly.",
                          false, "");
  parser.add<std::string>(
      "tgt", 't', "Target model file. No need to specify for disassembly.",
      false, "");
  parser.add<std::string>("trace", 'n', "Filename to output trace.", false,
                          "trace.nbt");
  parser.add<std::string>("label", 'l', "Name of the problem.", false,
                          "unknown");
  parser.add<std::string>("info", 'i', "Output simulated result.", false, "");
  // parser.add("strace", 's', "Output trace as readable string.");

  parser.parse_check(argc, argv);

  return parser;
}

}  // namespace
