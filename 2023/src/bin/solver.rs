use rand::{distributions::Uniform, prelude::Distribution, SeedableRng};
use rand_chacha::ChaCha8Rng;
use std::{env, time::Instant};

use icfpc2023::{Musician, Point, Problem, Solution};

struct Solver {
    problem: Problem,
}

impl Solver {
    const RANDOM_SEED: [u8; 32] = [123; 32];

    fn solve(&self) -> Solution {
        let timer = Instant::now();

        let num_musicians = self.problem.num_musicians();
        // Use a random number generator with a fixed seed to check behavioral differences.
        let mut rng: ChaCha8Rng = SeedableRng::from_seed(Self::RANDOM_SEED);
        let stage = &self.problem.stage;
        let left = stage.left + Problem::EMPTY_RADIUS;
        let right = stage.right - Problem::EMPTY_RADIUS;
        let bottom = stage.bottom + Problem::EMPTY_RADIUS;
        let top = stage.top - Problem::EMPTY_RADIUS;
        let x_gen = Uniform::new_inclusive(left, right);
        let y_gen = Uniform::new_inclusive(bottom, top);
        let mut musicians = Vec::new();
        for _ in 0..num_musicians {
            loop {
                let x = x_gen.sample(&mut rng);
                let y = y_gen.sample(&mut rng);
                let p = Point::new(x, y);
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
            .zip(self.problem.instruments.iter())
            .map(|(x, &inst)| Musician {
                placement: x,
                volume: 1.0,
                instrument: inst,
                score: 0,
            })
            .collect();
        let mut solution = Solution::new(&self.problem);
        solution.musicians = musicians;

        solution.evaluate(&self.problem);
        solution.elapsed_time = timer.elapsed().as_secs_f64();
        eprintln!(
            "Score: {0}, Time: {1:.2} sec.",
            solution.score, solution.elapsed_time
        );

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
    solution.save_output();
    solution.save_as_json();
}

fn main() {
    let args: Vec<String> = env::args().collect();
    match args.iter().nth(1) {
        Some(x) => {
            let id = x.parse::<usize>().unwrap();
            solve(id);
        }
        None => {
            for id in 1..=90 {
                solve(id);
            }
        }
    };
}
