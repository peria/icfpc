use crate::problem::Problem;

struct State {
    cost: i32,
    coordinate: i32,
    velocity: i32,
}

pub struct Solver {
    problem: Problem,
}

impl Solver {
    pub fn new(problem: Problem) -> Self {
        Solver { problem }
    }

    pub fn solve(&mut self) -> String {
        if let Some(r) = self.visit_every_turn() {
            return r;
        }
        self.solve_sa()
    }

    fn visit_every_turn(&self) -> Option<String> {
        let n = self.problem.len();
        let mut visited = vec![false; n];
        return self.search_every_turn((0, 0), (0, 0), n, &mut visited);
    }

    fn search_every_turn(
        &self,
        (x, y): (i64, i64),
        (vx, vy): (i64, i64),
        rem: usize,
        visited: &mut Vec<bool>,
    ) -> Option<String> {
        const ACCELARATIONS: [(i64, i64); 9] = [
            (-1, -1),
            (0, -1),
            (1, -1),
            (-1, 0),
            (0, 0),
            (1, 0),
            (-1, 1),
            (0, 1),
            (1, 1),
        ];

        if rem == 0 {
            return Some("".to_string());
        }
        let n = self.problem.len();
        for (a, (ax, ay)) in ACCELARATIONS.iter().enumerate() {
            let (vx1, vy1) = (vx + *ax, vy + *ay);
            let (x1, y1) = (x + vx1, y + vy1);
            for i in 0..n {
                if visited[i] {
                    continue;
                }
                if x1 != self.problem.xs[i] || y1 != self.problem.ys[i] {
                    continue;
                }
                visited[i] = true;
                if let Some(r) = self.search_every_turn((x1, y1), (vx1, vy1), rem - 1, visited) {
                    return Some((a + 1).to_string() + &r);
                }
                visited[i] = false;
            }
        }
        None
    }

    const CORDINATE_OFFSET: i32 = 3;
    const VELOCITY_OFFSET: i32 = 5;
    fn solve_sa(&mut self) -> String {
        self.setup();
        // initialize visiting order
        let mut order = self.initialize_order();
        // evaluate
        "".to_string()
    }

    fn setup(&mut self) {}

    fn get_cost_1d(dx: i32, v0: i32, v1: i32) -> i32 {
        let mut t_low = 0;
        let mut t_high = dx * 2;
        while t_low + 1 < t_high {
            let t0 = (t_low + t_high) / 2;
            let t1 = v0 + t0 - v1;
            let x0 = v0 * t0 + t0 * (t0 + 1) / 2;
            let x1 = v1 * t1 + t1 * (t1 - 1) / 2;
            let x = x0 + x1;
            if x <= dx {
                t_low = t0;
            } else {
                t_high = t0;
            }
        }
        let t0 = t_high - 1;
        let t1 = v0 + t0 - v1;
        let x0 = v0 * t0 + t0 * (t0 + 1) / 2;
        let x1 = v1 * t1 + t1 * (t1 - 1) / 2;
        let x = x0 + x1;
        let mut add_cost = if x == dx {
            0
        } else if v0.min(v1) <= dx - x && dx - x <= v0 + t0 {
            1
        } else {
            2
        };
        t0 + t1 + add_cost
    }

    fn get_control_1d(dx: i32, v0: i32, v1: i32) -> Vec<i32> {}

    fn initialize_order(&self) -> Vec<usize> {
        let n = self.problem.len();
        let mut order = (0..n).collect();
        order
    }
}
