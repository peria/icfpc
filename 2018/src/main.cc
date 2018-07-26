#include "main.h"

#include <memory>

#include "matrix.h"
#include "state.h"
#include "solver.h"
// 3rd party
#include "cmdline.h"

cmdline::parser ParseOption(int, char*[]);

int main(int argc, char* argv[]) {
  cmdline::parser options = ParseOption(argc, argv);

  Matrix src(options.get<std::string>("src"));
  Matrix target(options.get<std::string>("target"));

  if (!src.isValid() && target.isValid()) {
    src = Matrix(target.R);
  } else if (src.isValid() && !target.isValid()) {
    target = Matrix(src.R);
  }
  // CHECK_EQ(src.R, target.R);

  std::unique_ptr<Solver> solver(Solver::GetSolver(options.get<std::string>("solver")));
  Trace trace = solver->solve(src, target);

  trace.dump(options.get<std::string>("trace"));


  return 0;
}

cmdline::parser ParseOption(int argc, char* argv[]) {
  cmdline::parser parser;
  parser.add<std::string>("solver", 0, "Name of the solver to use.", false, "");
  parser.add<std::string>("src", 's', "Source model file. No need to specify for assembly.",
                          false, "");
  parser.add<std::string>("target", 't', "Target model file. No need to specify for disassembly.",
                          false, "");
  parser.add<std::string>("trace", 'n', "Filename to output trace.", false, "trace.nbt");
  // parser.add("strace", 's', "Output trace as readable string.");
  // parser.add("info", 'i', "Output simulated result.");

  parser.parse_check(argc, argv);

  return parser;
}
