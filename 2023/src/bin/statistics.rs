use icfpc2023::{Problem, Solution};

struct Statistics {
    problem: Problem,
    solution: Solution,
}

impl Statistics {
    fn draw_image(&self) {
        let problem = &self.problem;
        let problem_id = self.problem.problem_id;
        let path = format!("data/image/solution-{}.png", problem_id);
        // TODO: Define Image class to draw an image using f64 coordinates.
    }
}

fn dump_statistics(problem_id: usize) {
    let problem = Problem::read_from_id(problem_id);
    let solution = Solution::read_from_id(problem_id);
    let statistics = Statistics { problem, solution };

    statistics.draw_image();
}

fn main() {
    for id in 1..=90 {
        dump_statistics(id);
    }
}
