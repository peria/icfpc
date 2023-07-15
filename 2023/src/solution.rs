use core::panic;

use crate::{
    base::{Musician, Placement},
    Problem,
};
use serde::{Deserialize, Serialize};

pub struct Solution {
    pub problem_id: usize,
    pub musicians: Vec<Musician>,

    pub elapsed_time: f64,
    pub score: i64,
}

impl Solution {
    // Compute the final score, based on the spec.
    pub fn evaluate(&mut self, problem: &Problem) -> i64 {
        if self.problem_id != problem.problem_id {
            panic!(
                "Problem IDs are different {} - {}",
                self.problem_id, problem.problem_id
            );
        }

        let musicians = &self.musicians;
        let instruments = &problem.instruments;
        let num_musicians = problem.num_musicians;
        let mut qs = vec![1.0; num_musicians];
        if problem.is_full_div {
            for i in 0..num_musicians {
                let mut q = 0.0;
                let mi = &musicians[i];
                for j in 0..num_musicians {
                    if i == j || instruments[i] != instruments[j] {
                        continue;
                    }
                    let mj = &musicians[j];
                    let d = mi.placement.distance(&mj.placement);
                    q += 1.0 / d;
                }
                // Add 1.0 after summing 1/d to avoid underflow.
                qs[i] = 1.0 + q;
            }
        }

        let mut score = 0;
        for attendee in problem.attendees.iter() {
            for i in 0..num_musicians {
                let mi = &musicians[i];
                let instrument = instruments[i];
                let impact = if (0..num_musicians)
                    .any(|j| i != j && musicians[j].blocks(&mi.placement, &attendee.placement))
                {
                    0.0
                } else {
                    let d2 = attendee.placement.distance2(&mi.placement);
                    (1_000_000.0 * attendee.tastes[instrument] / d2).ceil()
                };
                score += (mi.volume * qs[i] * impact).ceil() as i64;
            }
        }
        self.score = score;

        self.score
    }

    pub fn to_json(&self) -> String {
        let placements: Vec<Placement> = self.musicians.iter().map(|x| x.placement).collect();
        let volumes: Vec<f64> = self.musicians.iter().map(|x| x.volume).collect();

        let output = JsonSolution {
            placements,
            volumes,
            problem_id: self.problem_id,
            elapsed_time: self.elapsed_time,
            score: self.score,
        };
        serde_json::to_string(&output).unwrap()
    }

    pub fn save_as_json(&self) {
        let json = self.to_json();
        let filepath = format!("data/solution/solution-{}.json", self.problem_id);
        if let Err(why) = std::fs::write(&filepath, &json) {
            panic!("Failed to save the solution {}: {}", &filepath, why);
        }
    }

    pub fn read_from_id(problem_id: usize) -> Solution {
        let filepath = format!("data/solution/solution-{}.json", problem_id);
        let json = match std::fs::read_to_string(&filepath) {
            Err(why) => panic!("Failed to load {}: {}", filepath, why),
            Ok(x) => x,
        };
        Self::from(json.as_str())
    }
}

impl From<&Problem> for Solution {
    fn from(value: &Problem) -> Self {
        Solution {
            problem_id: value.problem_id,
            musicians: vec![Musician::new(); value.num_musicians],
            elapsed_time: 0.0,
            score: 0,
        }
    }
}

impl From<&str> for Solution {
    fn from(value: &str) -> Self {
        let input: JsonSolution = serde_json::from_str(value).unwrap();
        Solution::from(input)
    }
}

impl From<JsonSolution> for Solution {
    fn from(value: JsonSolution) -> Self {
        let musicians = value
            .placements
            .iter()
            .zip(value.volumes.iter())
            .map(|(p, v)| Musician {
                placement: *p,
                volume: *v,
            })
            .collect();

        Solution {
            problem_id: value.problem_id,
            musicians,
            elapsed_time: value.elapsed_time,
            score: value.score,
        }
    }
}

#[derive(Serialize, Deserialize)]
struct JsonSolution {
    // Output requirementsi in contest spec.
    placements: Vec<Placement>,
    volumes: Vec<f64>,

    // Additional information to track.
    problem_id: usize,
    elapsed_time: f64,
    score: i64,
}
