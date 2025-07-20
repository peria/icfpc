mod arrow;
mod fuun;

use sha2::Digest;
use std::{fs::File, io::Read};

const DEFAULT_ENDO_PATH: &str = "../data/endo.dna";

struct Args {
    args: Vec<String>,
    dump_rna: bool,
    dump_statistics: bool,
    endo: Option<String>,
}

fn main() {
    let args = parse_args();

    let mut prefix = String::new();
    if args.args.len() >= 2 {
        prefix = args.args[1].clone();
    }
    let endo_path = args.endo.unwrap_or(DEFAULT_ENDO_PATH.to_string());

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

fn parse_args() -> Args {
    let mut args = Vec::new();
    let mut dump_rna = false;
    let mut dump_statistics = false;
    let mut endo = None;

    let mut read_endo = false;
    for arg in std::env::args() {
        if read_endo {
            read_endo = false;
            endo = Some(arg);
            continue;
        }

        if arg == "-r" || arg == "--rna" {
            dump_rna = true;
        } else if arg == "-s" || arg == "--statistics" {
            dump_statistics = true;
        } else if arg == "-e" || arg == "--endo" {
            read_endo = true;
        } else {
            args.push(arg);
        }
    }

    Args {
        args,
        dump_rna,
        dump_statistics,
        endo,
    }
}
