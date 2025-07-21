mod arrow;
mod fuun;

use clap::Parser;
use clap_derive::Parser;
use serde::{Deserialize, Serialize};
use sha2::Digest;
use std::{fs::File, io::Read};

const DEFAULT_ENDO_PATH: &str = "../data/endo.dna";

#[derive(Parser, Debug)]
struct Args {
    /// Dump statistics
    #[arg(short, long)]
    statistics: bool,

    /// Specify a file of Endo's DNA
    #[arg(short, long)]
    endo: Option<String>,

    /// Prefix bases to prefix on Endo's DNA.
    #[arg(short, long)]
    prefix: Option<String>,

    /// Directory to store image files.
    #[arg(long)]
    image_dir: Option<String>,
}

#[derive(Serialize, Deserialize)]
struct Statistics {
    prefix_size: usize,
    dna2rna: f64, // sec.
    rna: usize,   // count
}

fn main() {
    let args = Args::parse();
    let mut statistics = Statistics::new();

    let endo_path = args.endo.unwrap_or(DEFAULT_ENDO_PATH.to_string());

    let prefix = args.prefix.unwrap_or_default().clone();
    let mut dna = prefix.clone();
    let endo = load_endo(&endo_path);
    dna.push_str(&endo);
    let execute_start = std::time::Instant::now();
    let mut fuun = fuun::Fuun::new();
    fuun.execute(&dna);
    let dna2rna_time = (std::time::Instant::now() - execute_start).as_secs_f64();

    let mut arrow = arrow::Arrow::new();
    let bitmap = arrow.build(&fuun.rna);
    let hash = sha2::Sha256::digest(prefix.as_bytes());
    save_bitmap(
        &bitmap,
        &format!(
            "{}/{:x}.png",
            args.image_dir.unwrap_or(".".to_string()),
            hash
        ),
    );

    statistics.prefix_size = prefix.len();
    statistics.dna2rna = dna2rna_time;
    statistics.rna = fuun.rna.len();
    if args.statistics {
        eprintln!("{}", serde_json::to_string(&statistics).unwrap());
    }
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

impl Statistics {
    fn new() -> Self {
        Statistics {
            prefix_size: 0,
            dna2rna: 0.0f64,
            rna: 0,
        }
    }
}
