use um::UniversalMachine;

mod um;

const DEFAULT_FILENAME: &str = "../sandmark.umz";

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = match args.get(1) {
        Some(f) => f as &str,
        None => DEFAULT_FILENAME,
    };
    let mut um = um::plain::Plain::new(filename);
    um.run();
    um.report();
}
