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
    const TIME_THRESHOLD: f64 = 10.0;

    fn solve(&mut self) -> Solution {
        let timer = Instant::now();

        let mut solution = self.init_solution();
        let mut num_iterations = 0;
        loop {
            let updates = self.decide_update(&solution);
            let diff = self.apply_updates(&mut solution, &updates, false);
            if diff < 0 {
                self.apply_updates(&mut solution, &updates, true);
                solution.evaluate(&self.problem);
            } else {
                eprintln!("Updated: {}", solution.score);
            }
            num_iterations += 1;
            if timer.elapsed().as_secs_f64() > Self::TIME_THRESHOLD {
                break;
            }
            if num_iterations % 100 == 0 {
                solution.evaluate(&self.problem);
            }
        }
        solution.evaluate(&self.problem);

        solution.elapsed_time = timer.elapsed().as_secs_f64();
        solution.num_iterations = num_iterations;
        eprintln!(
            "Score: {0}, Time: {1:.2} sec. {2} iteration",
            solution.score, solution.elapsed_time, num_iterations
        );
        solution
    }

    fn init_solution(&mut self) -> Solution {
        let mut solution = Solution::new(&self.problem);
        solution.musicians = self.place_randomly();
        solution.evaluate(&self.problem);
        eprintln!("Initial solution score: {}", solution.score);
        solution
    }

    // TODO: Choose either one randomly
    // 1. Move one musician to a random position
    // self.move_musician_random(&mut solution, dry_run)
    // 2. Swap two musicians who have different instruments
    fn decide_update(&mut self, solution: &Solution) -> Vec<Update> {
        let mut updates = Vec::new();

        // TODO: Change behavor
        let mut id = 0;
        for i in 1..self.problem.num_musicians() {
            if solution.musicians[id].score > solution.musicians[i].score {
                id = i;
            }
        }

        let id = id;
        loop {
            let point = self.get_random_place();
            if solution.musicians.iter().enumerate().all(|(i, m)| {
                i == id
                    || point.distance2(&m.placement)
                        >= Problem::EMPTY_RADIUS * Problem::EMPTY_RADIUS
            }) {
                updates.push(Update {
                    id,
                    old_place: solution.musicians[id].placement,
                    new_place: point,
                });
                return updates;
            }
        }
    }

    fn apply_updates(&self, solution: &mut Solution, updates: &Vec<Update>, revert: bool) -> i64 {
        let attendees = &self.problem.attendees;
        let pillars = &self.problem.pillars;

        let mut diff = 0;
        for update in updates.iter() {
            let id = update.id;
            let musician = &solution.musicians[id];

            // Effect on removing the musician.
            diff -= musician.score;
            let mut diffs = Vec::new();
            for (j, mj) in solution.musicians.iter().enumerate() {
                if j == id {
                    continue;
                }
                for a in attendees.iter() {
                    if !musician.blocks(&mj.placement, &a.placement) {
                        continue;
                    }

                    let d = if pillars
                        .iter()
                        .any(|p| p.blocks(&mj.placement, &a.placement))
                        || solution.musicians.iter().enumerate().any(|(k, mk)| {
                            k != id && j != k && mk.blocks(&mj.placement, &a.placement)
                        }) {
                        0
                    } else {
                        let d2 = mj.placement.distance2(&a.placement);
                        (1_000_000.0 * a.tastes[mj.instrument] / d2).ceil() as i64
                    };
                    diffs.push((j, d));
                    diff += d;
                }
            }

            for (j, d) in diffs.iter() {
                solution.musicians[*j].score += d;
            }

            solution.musicians[id].placement = if revert {
                update.old_place
            } else {
                update.new_place
            };

            // Effect on putting the musician.
            let musician = &solution.musicians[id];
            for (j, mj) in solution.musicians.iter().enumerate() {
                if j == id {
                    continue;
                }
                for a in attendees.iter() {
                    if musician.blocks(&mj.placement, &a.placement) {
                        let d = if pillars
                            .iter()
                            .any(|p| p.blocks(&mj.placement, &a.placement))
                            || solution.musicians.iter().enumerate().any(|(k, mk)| {
                                k != id && j != k && mk.blocks(&mj.placement, &a.placement)
                            }) {
                            0
                        } else {
                            // Newly blocked by `musician`.
                            let d2 = mj.placement.distance2(&a.placement);
                            (1_000_000.0 * a.tastes[mj.instrument] / d2).ceil() as i64
                        };
                        diff -= d;
                    }
                }
            }
            let mut musician_score = 0;
            for a in attendees.iter() {
                let d = if solution
                    .musicians
                    .iter()
                    .enumerate()
                    .all(|(j, mj)| id == j || !mj.blocks(&musician.placement, &a.placement))
                    && pillars
                        .iter()
                        .all(|p| !p.blocks(&musician.placement, &a.placement))
                {
                    (1_000_000.0 * a.tastes[musician.instrument]
                        / musician.placement.distance2(&a.placement))
                    .ceil() as i64
                } else {
                    0
                };
                musician_score += d;
            }
            diff += musician_score;
            solution.musicians[id].score = musician_score;
        }
        diff
    }

    fn place_randomly(&mut self) -> Vec<Musician> {
        let num_musicians = self.problem.num_musicians();
        let mut placements = Vec::new();
        for _ in 0..num_musicians {
            loop {
                let p = self.get_random_place();
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

    fn get_random_place(&mut self) -> Point {
        let x = self.x_generator.sample(&mut self.generator);
        let y = self.y_generator.sample(&mut self.generator);
        Point::new(x, y)
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
