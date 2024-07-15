mod ast_node;
mod evaluator;
mod translator;

fn main() {
    let message = std::io::read_to_string(std::io::stdin()).unwrap();
    let message = message.trim().to_string();
    // let message = communicator::communicate(message);
    let message = evaluator::evaluate(message);
    println!("{}", &message);
}
