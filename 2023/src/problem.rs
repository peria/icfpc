use crate::{Attendee, Pillar, Point, Rect};
use serde::Deserialize;

pub struct Problem {
    pub problem_id: usize,
    pub room: Rect,
    pub stage: Rect,
    pub instruments: Vec<usize>, // named "musicians" in JSON
    pub attendees: Vec<Attendee>,
    pub pillars: Vec<Pillar>,
}

#[derive(Deserialize)]
struct JsonProblem {
    room_width: f64,
    room_height: f64,
    stage_width: f64,
    stage_height: f64,
    stage_bottom_left: [f64; 2],
    musicians: Vec<usize>,
    attendees: Vec<JsonAttendee>,
    pillars: Option<Vec<Pillar>>,
}

#[derive(Deserialize)]
struct JsonAttendee {
    x: f64,
    y: f64,
    tastes: Vec<f64>,
}

impl Problem {
    pub const EMPTY_RADIUS: f64 = 10.0;

    pub fn is_full_div(&self) -> bool {
        self.problem_id > 55
    }

    pub fn num_musicians(&self) -> usize {
        self.instruments.len()
    }

    pub fn num_attendees(&self) -> usize {
        self.attendees.len()
    }

    pub fn read_from_id(problem_id: usize) -> Problem {
        let filepath = format!("data/problem/problem-{}.json", problem_id);
        let mut problem = Problem::read_from_file(&filepath);
        problem.problem_id = problem_id;
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
            self.num_musicians(),
            self.num_attendees(),
            self.instruments.iter().max().unwrap() + 1
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

        let room = Rect {
            top: value.room_height,
            bottom: 0.0,
            left: 0.0,
            right: value.room_width,
        };
        let stage = Rect {
            left: value.stage_bottom_left[0],
            bottom: value.stage_bottom_left[1],
            top: value.stage_bottom_left[1] + value.stage_height,
            right: value.stage_bottom_left[0] + value.stage_width,
        };
        let pillars = match value.pillars {
            None => Vec::new(),
            Some(x) => x,
        };

        Problem {
            problem_id: 0,
            room,
            stage,
            instruments: value.musicians,
            attendees,
            pillars,
        }
    }
}

impl From<JsonAttendee> for Attendee {
    fn from(value: JsonAttendee) -> Self {
        Attendee {
            placement: Point {
                x: value.x,
                y: value.y,
            },
            tastes: value.tastes,
        }
    }
}
