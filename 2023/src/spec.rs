use crate::Point;
use serde::{Deserialize, Serialize};

#[derive(Clone, Serialize, Deserialize)]
pub struct Attendee {
    pub placement: Point,
    pub tastes: Vec<f64>,
}

#[derive(Clone, Copy, Serialize, Deserialize)]
pub struct Pillar {
    pub center: Point,
    pub radius: f64,
}

#[derive(Clone, Copy, Serialize, Deserialize)]
pub struct Musician {
    pub placement: Point,
    pub volume: f64,
    pub q: f64,
    pub instrument: usize,
    pub score: i64,
}

impl Musician {
    pub fn new(instrument: usize) -> Musician {
        Musician {
            placement: Point { x: 0.0, y: 0.0 },
            volume: 1.0,
            q: 1.0,
            instrument,
            score: 0,
        }
    }

    pub fn blocks(&self, p1: &Point, p2: &Point) -> bool {
        const BLOCK_RADIUS: f64 = 5.0;
        // https://tjkendev.github.io/procon-library/python/geometry/circle_line_cross_point.html
        let dx = p2.x - p1.x;
        let dy = p2.y - p1.y;
        let x = p1.x - self.placement.x;
        let y = p1.y - self.placement.y;
        let a = dx * dx + dy * dy;
        let b = dx * x + dy * y;
        let c = x * x + y * y - BLOCK_RADIUS * BLOCK_RADIUS;
        let det = b * b - a * c;
        if det <= 0.0 {
            return false;
        }

        let f0 = c;
        let f1 = a + 2.0 * b + c;
        f0 * f1 <= 0.0 || (-a <= b && b <= 0.0)
    }
}
