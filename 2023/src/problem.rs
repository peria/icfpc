use crate::base::{Placement, Rect};
use serde::{Deserialize, Serialize};

pub struct Attendee {
    pub placement: Placement,
    pub tastes: Vec<f64>,
}

#[derive(Serialize, Deserialize)]
struct JsonAttendee {
    x: f64,
    y: f64,
    tastes: Vec<f64>,
}

pub struct Pillar {
    pub center: Placement,
    pub radius: f64,
}

#[derive(Serialize, Deserialize)]
struct JsonPillar {
    center: [f64; 2],
    radius: f64,
}

pub struct Problem {
    pub problem_id: usize,
    pub room: Rect,
    pub stage: Rect,
    pub instruments: Vec<usize>,
    pub attendees: Vec<Attendee>,
    pub pillars: Vec<Pillar>,

    pub is_full_div: bool,
    pub num_musicians: usize,
    pub num_attendees: usize,
}

#[derive(Serialize, Deserialize)]
struct JsonProblem {
    room_width: f64,
    room_height: f64,
    stage_width: f64,
    stage_height: f64,
    stage_bottom_left: [f64; 2],
    musicians: Vec<usize>,
    attendees: Vec<JsonAttendee>,
    pillars: Vec<JsonPillar>,
}

impl Problem {
    pub const EMPTY_RADIUS: f64 = 10.0;

    pub fn read_from_id(problem_id: usize) -> Problem {
        let filepath = format!("data/problem/problem-{}.json", problem_id);
        let mut problem = Problem::read_from_file(&filepath);
        problem.problem_id = problem_id;
        problem.is_full_div = problem_id > 55;
        problem
    }

    pub fn read_from_file(filepath: &str) -> Problem {
        let json = match std::fs::read_to_string(filepath) {
            Err(why) => panic!("Failed to load {}: {}", filepath, why),
            Ok(x) => x,
        };
        Self::from(json.as_str())
    }

    pub fn dump_statistics(&self) {
        eprintln!(
            "[{:2}] {:5} Mus, {:5} Att, {:3} Inst.",
            self.problem_id,
            self.num_musicians,
            self.num_attendees,
            self.instruments.iter().max().unwrap()
        );
    }
}

impl From<&str> for Problem {
    fn from(value: &str) -> Self {
        let input: JsonProblem = serde_json::from_str(value).unwrap();
        Problem::from(input)
    }
}

impl From<JsonProblem> for Problem {
    fn from(value: JsonProblem) -> Self {
        let attendees: Vec<Attendee> = value
            .attendees
            .into_iter()
            .map(|x| Attendee::from(x))
            .collect();

        Problem {
            problem_id: 0,
            is_full_div: false,
            room: Rect {
                top: value.room_height,
                bottom: 0.0,
                left: 0.0,
                right: value.room_width,
            },
            stage: Rect {
                left: value.stage_bottom_left[0],
                bottom: value.stage_bottom_left[1],
                top: value.stage_bottom_left[1] + value.stage_height,
                right: value.stage_bottom_left[0] + value.stage_width,
            },

            num_musicians: value.musicians.len(),
            instruments: value.musicians,
            num_attendees: attendees.len(),
            attendees,
            pillars: Vec::new(),
        }
    }
}

impl From<JsonAttendee> for Attendee {
    fn from(value: JsonAttendee) -> Self {
        Self {
            placement: Placement {
                x: value.x,
                y: value.y,
            },
            tastes: value.tastes,
        }
    }
}
