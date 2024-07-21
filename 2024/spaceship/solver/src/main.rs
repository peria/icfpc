mod problem;
mod solver;

use problem::Problem;
use solver::Solver;
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    let problem = Problem::read_stdin();
    let mut solver = Solver::new(problem);
    let command = if args.len() > 1 {
        format!("solve spaceship{} ", &args[1])
    } else {
        "".to_string()
    };
    print!("{}{}", &command, &solver.solve());
}
