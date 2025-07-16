use std::{fs::File, io::Read};

use ropey::{Rope, RopeBuilder};

fn main() {
    let mut dna_file = File::open("../data/endo.dna").unwrap();
    let mut dna = String::new();
    let _ = dna_file.read_to_string(&mut dna);
    let mut fuun = Fuun::new(&dna);
    fuun.execute();

    // for r in fuun.rna.iter() {
    //     println!("{}", r);
    // }
}

type Base = char;
type DNA = Rope;
type RNA = Vec<String>;
type Pattern = Vec<PItem>;
#[derive(Debug, PartialEq)]
enum PItem {
    Base(Base),
    Skip(usize),
    Search(DNA),
    GroupBegin,
    GroupEnd,
}
type Template = Vec<TItem>;
#[derive(Debug, PartialEq)]
enum TItem {
    Base(Base),
    Protect(usize, usize), // [n, l]
    Number(usize),
}
type Environment = Vec<DNA>;

struct RichDNA {
    dna: DNA,
    pc: usize,
}

impl RichDNA {
    fn len(&self) -> usize {
        self.dna.len_chars() - self.pc
    }

    fn refer(&self, i: usize) -> Option<char> {
        self.dna.get_char(self.pc + i)
    }

    fn consume(&mut self, n: usize) {
        self.pc += n;
    }

    fn to_rna(&self, n: usize) -> String {
        let from = self.pc + n;
        let slice = self.dna.slice(from..(from + 7));
        String::from(slice)
    }

    fn prepend(&mut self, s: DNA) {
        let remained = self.dna.split_off(self.pc);
        self.dna = s;
        self.dna.append(remained);
        self.pc = 0;
    }

    fn find_postfix(&self, mut from: usize, s: &DNA) -> Option<usize> {
        let n = s.len_chars();
        loop {
            if from + n > self.len() {
                return None;
            }

            let slice = self.dna.get_chars_at(self.pc + from).unwrap();
            if slice.zip(s.chars()).all(|(a, b)| a == b) {
                return Some(from + n);
            }
            from += 1;
        }
    }

    fn get_env(&self, from: usize, to: usize) -> DNA {
        let slice = self.dna.slice((self.pc + from)..(self.pc + to));
        DNA::from(slice)
    }
}

impl From<&str> for RichDNA {
    fn from(value: &str) -> Self {
        Self {
            dna: DNA::from(value),
            pc: 0,
        }
    }
}

struct Fuun {
    dna: RichDNA,
    rna: RNA,
}

impl Fuun {
    pub fn new(dna: &str) -> Self {
        Fuun {
            dna: RichDNA::from(dna),
            rna: RNA::new(),
        }
    }

    pub fn execute(&mut self) {
        let mut loop_count = 0;
        loop {
            let p = self.pattern();
            if p.is_none() {
                break;
            }
            let p = p.unwrap();

            let t = self.template();
            if t.is_none() {
                break;
            }
            let t = t.unwrap();

            self.match_replace(p, t);

            loop_count += 1;
            if loop_count < 20 || loop_count % 1000 == 0 {
                eprintln!(
                    "{}-th loop is done. Length = {}, #RNA = {}",
                    loop_count,
                    self.dna.len(),
                    self.rna.len()
                );
            }
        }
        eprintln!(
            "{}-th loop is done. Length = {}, #RNA = {}",
            loop_count,
            self.dna.len(),
            self.rna.len()
        );
    }

    fn pattern(&mut self) -> Option<Pattern> {
        let mut p = Pattern::new();
        let mut level = 0;
        loop {
            let dna = &mut self.dna;
            match dna.refer(0) {
                Some('C') => {
                    dna.consume(1);
                    p.push(PItem::Base('I'));
                }
                Some('F') => {
                    dna.consume(1);
                    p.push(PItem::Base('C'));
                }
                Some('P') => {
                    dna.consume(1);
                    p.push(PItem::Base('F'));
                }
                Some('I') => match dna.refer(1) {
                    Some('C') => {
                        dna.consume(2);
                        p.push(PItem::Base('P'));
                    }
                    Some('P') => {
                        dna.consume(2);
                        match self.nat() {
                            Some(n) => p.push(PItem::Skip(n)),
                            None => return None,
                        }
                    }
                    Some('F') => {
                        dna.consume(3); // Consume 3 bases
                        let s = self.consts();
                        p.push(PItem::Search(s));
                    }
                    Some('I') => match dna.refer(2) {
                        Some('P') => {
                            dna.consume(3);
                            level += 1;
                            p.push(PItem::GroupBegin);
                        }
                        Some('C') | Some('F') => {
                            dna.consume(3);
                            if level == 0 {
                                return Some(p);
                            } else {
                                level -= 1;
                                p.push(PItem::GroupEnd);
                            }
                        }
                        Some('I') => {
                            let rna = self.dna.to_rna(3);
                            self.rna.push(rna);
                            self.dna.consume(10);
                        }
                        _ => return None,
                    },
                    _ => return None,
                },
                _ => return None,
            }
        }
    }

    fn template(&mut self) -> Option<Template> {
        let mut t = Template::new();
        loop {
            let dna = &mut self.dna;
            match dna.refer(0) {
                Some('C') => {
                    dna.consume(1);
                    t.push(TItem::Base('I'));
                }
                Some('F') => {
                    dna.consume(1);
                    t.push(TItem::Base('C'));
                }
                Some('P') => {
                    dna.consume(1);
                    t.push(TItem::Base('F'));
                }
                Some('I') => match dna.refer(1) {
                    Some('C') => {
                        dna.consume(2);
                        t.push(TItem::Base('P'));
                    }
                    Some('F') | Some('P') => {
                        dna.consume(2);
                        let l = self.nat();
                        if l.is_none() {
                            return None;
                        }
                        let n = self.nat();
                        if n.is_none() {
                            return None;
                        }
                        t.push(TItem::Protect(n.unwrap(), l.unwrap()));
                    }
                    Some('I') => match dna.refer(2) {
                        Some('C') | Some('F') => {
                            dna.consume(3);
                            return Some(t);
                        }
                        Some('P') => {
                            dna.consume(3);
                            match self.nat() {
                                Some(n) => t.push(TItem::Number(n)),
                                None => return None,
                            }
                        }
                        Some('I') => {
                            let rna = dna.to_rna(3);
                            self.rna.push(rna);
                            dna.consume(10);
                        }
                        _ => return None,
                    },
                    _ => return None,
                },
                _ => return None,
            }
        }
    }

    fn match_replace(&mut self, pat: Pattern, t: Template) {
        let mut i = 0;
        let mut e = Environment::new();
        let mut c = Vec::new();
        for p in pat.iter() {
            match p {
                PItem::Base(b) => {
                    if self.dna.refer(i) == Some(*b) {
                        i += 1;
                    } else {
                        return;
                    }
                }
                PItem::Skip(n) => {
                    i += n;
                    if i > self.dna.len() {
                        return;
                    }
                }
                PItem::Search(s) => {
                    if let Some(n) = self.dna.find_postfix(i, s) {
                        i = n
                    } else {
                        return;
                    }
                }
                PItem::GroupBegin => c.push(i),
                PItem::GroupEnd => {
                    let c0 = c.pop().unwrap();
                    let env = self.dna.get_env(c0, i);
                    e.push(env);
                }
            }
        }
        self.dna.consume(i);
        self.replace(t, e);
    }

    fn nat(&mut self) -> Option<usize> {
        match self.dna.refer(0) {
            Some('P') => {
                self.dna.consume(1);
                Some(0)
            }
            Some('I') | Some('F') => {
                self.dna.consume(1);
                match self.nat() {
                    Some(n) => Some(2 * n),
                    None => None,
                }
            }
            Some('C') => {
                self.dna.consume(1);
                match self.nat() {
                    Some(n) => Some(2 * n + 1),
                    None => None,
                }
            }
            _ => None,
        }
    }

    fn consts(&mut self) -> DNA {
        match self.dna.refer(0) {
            Some('C') => {
                self.dna.consume(1);
                let mut s = self.consts();
                s.insert_char(0, 'I');
                s
            }
            Some('F') => {
                self.dna.consume(1);
                let mut s = self.consts();
                s.insert_char(0, 'C');
                s
            }
            Some('P') => {
                self.dna.consume(1);
                let mut s = self.consts();
                s.insert_char(0, 'F');
                s
            }
            Some('I') => match self.dna.refer(1) {
                Some('C') => {
                    self.dna.consume(2);
                    let mut s = self.consts();
                    s.insert_char(0, 'P');
                    s
                }
                _ => DNA::new(),
            },
            _ => DNA::new(),
        }
    }

    fn replace(&mut self, tpl: Template, e: Environment) {
        let mut r = DNA::new();
        for t in tpl.iter() {
            match t {
                TItem::Base(b) => r.insert_char(r.len_chars(), *b),
                TItem::Protect(n, l) => r.append(Self::protect(*l, &e[*n])),
                TItem::Number(n) => r.append(Self::asnat(e[*n].len_chars())),
            }
        }
        self.dna.prepend(r);
    }

    fn protect(l: usize, d: &DNA) -> DNA {
        if l == 0 {
            d.clone()
        } else {
            Self::protect(l - 1, &Self::quote(d))
        }
    }

    fn quote(d: &DNA) -> DNA {
        let mut builder = RopeBuilder::new();
        for c in d.chars() {
            match c {
                'I' => builder.append("C"),
                'C' => builder.append("F"),
                'F' => builder.append("P"),
                'P' => builder.append("IC"),
                _ => (),
            }
        }
        builder.finish()
    }

    fn asnat(mut n: usize) -> DNA {
        let mut builder = RopeBuilder::new();
        while n > 0 {
            if n % 2 == 0 {
                builder.append("I");
            } else {
                builder.append("C");
            }
            n /= 2;
        }
        builder.append("P");
        builder.finish()
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn pattern_test() {
        let dna = "CIIC";
        let mut fuun = Fuun::new(dna);
        let actual = fuun.pattern();
        assert!(actual.is_some());
        let actual = actual.unwrap();
        assert_eq!(1, actual.len());
        assert_eq!(PItem::Base('I'), actual[0]);
    }
}
