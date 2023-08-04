use rand::{
    distributions::{Distribution, Uniform},
    SeedableRng,
};
use rand_chacha::ChaCha8Rng;
use std::{env, time::Instant};

use icfpc2023::{Musician, Point, Problem, Rect, Solution, StagePart};

enum Update {
    Move(usize, Point, Point),
    Swap(usize, usize),
}

struct Solver {
    problem: Problem,
    parts: Vec<Vec<StagePart>>,
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
            let update = self.decide_update(&solution);
            let diff = self.apply_update(&mut solution, &update, false);
            if diff > 0 {
                solution.score += diff;
            }
            if diff < 0 {
                self.apply_update(&mut solution, &update, true);
            }

            num_iterations += 1;
            if timer.elapsed().as_secs_f64() > Self::TIME_THRESHOLD {
                break;
            }
            if num_iterations % 100 == 0 {
                solution.evaluate(&self.problem);
                eprintln!("Updated: {}", solution.score);
            }
        }
        solution.evaluate(&self.problem);

        for musician in solution.musicians.iter_mut() {
            musician.volume = if musician.score < 0 { 0.0 } else { 10.0 };
        }

        solution.evaluate(&self.problem);
        solution.elapsed_time = timer.elapsed().as_secs_f64();
        solution.num_iterations = num_iterations;
        solution.stages = self.parts.clone();
        eprintln!(
            "Score: {0}, Time: {1:.2} sec. {2} iteration.",
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

    fn decide_update(&mut self, solution: &Solution) -> Update {
        match self.generator.get_stream() % 10 {
            0..=8 => self.decide_move(solution),
            _ => self.decide_swap(solution),
        }
    }

    fn decide_move(&mut self, solution: &Solution) -> Update {
        // Move the least scored musician to a random point.
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
                let old_point = solution.musicians[id].placement.clone();
                return Update::Move(id, old_point, point);
            }
        }
    }

    fn decide_swap(&mut self, solution: &Solution) -> Update {
        let num_musicians = self.problem.num_musicians() as usize;
        loop {
            let id0 = self.generator.get_stream() as usize % num_musicians;
            let id1 = self.generator.get_stream() as usize % num_musicians;
            if solution.musicians[id0].instrument != solution.musicians[id1].instrument {
                return Update::Swap(id0, id1);
            }
        }
    }

    fn apply_update(&self, solution: &mut Solution, update: &Update, revert: bool) -> i64 {
        match update {
            Update::Move(id, old_place, new_place) => {
                let mut diff = 0;
                let id = *id;
                let diffs = self.remove_musican(solution, id);
                for (j, d) in diffs.iter() {
                    diff += d;
                    solution.musicians[*j].score += d;
                }

                solution.musicians[id].placement = if revert { *old_place } else { *new_place };

                let diffs = self.place_musician(solution, id);
                for (j, d) in diffs.iter() {
                    diff += d;
                    solution.musicians[*j].score += d;
                }
                diff
            }
            Update::Swap(id0, id1) => {
                let m0 = &solution.musicians[*id0];
                let m1 = &solution.musicians[*id1];
                let p0 = m0.placement.clone();
                let p1 = m1.placement.clone();
                let diff = -m0.score - m1.score;
                let mut score1 = 0;
                let mut score0 = 0;
                if m0.score != 0 {
                    for a in self.problem.attendees.iter() {
                        if solution
                            .musicians
                            .iter()
                            .enumerate()
                            .any(|(j, mj)| j != *id0 && j != *id1 && mj.blocks(&p0, &a.placement))
                        {
                            continue;
                        }
                        let d2 = p1.distance2(&a.placement);
                        score1 += (1_000_000.0 / d2).ceil() as i64;
                    }
                }
                if m1.score != 0 {
                    for a in self.problem.attendees.iter() {
                        if solution
                            .musicians
                            .iter()
                            .enumerate()
                            .any(|(j, mj)| j != *id0 && j != *id1 && mj.blocks(&p1, &a.placement))
                        {
                            continue;
                        }
                        let d2 = p1.distance2(&a.placement);
                        score0 += (1_000_000.0 / d2).ceil() as i64;
                    }
                }
                solution.musicians[*id0].placement = p1;
                solution.musicians[*id1].placement = p0;
                solution.musicians[*id0].score = score0;
                solution.musicians[*id1].score = score1;

                diff + score0 + score1
            }
        }
    }

    // Remove a musician and returns approx. score difference.
    // Approx. means that we ignore q[i] in this computation.
    fn remove_musican(&self, solution: &mut Solution, id: usize) -> Vec<(usize, i64)> {
        let attendees = &self.problem.attendees;
        let pillars = &self.problem.pillars;
        let musician = &solution.musicians[id];

        let mut diffs = Vec::new();
        diffs.push((id, -musician.score));
        for (j, mj) in solution.musicians.iter().enumerate() {
            if j == id {
                continue;
            }
            let mut diff = 0;
            for a in attendees.iter() {
                // If `musician` is the one who blocks the ray between `mj`
                // and `a`, the ray earns a new score.
                if !musician.blocks(&mj.placement, &a.placement) {
                    continue;
                }
                if pillars
                    .iter()
                    .any(|p| !p.blocks(&mj.placement, &a.placement))
                {
                    continue;
                }
                if solution
                    .musicians
                    .iter()
                    .enumerate()
                    .any(|(k, mk)| k != id || k != j || mk.blocks(&mj.placement, &a.placement))
                {
                    continue;
                }
                let d2 = mj.placement.distance2(&a.placement);
                diff += (1_000_000.0 * a.tastes[mj.instrument] / d2).ceil() as i64;
            }
            diffs.push((j, diff));
        }
        diffs
    }

    // Place a musician and returns approx. score difference.
    // Approx. means that we ignore q[i] in this computation.
    fn place_musician(&self, solution: &mut Solution, id: usize) -> Vec<(usize, i64)> {
        let pillars = &self.problem.pillars;
        let attendees = &self.problem.attendees;
        let musician = &solution.musicians[id];
        let mut diffs = Vec::new();

        // Look for pairs of a musican and an attendee that can be newly blocked
        // by `musician`.
        for (j, mj) in solution.musicians.iter().enumerate() {
            if j == id {
                continue;
            }
            let mut diff = 0;
            for a in attendees.iter() {
                if !musician.blocks(&mj.placement, &a.placement) {
                    continue;
                }
                if pillars
                    .iter()
                    .any(|p| p.blocks(&mj.placement, &a.placement))
                {
                    continue;
                }
                if solution
                    .musicians
                    .iter()
                    .enumerate()
                    .any(|(k, mk)| k != id && k != j && mk.blocks(&mj.placement, &a.placement))
                {
                    continue;
                }
                // Newly blocked by `musician`.
                let d2 = mj.placement.distance2(&a.placement);
                diff -= (1_000_000.0 * a.tastes[mj.instrument] / d2).ceil() as i64;
            }
            diffs.push((j, diff));
        }

        // Put musican and computes their earning score.
        let mut diff = 0;
        for a in attendees.iter() {
            if solution
                .musicians
                .iter()
                .enumerate()
                .any(|(j, mj)| j != id && mj.blocks(&musician.placement, &a.placement))
            {
                continue;
            }

            if pillars
                .iter()
                .any(|p| p.blocks(&musician.placement, &a.placement))
            {
                continue;
            }

            let d2 = musician.placement.distance2(&a.placement);
            diff += (1_000_000.0 * a.tastes[musician.instrument] / d2).ceil() as i64;
        }
        diffs.push((id, diff));

        diffs
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
    fn from(problem: Problem) -> Self {
        let stage = &problem.stage;
        let left = stage.left + Problem::EMPTY_RADIUS;
        let right = stage.right - Problem::EMPTY_RADIUS;
        let bottom = stage.bottom + Problem::EMPTY_RADIUS;
        let top = stage.top - Problem::EMPTY_RADIUS;

        let num_instruments = problem.instruments.iter().max().unwrap() + 1;
        let mut parts = Vec::new();
        for inst in 0..num_instruments {
            let mut partsv = Vec::new();
            let inner_stage = Rect {
                left: problem.stage.left + Problem::EMPTY_RADIUS,
                top: problem.stage.top - Problem::EMPTY_RADIUS,
                bottom: problem.stage.bottom + Problem::EMPTY_RADIUS,
                right: problem.stage.right - Problem::EMPTY_RADIUS,
            };
            let part = StagePart::new(inner_stage, inst, &problem);
            partsv.push(part);

            // TODO: implement here
            let mut loop_time = 0;
            loop {
                let mut index = partsv.len() - 1;
                while index > 0 && partsv[index].size() < Problem::EMPTY_RADIUS * 6.0 {
                    index -= 1;
                }
                if index == 0 && partsv[index].size() < Problem::EMPTY_RADIUS {
                    break;
                }
                let broken = partsv.remove(index);
                for piece in broken.break2(inst, &problem).into_iter() {
                    match partsv.binary_search(&piece) {
                        Ok(index) => partsv.insert(index, piece),
                        Err(index) => partsv.insert(index, piece),
                    }
                }
                loop_time += 1;
                if loop_time > 100 || partsv.len() > 60 {
                    break;
                }
            }
            parts.push(partsv);
        }

        Solver {
            problem,
            parts,
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
