mod cbv;
mod compiler;

fn main() {
    let buffer = std::io::read_to_string(std::io::stdin()).unwrap();
    let message = buffer.trim().to_string();
    let response = cbv::communicate(message);
    let response = compiler::compile(&response);
    println!("{}", &response);
}
