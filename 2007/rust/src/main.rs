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

    let mut arrow = arrow::Arrow::new();
    let bitmap = arrow.build(&fuun.rna);
    let mut img = image::RgbImage::new(600, 600);
    for (x, y, pixel) in img.enumerate_pixels_mut() {
        let rgb = &bitmap[y as usize][x as usize].0;
        *pixel = image::Rgb([rgb.0, rgb.1, rgb.2]);
    }
    img.save("result.png").unwrap();
}
