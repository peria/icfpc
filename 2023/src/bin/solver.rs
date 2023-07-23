use rand::{distributions::Distribution, distributions::Uniform, SeedableRng};
use rand_chacha::ChaCha8Rng;
use std::{env, time::Instant};

use icfpc2023::{Musician, Point, Problem, Solution};

pub struct Update {
    pub id: usize,
    pub new_place: Point,
    pub old_place: Point,
}

struct Solver {
    problem: Problem,
    generator: ChaCha8Rng,
    x_generator: Uniform<f64>,
    y_generator: Uniform<f64>,
}

impl Solver {
    const RANDOM_SEED: [u8; 32] = [123; 32];

    fn solve(&mut self) -> Solution {
        let timer = Instant::now();

        let mut solution = self.init_solution();
        let mut num_iteration = 0;
        loop {
            let score = solution.score;
            let updates = self.decide_update(&solution);
            self.apply_updates(&mut solution, &updates, false);
            solution.evaluate(&self.problem);
            let diff = solution.score - score;
            if diff < 0 {
                self.apply_updates(&mut solution, &updates, true);
                solution.evaluate(&self.problem);
            }
            eprintln!("Updated: {}", solution.score);

            num_iteration += 1;
            if num_iteration > 20 {
                break;
            }
        }
        solution.elapsed_time = timer.elapsed().as_secs_f64();
        eprintln!(
            "Score: {0}, Time: {1:.2} sec.",
            solution.score, solution.elapsed_time
        );
        solution
    }

    fn init_solution(&mut self) -> Solution {
        let mut solution = Solution::new(&self.problem);
        solution.musicians = self.place_randomly();
        solution.evaluate(&self.problem);
        solution
    }

    // TODO: Choose either one randomly
    // 1. Move one musician to a random position
    // self.move_musician_random(&mut solution, dry_run)
    // 2. Swap two musicians who have different instruments
    fn decide_update(&mut self, solution: &Solution) -> Vec<Update> {
        let mut updates = Vec::new();

        // TODO: Change behavor
        let id = self.generator.get_stream() as usize % self.problem.num_musicians();
        loop {
            let x = self.x_generator.sample(&mut self.generator);
            let y = self.y_generator.sample(&mut self.generator);
            let p = Point::new(x, y);
            if solution.musicians.iter().enumerate().all(|(i, m)| {
                i == id
                    || p.distance2(&m.placement) >= Problem::EMPTY_RADIUS * Problem::EMPTY_RADIUS
            }) {
                updates.push(Update {
                    id,
                    old_place: solution.musicians[id].placement,
                    new_place: Point::new(x, y),
                });
                return updates;
            }
        }
    }

    fn apply_updates(&self, solution: &mut Solution, updates: &Vec<Update>, revert: bool) {
        for update in updates.iter() {
            let id = update.id;
            solution.musicians[id].placement = if revert {
                update.old_place
            } else {
                update.new_place
            };
        }
    }

    fn place_randomly(&mut self) -> Vec<Musician> {
        let num_musicians = self.problem.num_musicians();
        let mut placements = Vec::new();
        for _ in 0..num_musicians {
            loop {
                let x = self.x_generator.sample(&mut self.generator);
                let y = self.y_generator.sample(&mut self.generator);
                let p = Point::new(x, y);
                if placements
                    .iter()
                    .all(|q| p.distance2(q) >= Problem::EMPTY_RADIUS * Problem::EMPTY_RADIUS)
                {
                    placements.push(p);
                    break;
                }
            }
        }

        let instruments = &self.problem.instruments;
        let mut qs = vec![1.0; num_musicians];
        if self.problem.is_full_div() {
            for (i, pi) in placements.iter().enumerate() {
                let mut q = 0.0;
                for (j, pj) in placements.iter().enumerate() {
                    if i == j || instruments[i] != instruments[j] {
                        continue;
                    }
                    let d = pi.distance(&pj);
                    q += 1.0 / d;
                }
                // Add 1.0 after summing 1/d to avoid underflow.
                qs[i] = 1.0 + q;
            }
        }

        placements
            .into_iter()
            .zip(self.problem.instruments.iter())
            .zip(qs.iter())
            .map(|((x, &inst), q)| Musician {
                placement: x,
                volume: 1.0,
                instrument: inst,
                q: *q,
                score: 0,
            })
            .collect()
    }
}

impl From<Problem> for Solver {
    fn from(value: Problem) -> Self {
        let stage = &value.stage;
        let left = stage.left + Problem::EMPTY_RADIUS;
        let right = stage.right - Problem::EMPTY_RADIUS;
        let bottom = stage.bottom + Problem::EMPTY_RADIUS;
        let top = stage.top - Problem::EMPTY_RADIUS;

        Solver {
            problem: value,
            generator: SeedableRng::from_seed(Self::RANDOM_SEED),
            x_generator: Uniform::new_inclusive(left, right),
            y_generator: Uniform::new_inclusive(bottom, top),
        }
    }
}

fn solve(problem_id: usize) {
    let problem = Problem::read_from_id(problem_id);
    problem.dump_statistics();
    let mut solver = Solver::from(problem);
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
