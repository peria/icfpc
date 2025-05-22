use std::{
    ops::{Index, IndexMut},
    time::Instant,
};

pub(crate) mod plain;

pub trait UniversalMachine {
    /// Build a UM implementation. The first program is loaded on built.
    fn new(filename: &str) -> Self;

    /// Runs the instructions.
    fn run(&mut self);

    /// Reports some statistics that may be used for optimization.
    fn report(&self);
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

pub(crate) struct Statistics {
    start: Instant,
}
