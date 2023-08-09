use crate::{Musician, Point, Problem, Rect};
use core::panic;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Clone)]
pub struct Solution {
    pub problem_id: usize,
    // `musicians` may have some duplicated information with Problem.
    pub musicians: Vec<Musician>,
    pub stages: Vec<Vec<StagePart>>,
    pub elapsed_time: f64,
    pub num_iterations: usize,
    pub score: i64,
}

#[derive(Serialize, Deserialize)]
struct Output {
    placements: Vec<Point>,
    volumes: Vec<f64>,
}

impl Solution {
    pub fn new(value: &Problem) -> Solution {
        let musicians = value
            .instruments
            .iter()
            .map(|&x| Musician::new(x))
            .collect();
        let mut stages = Vec::new();
        for i in 0..value.instruments.iter().max().unwrap() + 1 {
            stages.push(vec![StagePart::new(value.stage, i, value)]);
        }

        Solution {
            problem_id: value.problem_id,
            musicians,
            stages,
            elapsed_time: 0.0,
            num_iterations: 0,
            score: 0,
        }
    }

    // Compute the final score, based on the spec.
    pub fn evaluate(&mut self, problem: &Problem) -> i64 {
        if self.problem_id != problem.problem_id {
            panic!(
                "Problem IDs are different {} - {}",
                self.problem_id, problem.problem_id
            );
        }

        let musicians = &mut self.musicians;
        let mut qs = vec![1.0; musicians.len()];
        // Extend 2: Playing Together
        if problem.is_full_div() {
            for (i, mi) in musicians.iter().enumerate() {
                let mut q = 0.0;
                for (j, mj) in musicians.iter().enumerate() {
                    if i == j || mi.instrument != mj.instrument {
                        continue;
                    }
                    let d = mi.placement.distance(&mj.placement);
                    q += 1.0 / d;
                }
                // Add 1.0 after summing 1/d to avoid underflow.
                qs[i] = 1.0 + q;
            }
        }

        let mut score = 0;
        let mut musician_scores = Vec::new();
        let mut volumes = Vec::new();
        for (i, mi) in musicians.iter().enumerate() {
            let mut musician_score = 0;
            let mut volume = 10.0;
            for attendee in problem.attendees.iter() {
                let impact = if musicians
                    .iter()
                    .enumerate()
                    .any(|(j, mj)| i != j && mj.blocks(&mi.placement, &attendee.placement))
                {
                    0.0
                } else {
                    let d2 = attendee.placement.distance2(&mi.placement);
                    (1_000_000.0 * attendee.tastes[mi.instrument] / d2).ceil()
                };
                let impact = if problem
                    .pillars
                    .iter()
                    .any(|p| p.blocks(&mi.placement, &attendee.placement))
                {
                    0.0
                } else {
                    impact
                };
                let pair_score = (volume * qs[i] * impact).ceil() as i64;
                musician_score += pair_score;
            }
            if musician_score < 0 {
                volume = 0.0;
                musician_score = 0;
            }
            volumes.push(volume);
            musician_scores.push(musician_score);
            score += musician_score;
        }
        self.score = score;

        musicians
            .iter_mut()
            .zip(musician_scores.iter())
            .zip(qs.iter())
            .zip(volumes.iter())
            .for_each(|(((mi, score), q), v)| {
                mi.score = *score;
                mi.q = *q;
                mi.volume = *v;
            });

        self.score
    }

    pub fn as_output(&self) -> String {
        let placements: Vec<Point> = self.musicians.iter().map(|x| x.placement).collect();
        let volumes: Vec<f64> = self.musicians.iter().map(|x| x.volume).collect();

        let output = Output {
            placements,
            volumes,
        };
        serde_json::to_string(&output).unwrap()
    }

    pub fn save_output(&self) {
        let output = self.as_output();
        let filepath = format!("data/output/output-{}.json", self.problem_id);
        if let Err(why) = std::fs::write(&filepath, &output) {
            panic!("Failed to save output to commit {}: {}", &filepath, why);
        }
    }

    pub fn save_as_json(&self) {
        let json = serde_json::to_string(self).unwrap();
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

impl From<&str> for Solution {
    fn from(value: &str) -> Self {
        serde_json::from_str(value).unwrap()
    }
}

#[derive(Serialize, Deserialize, Clone)]
pub struct StagePart {
    pub place: Rect,
    pub impact: i64,
}

impl StagePart {
    pub fn new(place: Rect, instrument: usize, problem: &Problem) -> Self {
        let mut impact = 0;
        for a in problem.attendees.iter() {
            let x = a.placement.x.max(place.left).min(place.right);
            let y = a.placement.y.max(place.bottom).min(place.top);
            let d2 = Point { x, y }.distance2(&a.placement);
            impact += (1_000_000.0 * a.tastes[instrument] / d2).ceil() as i64;
        }

        Self { place, impact }
    }

    pub fn size(&self) -> f64 {
        self.place.width().min(self.place.height())
    }

    pub fn break4(&self, instrument: usize, problem: &Problem) -> Vec<Self> {
        self.place
            .break4()
            .into_iter()
            .map(|p| Self::new(p, instrument, problem))
            .collect()
    }

    pub fn break2(&self, instrument: usize, problem: &Problem) -> Vec<Self> {
        self.place
            .break2()
            .into_iter()
            .map(|p| Self::new(p, instrument, problem))
            .collect()
    }
}

impl Ord for StagePart {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.impact.cmp(&other.impact)
    }
}

impl PartialOrd for StagePart {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Eq for StagePart {}

impl PartialEq for StagePart {
    fn eq(&self, other: &Self) -> bool {
        self.impact == other.impact
    }
}
