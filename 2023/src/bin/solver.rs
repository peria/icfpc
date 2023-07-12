use core::panic;
use rand::Rng;
use std::{env, time::Instant};

use icfpc2023::{Musician, Placement, Problem, Solution};

struct Solver {
    problem: Problem,
}

impl Solver {
    fn solve(&self) -> Solution {
        let timer = Instant::now();

        let num_musicians = self.problem.num_musicians;
        let mut rng = rand::thread_rng();
        let stage = &self.problem.stage;
        let left = stage.left + Problem::EMPTY_RADIUS;
        let right = stage.right - Problem::EMPTY_RADIUS;
        let bottom = stage.bottom + Problem::EMPTY_RADIUS;
        let top = stage.top - Problem::EMPTY_RADIUS;
        let mut musicians = Vec::new();
        for _ in 0..num_musicians {
            loop {
                let p = Placement::new(rng.gen_range(left..right), rng.gen_range(bottom..top));
                if musicians
                    .iter()
                    .all(|q| p.distance2(q) >= Problem::EMPTY_RADIUS * Problem::EMPTY_RADIUS)
                {
                    musicians.push(p);
                    break;
                }
            }
        }

        let musicians = musicians
            .into_iter()
            .map(|x| Musician {
                placement: x,
                volume: 1.0,
            })
            .collect();
        let mut solution = Solution::from(&self.problem);
        solution.musicians = musicians;

        solution.evaluate();
        solution.elapsed_time = timer.elapsed().as_secs_f64();

        solution
    }
}

impl From<Problem> for Solver {
    fn from(value: Problem) -> Self {
        Solver { problem: value }
    }
}

fn solve(problem_id: usize) {
    let problem = Problem::read_from_id(problem_id);
    problem.dump_statistics();
    let solver = Solver::from(problem);
    let solution = solver.solve();
    let json = solution.to_json();
    let filepath = format!("data/solution/solution-{}.json", problem_id);
    match std::fs::write(&filepath, &json) {
        Err(why) => panic!("Failed to save the solution {}: {}", &filepath, why),
        Ok(_) => (),
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    match args.iter().nth(1) {
        Some(x) => {
            let id = x.parse::<usize>().unwrap();
            solve(id);
        }
        None => {
            eprintln!("Usage: {} <problem_id>", &args[0]);
        }
    };
}
