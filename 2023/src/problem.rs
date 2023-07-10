use crate::base::Placement;
use serde::{Deserialize, Serialize};

pub struct Attendee {
    pub placement: Placement,
    pub tastes: Vec<f64>,
}

pub struct Pillar {
    pub center: Placement,
    pub radius: f64,
}

pub struct Problem {
    pub problem_id: usize,
    pub room_size: Placement,
    pub stage_size: Placement,
    pub stage_bottom_left: Placement,
    pub instruments: Vec<usize>,
    pub attendees: Vec<Attendee>,
    pub pillars: Vec<Pillar>,
}

#[derive(Serialize, Deserialize)]
struct InputPillar {
    center: [f64; 2],
    radius: f64,
}

#[derive(Serialize, Deserialize)]
struct InputAttendee {
    x: f64,
    y: f64,
    tastes: Vec<f64>,
}

#[derive(Serialize, Deserialize)]
struct InputProblem {
    room_width: f64,
    room_height: f64,
    stage_width: f64,
    stage_height: f64,
    stage_bottom_left: [f64; 2],
    musicians: Vec<usize>,
    attendees: Vec<InputAttendee>,
    pillars: Vec<InputPillar>,
}

impl Problem {
    pub fn read(problem_id: usize, filepath: &str) -> Problem {
        let json = match std::fs::read_to_string(filepath) {
            Err(why) => panic!("Failed to load {}: {}", filepath, why),
            Ok(x) => x,
        };
        let mut problem = Self::from(json.as_str());
        problem.problem_id = problem_id;

        problem
    }
}

impl From<&str> for Problem {
    fn from(value: &str) -> Self {
        let input: InputProblem = serde_json::from_str(value).unwrap();
        Problem::from(input)
    }
}

impl From<InputProblem> for Problem {
    fn from(value: InputProblem) -> Self {
        let attendees: Vec<Attendee> = value
            .attendees
            .into_iter()
            .map(|x| Attendee::from(x))
            .collect();

        Problem {
            problem_id: 0,
            room_size: Placement {
                x: value.room_width,
                y: value.room_height,
            },
            stage_size: Placement {
                x: value.stage_width,
                y: value.stage_height,
            },
            stage_bottom_left: Placement {
                x: value.stage_bottom_left[0],
                y: value.stage_bottom_left[1],
            },
            instruments: value.musicians,
            attendees,
            pillars: Vec::new(),
        }
    }
}

impl From<InputAttendee> for Attendee {
    fn from(value: InputAttendee) -> Self {
        Self {
            placement: Placement {
                x: value.x,
                y: value.y,
            },
            tastes: value.tastes,
        }
    }
}
