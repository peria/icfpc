use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Clone, Copy, Debug)]
pub struct Placement {
    pub x: f64,
    pub y: f64,
}

impl Placement {
    pub fn new(x: f64, y: f64) -> Placement {
        Placement { x, y }
    }

    pub fn distance2(&self, other: &Placement) -> f64 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        dx * dx + dy * dy
    }

    pub fn distance(&self, other: &Placement) -> f64 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        dx.hypot(dy)
    }
}

#[derive(Clone, Copy)]
pub struct Rect {
    pub top: f64,
    pub bottom: f64,
    pub left: f64,
    pub right: f64,
}

impl Rect {
    pub fn width(&self) -> f64 {
        self.right - self.left
    }

    pub fn height(&self) -> f64 {
        self.top - self.bottom
    }
}

#[derive(Clone, Copy, Debug)]
pub struct Musician {
    pub placement: Placement,
    pub volume: f64,
}

impl Musician {
    pub fn new() -> Musician {
        Musician {
            placement: Placement { x: 0.0, y: 0.0 },
            volume: 1.0,
        }
    }

    pub fn blocks(&self, p1: &Placement, p2: &Placement) -> bool {
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
