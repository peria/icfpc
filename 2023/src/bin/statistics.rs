use serde::{Deserialize, Serialize};

use icfpc2023::{Image, Problem, Solution};

struct Statistics {
    problem: Problem,
    solution: Solution,
}

#[derive(Deserialize, Serialize)]
struct JsonStatistics {
    problem_id: usize,
    image: String,
    score: i64,
    room_width: usize,
    room_height: usize,
    stage_width: usize,
    stage_height: usize,
}

impl Statistics {
    pub fn new(problem_id: usize) -> Statistics {
        let statistics = Statistics {
            problem: Problem::read_from_id(problem_id),
            solution: Solution::read_from_id(problem_id),
        };
        let problem = &statistics.problem;
        let solution = &statistics.solution;

        statistics.draw_image();
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

    fn draw_image(&self) {
        let problem = &self.problem;
        let problem_id = self.problem.problem_id;
        let path = format!("data/image/solution-{}.png", problem_id);
        let mut image = Image::new(&problem.room);
        image.rectangle(&problem.stage, (128.0, 128.0, 0.0));
        for pillar in &problem.pillars {
            image.circle(&pillar.center, pillar.radius, (0.0, 255.0, 0.0))
        }
        for musician in &self.solution.musicians {
            image.circle(&musician.placement, 5.0, (255.0, 255.0, 255.0));
        }
        for attendee in &self.problem.attendees {
            image.circle(&attendee.placement, 2.0, (0.0, 255.0, 255.0));
        }
        image.save(&path);
    }

    fn to_json(&self) -> JsonStatistics {
        JsonStatistics {
            problem_id: self.problem.problem_id,
            image: format!("data/image/solution-{}.png", self.problem.problem_id),
            score: self.solution.score,
            room_width: self.problem.room.width() as usize,
            room_height: self.problem.room.height() as usize,
            stage_width: self.problem.stage.width() as usize,
            stage_height: self.problem.stage.height() as usize,
        }
    }
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
