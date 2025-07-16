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

struct Fuun {
    dna: DNA,
    rna: RNA,
}

impl Fuun {
    pub fn new(dna: &str) -> Self {
        Fuun {
            dna: DNA::from(dna),
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
                    self.dna.len_chars(),
                    self.rna.len()
                );
            }
        }
        eprintln!(
            "{}-th loop is done. Length = {}, #RNA = {}",
            loop_count,
            self.dna.len_chars(),
            self.rna.len()
        );
    }

    fn pattern(&mut self) -> Option<Pattern> {
        let mut p = Pattern::new();
        let mut level = 0;
        loop {
            let dna = &mut self.dna;
            match dna.get_char(0) {
                Some('C') => {
                    dna.remove(..1);
                    p.push(PItem::Base('I'));
                }
                Some('F') => {
                    dna.remove(..1);
                    p.push(PItem::Base('C'));
                }
                Some('P') => {
                    dna.remove(..1);
                    p.push(PItem::Base('F'));
                }
                Some('I') => match dna.get_char(1) {
                    Some('C') => {
                        dna.remove(..2);
                        p.push(PItem::Base('P'));
                    }
                    Some('P') => {
                        dna.remove(..2);
                        match self.nat() {
                            Some(n) => p.push(PItem::Skip(n)),
                            None => return None,
                        }
                    }
                    Some('F') => {
                        dna.remove(..3); // Consume 3 bases
                        let s = self.consts();
                        p.push(PItem::Search(s));
                    }
                    Some('I') => match dna.get_char(2) {
                        Some('P') => {
                            dna.remove(..3);
                            level += 1;
                            p.push(PItem::GroupBegin);
                        }
                        Some('C') | Some('F') => {
                            dna.remove(..3);
                            if level == 0 {
                                return Some(p);
                            } else {
                                level -= 1;
                                p.push(PItem::GroupEnd);
                            }
                        }
                        Some('I') => {
                            let rna = self.dna.slice(3..10);
                            self.rna.push(String::from(rna));
                            self.dna.remove(..10);
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
            match dna.get_char(0) {
                Some('C') => {
                    dna.remove(..1);
                    t.push(TItem::Base('I'));
                }
                Some('F') => {
                    dna.remove(..1);
                    t.push(TItem::Base('C'));
                }
                Some('P') => {
                    dna.remove(..1);
                    t.push(TItem::Base('F'));
                }
                Some('I') => match dna.get_char(1) {
                    Some('C') => {
                        dna.remove(..2);
                        t.push(TItem::Base('P'));
                    }
                    Some('F') | Some('P') => {
                        dna.remove(..2);
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
                    Some('I') => match dna.get_char(2) {
                        Some('C') | Some('F') => {
                            dna.remove(..3);
                            return Some(t);
                        }
                        Some('P') => {
                            dna.remove(..3);
                            match self.nat() {
                                Some(n) => t.push(TItem::Number(n)),
                                None => return None,
                            }
                        }
                        Some('I') => {
                            let rna = dna.slice(3..10);
                            self.rna.push(String::from(rna));
                            dna.remove(..10);
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
                    if self.dna.get_char(i) == Some(*b) {
                        i += 1;
                    } else {
                        return;
                    }
                }
                PItem::Skip(n) => {
                    i += n;
                    if i > self.dna.len_chars() {
                        return;
                    }
                }
                PItem::Search(s) => {
                    if let Some(n) = self.find_postfix(i, s) {
                        i = n
                    } else {
                        return;
                    }
                }
                PItem::GroupBegin => c.push(i),
                PItem::GroupEnd => {
                    let c0 = c.pop().unwrap();
                    let env = self.dna.slice(c0..i);
                    e.push(DNA::from(env));
                }
            }
        }
        self.dna.remove(..i);
        self.replace(t, e);
    }

    fn nat(&mut self) -> Option<usize> {
        match self.dna.get_char(0) {
            Some('P') => {
                self.dna.remove(..1);
                Some(0)
            }
            Some('I') | Some('F') => {
                self.dna.remove(..1);
                match self.nat() {
                    Some(n) => Some(2 * n),
                    None => None,
                }
            }
            Some('C') => {
                self.dna.remove(..1);
                match self.nat() {
                    Some(n) => Some(2 * n + 1),
                    None => None,
                }
            }
            _ => None,
        }
    }

    fn consts(&mut self) -> DNA {
        match self.dna.get_char(0) {
            Some('C') => {
                self.dna.remove(..1);
                let mut s = self.consts();
                s.insert_char(0, 'I');
                s
            }
            Some('F') => {
                self.dna.remove(..1);
                let mut s = self.consts();
                s.insert_char(0, 'C');
                s
            }
            Some('P') => {
                self.dna.remove(..1);
                let mut s = self.consts();
                s.insert_char(0, 'F');
                s
            }
            Some('I') => match self.dna.get_char(1) {
                Some('C') => {
                    self.dna.remove(..2);
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
        self.dna.insert(0, &r.to_string());
    }

    fn find_postfix(&self, mut from: usize, s: &Rope) -> Option<usize> {
        let n = s.len_chars();
        loop {
            if from + n > self.dna.len_chars() {
                return None;
            }

            let slice = self.dna.get_chars_at(from).unwrap();
            if slice.zip(s.chars()).all(|(a, b)| a == b) {
                return Some(from + n);
            }
            from += 1;
        }
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
