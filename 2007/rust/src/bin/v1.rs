use core::panic;
use std::{fs::File, io::Read};

use ropey::Rope;

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

type Base = char; // 'I', 'C', 'F', or 'P'
type DNA = ropey::Rope;
type RNA = Vec<DNA>;
type Environment = Vec<DNA>;

#[derive(Debug, Clone, PartialEq)]
enum PItem {
    Base(Base),
    Skip(usize),
    Search(DNA),
    GroupBegin,
    GroupEnd,
}
type Pattern = Vec<PItem>;

#[derive(Debug, Clone, PartialEq)]
enum TItem {
    Base(Base),
    Reference(usize, usize),
    Length(usize),
}
type Template = Vec<TItem>;

#[derive(Debug)]
struct RichDNA {
    dna: DNA,
    pc: usize,
}

impl RichDNA {
    fn new(dna: &str) -> Self {
        let dna = DNA::from(dna);
        RichDNA { dna, pc: 0 }
    }

    fn len(&self) -> usize {
        self.dna.len_chars()
    }

    fn consume(&mut self) -> Option<Base> {
        let b = self.refer(0);
        if b.is_some() {
            self.pc += 1;
        }
        b
    }

    fn consume_rna(&mut self) -> DNA {
        const RNA_SIZE: usize = 7;
        let slice = self.dna.slice(self.pc..(self.pc + RNA_SIZE));
        self.pc += RNA_SIZE;
        DNA::from(slice)
    }

    fn refer(&self, offset: usize) -> Option<Base> {
        let pc = self.pc + offset;
        self.dna.get_char(pc)
    }

    fn get_env(&self, i: usize, j: usize) -> DNA {
        let offset = self.pc;
        DNA::from(self.dna.slice((offset + i)..(offset + j)))
    }

    fn rollback(&mut self, size: usize) {
        self.pc -= size;
    }

    fn position(&self, mut from: usize, s: &DNA) -> Option<usize> {
        loop {
            if self.refer(from).is_none() {
                return None;
            }

            let mut is_ok = true;
            for (j, b) in s.chars().enumerate() {
                if Some(b) != self.refer(from + j) {
                    is_ok = false;
                    break;
                }
            }
            if is_ok {
                return Some(from);
            }
            from += 1;
        }
    }

    fn seek(&mut self, size: usize) {
        self.pc += size;
    }

    fn gc(&mut self) {
        let pc = self.pc;
        self.pc = 0;
        self.dna.remove(..pc);
    }

    fn prepend(&mut self, r: Rope) {
        self.dna.insert(0, &r.to_string());
    }

    #[allow(dead_code)]
    fn to_string(&self) -> String {
        self.dna.to_string()
    }
}

struct Fuun {
    rna: RNA,
    dna: RichDNA,
}

impl Fuun {
    pub fn new(dna: &str) -> Self {
        Self {
            rna: RNA::new(),
            dna: RichDNA::new(dna),
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
        self.finish();
    }

    fn finish(&self) {
        eprintln!(
            "Exec is done. Length = {}, #RNA = {}",
            self.dna.len(),
            self.rna.len()
        );
    }

    fn pattern(&mut self) -> Option<Pattern> {
        let mut p = Vec::new();
        let mut level = 0;
        loop {
            match self.dna.consume() {
                Some('C') => p.push(PItem::Base('I')),
                Some('F') => p.push(PItem::Base('C')),
                Some('P') => p.push(PItem::Base('F')),
                Some('I') => {
                    match self.dna.consume() {
                        Some('C') => p.push(PItem::Base('P')),
                        Some('F') => {
                            self.dna.consume(); // three bases consumed.
                            let s = self.consts();
                            p.push(PItem::Search(s));
                        }
                        Some('P') => {
                            let n = self.nat();
                            let n = if n.1 { 0xffff_ffff } else { n.0 };
                            p.push(PItem::Skip(n));
                        }
                        Some('I') => match self.dna.consume() {
                            Some('C') | Some('F') => {
                                if level == 0 {
                                    return Some(p);
                                }
                                level -= 1;
                                p.push(PItem::GroupEnd);
                            }
                            Some('I') => {
                                self.rna.push(self.dna.consume_rna());
                            }
                            Some('P') => {
                                p.push(PItem::GroupBegin);
                                level += 1;
                            }
                            _ => return None,
                        },
                        _ => return None,
                    }
                }
                _ => return None,
            }
        }
    }

    fn nat(&mut self) -> (usize, bool) {
        let mut n = 0;
        let mut base: usize = 1;
        let mut overflow = false;
        loop {
            match self.dna.consume() {
                Some('P') => return (n, overflow),
                Some('C') => n += base,
                Some('I') | Some('F') => (),
                _ => panic!(),
            }
            let r = base.overflowing_mul(2);
            base = r.0;
            overflow |= r.1;
        }
    }

    fn consts(&mut self) -> DNA {
        let mut s = DNA::new();
        loop {
            match self.dna.consume() {
                Some('C') => s.insert_char(0, 'I'),
                Some('F') => s.insert_char(0, 'C'),
                Some('P') => s.insert_char(0, 'F'),
                Some('I') => match self.dna.consume() {
                    Some('C') => s.insert_char(0, 'P'),
                    _ => {
                        self.dna.rollback(2);
                        return s;
                    }
                },
                _ => {
                    self.dna.rollback(1);
                    return s;
                }
            }
        }
    }

    fn template(&mut self) -> Option<Template> {
        let mut t = Template::new();
        loop {
            match self.dna.consume() {
                Some('C') => t.push(TItem::Base('I')),
                Some('F') => t.push(TItem::Base('C')),
                Some('P') => t.push(TItem::Base('F')),
                Some('I') => match self.dna.consume() {
                    Some('C') => t.push(TItem::Base('P')),
                    Some('F') | Some('P') => {
                        let l = self.nat();
                        let n = self.nat();
                        debug_assert!(!(l.1 || n.1));
                        t.push(TItem::Reference(l.0, n.0));
                    }
                    Some('I') => match self.dna.consume() {
                        Some('C') | Some('F') => return Some(t),
                        Some('P') => {
                            let n = self.nat();
                            debug_assert!(!n.1);
                            t.push(TItem::Length(n.0));
                        }
                        Some('I') => self.rna.push(self.dna.consume_rna()),
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
                        i += 1
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
                    let n = self.dna.position(i, s);
                    if n.is_none() {
                        return;
                    }
                    i = n.unwrap() + s.len_chars();
                }
                PItem::GroupBegin => c.push(i),
                PItem::GroupEnd => {
                    let c0 = c.pop().unwrap();
                    let env = self.dna.get_env(c0, i);
                    e.push(env);
                }
            }
        }
        self.dna.seek(i);
        self.dna.gc();

        self.replace(t, e);
    }

    fn replace(&mut self, tpl: Template, env: Environment) {
        let mut r = DNA::new();
        for t in tpl.iter() {
            match t {
                &TItem::Base(b) => r.insert_char(r.len_chars(), b),
                &TItem::Reference(l, n) => r.append(Self::protect(l, &env[n])),
                &TItem::Length(n) => {
                    let len = if n < env.len() { env[n].len_chars() } else { 0 };
                    r.append(Self::asnat(len));
                }
            }
        }
        self.dna.prepend(r);
    }

    fn protect(l: usize, d: &DNA) -> DNA {
        let mut dna = d.clone();
        for _ in 0..l {
            dna = Self::quote(&dna);
        }
        dna
    }

    fn quote(d: &DNA) -> DNA {
        let mut dna = String::new();
        for b in d.chars() {
            match b {
                'I' => dna.push('C'),
                'C' => dna.push('F'),
                'F' => dna.push('P'),
                'P' => {
                    dna.push('I');
                    dna.push('C');
                }
                _ => panic!(),
            }
        }
        Rope::from(dna)
    }

    fn asnat(mut n: usize) -> DNA {
        let mut dna = DNA::from("P");
        while n > 0 {
            if n % 2 == 0 {
                dna.insert_char(0, 'I');
            } else {
                dna.insert_char(0, 'C');
            }
            n /= 2;
        }
        dna
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn pattern_test() {
        let mut fuun = Fuun::new("CIIC");
        let pattern = fuun.pattern();
        assert!(pattern.is_some());
        let pattern = pattern.unwrap();
        assert_eq!(pattern.len(), 1);
        assert_eq!(pattern[0], PItem::Base('I'));

        let mut fuun = Fuun::new("IIPIPICPIICICIIF");
        let pattern = fuun.pattern();
        assert!(pattern.is_some());
        let pattern = pattern.unwrap();

        assert_eq!(pattern.len(), 4);
        assert_eq!(pattern[0], PItem::GroupBegin);
        assert_eq!(pattern[1], PItem::Skip(2));
        assert_eq!(pattern[2], PItem::GroupEnd);
        assert_eq!(pattern[3], PItem::Base('P'));
    }

    #[test]
    fn execute_test() {
        let test_data = [
            ("IIPIPICPIICICIIFICCIFPPIICCFPC", "PICFC"),
            ("IIPIPICPIICICIIFICCIFCCCPPIICCFPC", "PIICCFCFFPC"),
            ("IIPIPIICPIICIICCIICFCFC", "I"),
        ];
        for &(dna, expect) in test_data.iter() {
            let mut fuun = Fuun::new(dna);
            fuun.execute();
            assert_eq!(fuun.dna.to_string(), expect);
        }
    }
}
