mod arrow;
mod fuun;

use std::{fs::File, io::Read};

fn main() {
    let mut dna_file = File::open("../data/endo.dna").unwrap();
    let mut dna = String::new();
    let _ = dna_file.read_to_string(&mut dna);
    // TODO: Prepend a prefix to DNA.
    let mut fuun = fuun::Fuun::new();
    fuun.execute(&mut dna);

    // let mut arrow = arrow::Arrow::new();
}
