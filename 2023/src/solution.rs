use crate::base::Placement;
use serde::{Deserialize, Serialize};

pub struct Solution {
    pub musicians: Vec<Musician>,
}

pub struct Musician {
    pub placement: Placement,
    pub volume: f64,
}

#[derive(Serialize, Deserialize)]
struct Output {
    // Output requirementsi in contest spec.
    placements: Vec<Placement>,
    volumes: Vec<f64>,

    // Additional information to track.
    problem_id: usize,
    elapsed_time: f64,
    score: usize,
}
