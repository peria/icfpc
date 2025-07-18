mod arrow;
mod fuun;

use sha2::Digest;
use std::{fs::File, io::Read};

fn main() {
    // TODO: Use arguments.
    let prefix = String::from("IIPIFFCPICICIICPIICIPPPICIIC");
    let endo_path = "../data/endo.dna";

    let mut dna = prefix.clone();
    let endo = load_endo(&endo_path);
    dna.push_str(&endo);
    let mut fuun = fuun::Fuun::new();
    fuun.execute(&dna);

    let mut arrow = arrow::Arrow::new();
    let bitmap = arrow.build(&fuun.rna);
    let hash = sha2::Sha256::digest(prefix.as_bytes());
    save_bitmap(&bitmap, &format!("{:x}.png", hash));
}

fn load_endo(endo_path: &str) -> String {
    let mut endo_file = File::open(endo_path).unwrap();
    let mut endo = String::new();
    let _ = endo_file.read_to_string(&mut endo);
    endo
}

fn save_bitmap(bitmap: &[[((u8, u8, u8), u8); 600]; 600], filename: &str) {
    let mut img = image::RgbImage::new(600, 600);
    for (x, y, pixel) in img.enumerate_pixels_mut() {
        let rgb = &bitmap[y as usize][x as usize].0;
        *pixel = image::Rgb([rgb.0, rgb.1, rgb.2]);
    }
    let file_path = format!("./{}", filename);
    img.save(file_path).unwrap();
}
