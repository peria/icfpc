use std::collections::VecDeque;

type Coord = usize;
type Pos = (Coord, Coord);
type Component = u8;
type RGB = (Component, Component, Component);
type Transparency = Component;
type Pixel = (RGB, Transparency);

type Bitmap = [[Pixel; 600]; 600];

#[derive(Clone, Copy)]
enum Color {
    RGB(RGB),
    Alpha(Transparency),
}
type Bucket = Vec<Color>;

#[derive(Clone, Copy)]
enum Dir {
    N,
    E,
    S,
    W,
}

const BLACK: RGB = (0, 0, 0);
const RED: RGB = (255, 0, 0);
const GREEN: RGB = (0, 255, 0);
const YELLOW: RGB = (255, 255, 0);
const BLUE: RGB = (0, 0, 255);
const MAGENTA: RGB = (255, 0, 255);
const CYAN: RGB = (0, 255, 255);
const WHITE: RGB = (255, 255, 255);
const TRANSPARENT: Transparency = 0;
const OPAQUE: Transparency = 255;

const TRANSPARENT_BITMAP: Bitmap = [[(BLACK, TRANSPARENT); 600]; 600];

pub struct Arrow {
    bucket: Bucket,
    position: Pos,
    mark: Pos,
    dir: Dir,
    bitmaps: VecDeque<Bitmap>,
}

impl Arrow {
    pub fn new() -> Self {
        Self {
            bucket: Vec::new(),
            position: (0, 0),
            mark: (0, 0),
            dir: Dir::E,
            bitmaps: VecDeque::from(vec![TRANSPARENT_BITMAP]),
        }
    }

    pub fn build(&mut self, rna: &Vec<String>) -> Bitmap {
        for r in rna.iter() {
            if r == "PIPIIIC" {
                self.add_color(Color::RGB(BLACK));
            } else if r == "PIPIIIP" {
                self.add_color(Color::RGB(RED));
            } else if r == "PIPIICC" {
                self.add_color(Color::RGB(GREEN));
            } else if r == "PIPIICF" {
                self.add_color(Color::RGB(YELLOW));
            } else if r == "PIPIICP" {
                self.add_color(Color::RGB(BLUE));
            } else if r == "PIPIIFC" {
                self.add_color(Color::RGB(MAGENTA));
            } else if r == "PIPIIFF" {
                self.add_color(Color::RGB(CYAN));
            } else if r == "PIPIIPC" {
                self.add_color(Color::RGB(WHITE));
            } else if r == "PIPIIPF" {
                self.add_color(Color::Alpha(TRANSPARENT));
            } else if r == "PIPIIPP" {
                self.add_color(Color::Alpha(OPAQUE));
            } else if r == "PIIPICP" {
                self.bucket.clear();
            } else if r == "PIIIIIP" {
                self.position = Self::do_move(self.position, self.dir);
            } else if r == "PCCCCCP" {
                self.dir = Self::turn_counter_clockwise(self.dir);
            } else if r == "PFFFFFP" {
                self.dir = Self::turn_clockwise(self.dir);
            } else if r == "PCCIFFP" {
                self.mark = self.position;
            } else if r == "PFFICCP" {
                self.line(self.position, self.mark);
            } else if r == "PIIPIIP" {
                self.tryfill();
            } else if r == "PCCPFFP" {
                self.add_bitmap(TRANSPARENT_BITMAP);
            } else if r == "PFFPCCP" {
                self.compose();
            } else if r == "PFFICCF" {
                self.clip();
            }
            // Do nothing for other `r`
        }

        self.bitmaps[0].clone()
    }

    fn add_color(&mut self, c: Color) {
        self.bucket.push(c);
    }

    fn current_pixel(&self) -> Pixel {
        let bucket = &self.bucket;
        let n = bucket.len();
        if n == 0 {
            return ((0, 0, 0), 255);
        }

        let rgbs: Vec<_> = bucket
            .iter()
            .filter_map(|c| match c {
                Color::RGB(rgb) => Some(*rgb),
                _ => None,
            })
            .collect();
        let r = if rgbs.is_empty() {
            0
        } else {
            rgbs.iter().map(|c| c.0 as u64).sum::<u64>() / rgbs.len() as u64
        };
        let g = if rgbs.is_empty() {
            0
        } else {
            rgbs.iter().map(|c| c.1 as u64).sum::<u64>() / rgbs.len() as u64
        };
        let b = if rgbs.is_empty() {
            0
        } else {
            rgbs.iter().map(|c| c.2 as u64).sum::<u64>() / rgbs.len() as u64
        };

        let alphas: Vec<_> = bucket
            .iter()
            .filter_map(|c| match c {
                Color::Alpha(alpha) => Some(*alpha as u64),
                _ => None,
            })
            .collect();
        let a = if alphas.is_empty() {
            255u64
        } else {
            alphas.iter().sum::<u64>() / alphas.len() as u64
        };

        let r = (r * a / 255) as u8;
        let g = (g * a / 255) as u8;
        let b = (b * a / 255) as u8;
        ((r, g, b), a as u8)
    }

    fn do_move((x, y): Pos, d: Dir) -> Pos {
        match d {
            Dir::N => (x, (y + 599) % 600),
            Dir::E => ((x + 1) % 600, y),
            Dir::S => (x, (y + 1) % 600),
            Dir::W => ((x + 599) % 600, y),
        }
    }

    fn turn_counter_clockwise(d: Dir) -> Dir {
        match d {
            Dir::N => Dir::W,
            Dir::E => Dir::N,
            Dir::S => Dir::E,
            Dir::W => Dir::S,
        }
    }

    fn turn_clockwise(d: Dir) -> Dir {
        match d {
            Dir::N => Dir::E,
            Dir::E => Dir::S,
            Dir::S => Dir::W,
            Dir::W => Dir::N,
        }
    }

    fn get_pixel(&self, (x, y): Pos) -> &Pixel {
        &self.bitmaps[0][y][x]
    }

    fn set_pixel(&mut self, (x, y): Pos) {
        self.bitmaps[0][y][x] = self.current_pixel();
    }

    fn line(&mut self, (x0, y0): Pos, (x1, y1): Pos) {
        let (x0, y0) = (x0 as i64, y0 as i64);
        let (x1, y1) = (x1 as i64, y1 as i64);
        let delta_x = x1 - x0;
        let delta_y = y1 - y0;
        let d = delta_x.abs().max(delta_y.abs());
        let c = if delta_x * delta_y <= 0 { 1 } else { 0 };
        let mut x = x0 * d + (d - c) / 2;
        let mut y = y0 * d + (d - c) / 2;
        for _ in 0..d {
            self.set_pixel(((x / d) as usize, (y / d) as usize));
            x += delta_x;
            y += delta_y;
        }
        self.set_pixel((x1 as usize, y1 as usize));
    }

    fn tryfill(&mut self) {
        let new = self.current_pixel();
        let old = self.get_pixel(self.position);
        if &new != old {
            self.fill(self.position, old.clone());
        }
    }

    fn fill(&mut self, (x, y): Pos, initial: Pixel) {
        let mut q = VecDeque::from(vec![(x, y)]);
        while let Some((x, y)) = q.pop_front() {
            if self.get_pixel((x, y)) != &initial {
                continue;
            }

            self.set_pixel((x, y));
            if x > 0 {
                q.push_back((x - 1, y));
            }
            if x < 599 {
                q.push_back((x + 1, y));
            }
            if y > 0 {
                q.push_back((x, y - 1));
            }
            if y < 599 {
                q.push_back((x, y + 1));
            }
        }
    }

    fn add_bitmap(&mut self, b: Bitmap) {
        if self.bitmaps.len() < 10 {
            self.bitmaps.push_front(b);
        }
    }

    fn compose(&mut self) {
        if self.bitmaps.len() < 2 {
            return;
        }

        let bitmap0 = self.bitmaps.pop_front().unwrap();
        let mut bitmap1 = self.bitmaps.pop_front().unwrap();
        for (l0, l1) in bitmap0.iter().zip(bitmap1.iter_mut()) {
            for (((r0, g0, b0), a0), ((r1, g1, b1), a1)) in l0.iter().zip(l1.iter_mut()) {
                *r1 = (*r0 as u64 + *r1 as u64 * (255 - *a0 as u64) / 255) as u8;
                *g1 = (*g0 as u64 + *g1 as u64 * (255 - *a0 as u64) / 255) as u8;
                *b1 = (*b0 as u64 + *b1 as u64 * (255 - *a0 as u64) / 255) as u8;
                *a1 = (*a0 as u64 + *a1 as u64 * (255 - *a0 as u64) / 255) as u8;
            }
        }
        self.bitmaps.push_front(bitmap1);
    }

    fn clip(&mut self) {
        if self.bitmaps.len() < 2 {
            return;
        }

        let bitmap0 = self.bitmaps.pop_front().unwrap();
        let mut bitmap1 = self.bitmaps.pop_front().unwrap();
        for (l0, l1) in bitmap0.iter().zip(bitmap1.iter_mut()) {
            for ((_, a0), ((r1, g1, b1), a1)) in l0.iter().zip(l1.iter_mut()) {
                let a0 = *a0 as u64;
                *r1 = (*r1 as u64 * a0 / 255) as u8;
                *g1 = (*g1 as u64 * a0 / 255) as u8;
                *b1 = (*b1 as u64 * a0 / 255) as u8;
                *a1 = (*a1 as u64 * a0 / 255) as u8;
            }
        }
        self.bitmaps.push_front(bitmap1);
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn current_pixel_test() {
        let mut arrow = Arrow::new();
        let b = Color::RGB(BLACK);
        let r = Color::RGB(RED);
        let m = Color::RGB(MAGENTA);
        let w = Color::RGB(WHITE);
        let y = Color::RGB(YELLOW);
        let c = Color::RGB(CYAN);
        let t = Color::Alpha(TRANSPARENT);
        let o = Color::Alpha(OPAQUE);

        for c in [t, o, o] {
            arrow.add_color(c);
        }
        let pixel = arrow.current_pixel();
        assert_eq!(((0, 0, 0), 170), pixel);

        arrow.bucket.clear();
        for c in [b, y, c] {
            arrow.add_color(c);
        }
        let pixel = arrow.current_pixel();
        assert_eq!(((85, 170, 85), 255), pixel);

        arrow.bucket.clear();
        for c in [y, t, o] {
            arrow.add_color(c);
        }
        let pixel = arrow.current_pixel();
        assert_eq!(((127, 127, 0), 127), pixel);

        arrow.bucket.clear();
        for c in [
            b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, r, r, r, r, r, r, r, m, m, m, m,
            m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m, m,
            m, m, m, m, m, m, w, w, w, w, w, w, w, w, w, w, o, o, o, t,
        ] {
            arrow.add_color(c);
        }
        let pixel = arrow.current_pixel();
        assert_eq!(((143, 25, 125), 191), pixel);
    }
}
