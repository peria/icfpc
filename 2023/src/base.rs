use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Copy, Clone)]
pub struct Point {
    pub x: f64,
    pub y: f64,
}

impl Point {
    pub fn new(x: f64, y: f64) -> Self {
        Point { x, y }
    }

    pub fn distance2(&self, other: &Point) -> f64 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        dx * dx + dy * dy
    }

    pub fn distance(&self, other: &Point) -> f64 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        dx.hypot(dy)
    }
}

#[derive(Serialize, Deserialize, Clone, Copy)]
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

    pub fn break4(&self) -> Vec<Rect> {
        let midx = (self.left + self.right) * 0.5;
        let midy = (self.bottom + self.top) * 0.5;
        let places: Vec<Rect> = vec![
            Rect {
                left: self.left,
                right: midx,
                bottom: self.bottom,
                top: midy,
            },
            Rect {
                left: midx,
                right: self.right,
                bottom: self.bottom,
                top: midy,
            },
            Rect {
                left: self.left,
                right: midx,
                bottom: midy,
                top: self.top,
            },
            Rect {
                left: midx,
                right: self.right,
                bottom: midy,
                top: self.top,
            },
        ];
        places
    }

    pub fn break2(&self) -> Vec<Rect> {
        if self.width() > self.height() {
            let midx = (self.left + self.right) * 0.5;
            vec![
                Rect {
                    left: self.left,
                    right: midx,
                    bottom: self.bottom,
                    top: self.top,
                },
                Rect {
                    left: midx,
                    right: self.right,
                    bottom: self.bottom,
                    top: self.top,
                },
            ]
        } else {
            let midy = (self.bottom + self.top) * 0.5;
            vec![
                Rect {
                    left: self.left,
                    right: self.right,
                    bottom: self.bottom,
                    top: midy,
                },
                Rect {
                    left: self.left,
                    right: self.right,
                    bottom: midy,
                    top: self.top,
                },
            ]
        }
    }
}
