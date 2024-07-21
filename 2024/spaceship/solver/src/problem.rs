use std::{collections::HashSet, io::BufRead};

pub struct Problem {
    pub xs: Vec<i64>,
    pub ys: Vec<i64>,
    pub xrange: (i64, i64),
    pub yrange: (i64, i64),
}

impl Problem {
    pub fn read_stdin() -> Self {
        // Remove duplicates.
        let mut xys = HashSet::new();
        let stdin = std::io::stdin();
        for line in stdin.lock().lines() {
            let line = line.unwrap();
            if line.trim().len() == 0 {
                continue;
            }
            let xy: Vec<i64> = line.split(' ').map(|x| x.parse::<i64>().unwrap()).collect();
            xys.insert((xy[0], xy[1]));
        }
        // We can ignore the starting position (0, 0).
        xys.remove(&(0, 0));

        let mut xs = Vec::new();
        let mut ys = Vec::new();
        for (x, y) in xys.iter() {
            xs.push(*x);
            ys.push(*y);
        }
        let xrange = (*xs.iter().min().unwrap(), *xs.iter().max().unwrap());
        let yrange = (*ys.iter().min().unwrap(), *ys.iter().max().unwrap());
        Problem {
            xs,
            ys,
            xrange,
            yrange,
        }
    }

    pub fn len(&self) -> usize {
        self.xs.len()
    }
}
