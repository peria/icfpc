use crate::{
    base::{Musician, Placement},
    Problem,
};
use serde::{Deserialize, Serialize};

pub struct Solution<'a> {
    pub problem: &'a Problem,
    pub musicians: Vec<Musician>,

    pub elapsed_time: f64,
    pub score: i64,
}

impl<'a> Solution<'a> {
    pub fn from(value: &'a Problem) -> Self {
        Solution {
            musicians: vec![Musician::new(); value.num_musicians],
            problem: value,
            elapsed_time: 0.0,
            score: 0,
        }
    }

    // Compute the final score, based on the spec.
    pub fn evaluate(&mut self) -> i64 {
        let musicians = &self.musicians;
        let instruments = &self.problem.instruments;
        let num_musicians = self.problem.num_musicians;
        let mut qs = vec![1.0; self.problem.num_musicians];
        if self.problem.is_full_div {
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
        for attendee in self.problem.attendees.iter() {
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
            problem_id: self.problem.problem_id,
            elapsed_time: self.elapsed_time,
            score: self.score,
        };
        serde_json::to_string(&output).unwrap()
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
