use icfpc2023::{Image, Problem, Solution};

struct Statistics {
    problem: Problem,
    solution: Solution,
}

impl Statistics {
    fn draw_image(&self) {
        let problem = &self.problem;
        let problem_id = self.problem.problem_id;
        let path = format!("data/image/solution-{}.png", problem_id);
        let mut image = Image::new(&problem.room);
        image.rectangle(&problem.stage, (128.0, 128.0, 0.0));
        for pillar in &problem.pillars {
            image.circle(&pillar.center, pillar.radius, (0.0, 255.0, 0.0))
        }
        for musician in &self.solution.musicians {
            image.circle(&musician.placement, 5.0, (255.0, 255.0, 255.0));
        }
        for attendee in &self.problem.attendees {
            image.circle(&attendee.placement, 2.0, (0.0, 255.0, 255.0));
        }
        image.save(&path);
    }
}

fn dump_statistics(problem_id: usize) {
    let statistics = Statistics {
        problem: Problem::read_from_id(problem_id),
        solution: Solution::read_from_id(problem_id),
    };
    let problem = &statistics.problem;
    let solution = &statistics.solution;

    statistics.draw_image();
    println!(
        "{:2}\t{:12}\t{:4}x{:4}\t{:4}x{:4}",
        problem.problem_id,
        solution.score,
        problem.room.width(),
        problem.room.height(),
        problem.stage.width(),
        problem.stage.height()
    );
}

fn main() {
    for id in 1..=90 {
        dump_statistics(id);
    }
}
