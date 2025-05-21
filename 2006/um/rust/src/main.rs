use core::panic;
use std::{
    env,
    io::Read,
    ops::{Index, IndexMut},
};

const DEFAULT_FILENAME: &str = "../sandmark.umz";

fn main() {
    let args: Vec<String> = env::args().collect();
    let filename = match args.get(1) {
        Some(f) => f as &str,
        None => DEFAULT_FILENAME,
    };
    let mut um = UniversalMachine::new(filename);
    um.run();
}

#[derive(Default, Copy, Clone)]
struct Platter(u32);

#[derive(Copy, Clone)]
enum Operation {
    CondMove(usize, usize, usize),
    Read(usize, usize, usize),
    Write(usize, usize, usize),
    Add(usize, usize, usize),
    Mult(usize, usize, usize),
    Div(usize, usize, usize),
    Nand(usize, usize, usize),
    Halt,
    Alloc(usize, usize),
    Release(usize),
    Output(usize),
    Input(usize),
    Load(usize, usize),
    Imm(usize, u32),
}

impl Platter {
    fn as_operation(&self) -> Operation {
        let value = self.0;
        let operator = value >> 28;
        let (a, b, c) = ((value >> 6) & 7, (value >> 3) & 7, value & 7);
        let (a, b, c) = (a as usize, b as usize, c as usize);
        match operator {
            0 => Operation::CondMove(a, b, c),
            1 => Operation::Read(a, b, c),
            2 => Operation::Write(a, b, c),
            3 => Operation::Add(a, b, c),
            4 => Operation::Mult(a, b, c),
            5 => Operation::Div(a, b, c),
            6 => Operation::Nand(a, b, c),
            7 => Operation::Halt,
            8 => Operation::Alloc(b, c),
            9 => Operation::Release(c),
            10 => Operation::Output(c),
            11 => Operation::Input(c),
            12 => Operation::Load(b, c),
            13 => {
                let a = ((value >> 25) & 7) as usize;
                let imm = value & 0x1ff_ffff;
                Operation::Imm(a, imm)
            }
            _ => panic!("Unknown operation bits: {}", operator),
        }
    }
}

impl From<u32> for Platter {
    fn from(value: u32) -> Self {
        Platter(value)
    }
}

#[derive(Default, Clone)]
struct Array {
    array: Vec<Platter>,
}

impl Array {
    fn new(size: usize) -> Self {
        Self::from(vec![Platter::from(0); size])
    }
}

impl From<Vec<Platter>> for Array {
    fn from(value: Vec<Platter>) -> Self {
        Array { array: value }
    }
}

impl Index<usize> for Array {
    type Output = Platter;

    fn index(&self, index: usize) -> &Self::Output {
        &self.array[index]
    }
}

impl IndexMut<usize> for Array {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        &mut self.array[index]
    }
}

struct UniversalMachine {
    registers: [Platter; 8],
    arrays: Vec<Array>,
    free_list: Vec<usize>,
}

impl UniversalMachine {
    pub fn new(filename: &str) -> Self {
        let program = Self::load(filename);
        Self {
            registers: [Platter::from(0); 8],
            arrays: vec![Array::from(program)],
            free_list: Vec::new(),
        }
    }

    fn load(filename: &str) -> Vec<Platter> {
        let path = std::path::absolute(filename).unwrap();
        let mut file = std::fs::File::open(&path)
            .expect(&format!("Fail to load a file: {}", path.to_str().unwrap()));
        let mut bytes = Vec::new();
        let size = file
            .read_to_end(&mut bytes)
            .expect("Fail to load a file to bytes.");

        let mut program = Vec::new();
        for i in (0..size).step_by(4) {
            let platter = bytes[i..(i + 4)]
                .iter()
                .rev()
                .enumerate()
                .fold(0, |p, (i, b)| p | (*b as u32) << (i * 8));
            program.push(Platter::from(platter));
        }
        program
    }

    pub fn run(&mut self) {
        let mut pc = 0;
        loop {
            let operation = self.arrays[0][pc].as_operation();
            match operation {
                Operation::CondMove(a, b, c) => {
                    if self.registers[c].0 != 0 {
                        self.registers[a] = self.registers[b];
                    }
                }
                Operation::Read(a, b, c) => {
                    let vb = self.registers[b].0 as usize;
                    let vc = self.registers[c].0 as usize;
                    let ra = &mut self.registers[a];
                    *ra = self.arrays[vb][vc];
                }
                Operation::Write(a, b, c) => {
                    let va = self.registers[a].0 as usize;
                    let vb = self.registers[b].0 as usize;
                    let vc = self.registers[c];
                    self.arrays[va][vb] = vc;
                }
                Operation::Add(a, b, c) => {
                    let vb = self.registers[b].0;
                    let vc = self.registers[c].0;
                    let ra = &mut self.registers[a];
                    *ra = Platter::from(vb.overflowing_add(vc).0);
                }
                Operation::Mult(a, b, c) => {
                    let vb = self.registers[b].0;
                    let vc = self.registers[c].0;
                    let ra = &mut self.registers[a];
                    *ra = Platter::from(vb.overflowing_mul(vc).0);
                }
                Operation::Div(a, b, c) => {
                    let vb = self.registers[b].0;
                    let vc = self.registers[c].0;
                    let ra = &mut self.registers[a];
                    *ra = Platter::from(vb / vc);
                }
                Operation::Nand(a, b, c) => {
                    let vb = self.registers[b].0;
                    let vc = self.registers[c].0;
                    let ra = &mut self.registers[a];
                    *ra = Platter::from(!(vb & vc));
                }
                Operation::Halt => break,
                Operation::Alloc(b, c) => {
                    let vc = self.registers[c].0 as usize;
                    let id = self.allocate(vc);
                    let rb = &mut self.registers[b];
                    *rb = Platter::from(id);
                }
                Operation::Release(c) => {
                    let vc = self.registers[c].0 as usize;
                    self.release(vc);
                }
                Operation::Output(c) => {
                    let vc = self.registers[c].0 as u8 as char;
                    print!("{}", vc);
                }
                Operation::Input(c) => {
                    let mut cs: [u8; 1] = [0u8];
                    let _ = std::io::stdin().lock().read(&mut cs);
                    let rc = &mut self.registers[c];
                    *rc = Platter::from(cs[0] as u32);
                }
                Operation::Load(b, c) => {
                    let vb = self.registers[b].0 as usize;
                    let vc = self.registers[c].0 as usize;
                    if vb != 0 {
                        self.arrays[0] = self.arrays[vb].clone();
                    } else {
                        // Load(0, c) is a jump operation. No need to clone the array.
                    }
                    pc = vc;
                    continue;
                }
                Operation::Imm(a, imm) => {
                    let ra = &mut self.registers[a];
                    *ra = Platter::from(imm);
                }
            }
            pc += 1;
        }
    }

    fn allocate(&mut self, size: usize) -> u32 {
        if let Some(index) = self.free_list.pop() {
            let array = &mut self.arrays[index].array;
            let prev_size = array.len();
            if prev_size < size {
                array.resize(size, Platter::default());
            }
            array[..prev_size.min(size)].fill(Platter::default());
            return index as u32;
        }

        let id = self.arrays.len();
        self.arrays.push(Array::new(size));
        return id as u32;
    }

    fn release(&mut self, index: usize) {
        self.free_list.push(index);
    }
}
