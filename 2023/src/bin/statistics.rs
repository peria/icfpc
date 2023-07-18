use serde::Serialize;

use icfpc2023::{Musician, Problem, Solution};

struct Statistics {
    problem: Problem,
    solution: Solution,
}

// JsonStatistics stores data to be displayed in HTML.
#[derive(Serialize)]
struct JsonStatistics {
    problem_id: usize,
    score: i64,
    elapsed_time: f64,
    room_width: usize,
    room_height: usize,
    stage_width: usize,
    stage_height: usize,
    num_musicians: usize,
    num_attendees: usize,
}

impl Statistics {
    pub fn new(problem_id: usize) -> Statistics {
        let statistics = Statistics {
            problem: Problem::read_from_id(problem_id),
            solution: Solution::read_from_id(problem_id),
        };
        let problem = &statistics.problem;
        let solution = &statistics.solution;

        println!(
            "{:2}\t{:12}\t{:4}x{:4}\t{:4}x{:4}",
            problem.problem_id,
            solution.score,
            problem.room.width(),
            problem.room.height(),
            problem.stage.width(),
            problem.stage.height()
        );

        statistics
    }

    fn to_json(&self) -> JsonStatistics {
        JsonStatistics {
            problem_id: self.problem.problem_id,
            score: self.solution.score,
            elapsed_time: self.solution.elapsed_time,
            room_width: self.problem.room.width() as usize,
            room_height: self.problem.room.height() as usize,
            stage_width: self.problem.stage.width() as usize,
            stage_height: self.problem.stage.height() as usize,
            num_musicians: self.problem.num_musicians,
            num_attendees: self.problem.num_attendees,
        }
    }
}

#[derive(Serialize)]
struct JsonMusician {
    musician: Musician,
    instrument: usize,
    score: i64,
}

fn dump_statistics() {
    let mut json = Vec::new();
    for id in 1..=90 {
        let statistics = Statistics::new(id);
        json.push(statistics.to_json());
    }
    let json = serde_json::to_string(&json).unwrap();
    let filepath = format!("data/solution/solution-data.json");
    if let Err(why) = std::fs::write(&filepath, &json) {
        panic!("Failed to save the solution {}: {}", &filepath, why);
    }
}

fn main() {
    dump_statistics();
}
