use icfpc2023::Problem;

fn get_statistics(problem_id: usize) {
    let problem = Problem::read_from_id(problem_id);
    problem.dump_statistics();
}

fn main() {
    for id in 1..=90 {
        get_statistics(id);
    }
}
