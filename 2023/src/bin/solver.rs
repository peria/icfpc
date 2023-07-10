use std::env;

use icfpc2023::problem::Problem;

fn main() {
    let args: Vec<String> = env::args().collect();
    let problem_id = args[1].parse::<usize>().unwrap();
    let filepath = format!("data/problem/problem-{}.json", problem_id);
    let problem = Problem::read(problem_id, &filepath);
    println!(
        "num_musician: {}, num_attendee: {}",
        problem.instruments.len(),
        problem.attendees.len()
    );

    // initialize
    // arg -> id
    // load json -> Problem
    // solve it
    // output as a file
}
