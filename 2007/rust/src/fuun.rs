use ropey::{Rope, RopeBuilder};

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

pub struct Fuun {
    pub rna: RNA,
}

impl Fuun {
    pub fn new() -> Self {
        Fuun { rna: RNA::new() }
    }

    pub fn execute(&mut self, dna: &str) {
        let mut dna = RichDNA::from(dna);
        let mut loop_count = 0;
        loop {
            let p = self.pattern(&mut dna);
            if p.is_none() {
                break;
            }
            let p = p.unwrap();

            let t = self.template(&mut dna);
            if t.is_none() {
                break;
            }
            let t = t.unwrap();

            self.match_replace(p, t, &mut dna);

            loop_count += 1;
            if loop_count < 20 || loop_count % 10000 == 0 {
                eprintln!(
                    "{}-th loop is done. Length = {}, #RNA = {}",
                    loop_count,
                    dna.len(),
                    self.rna.len()
                );
            }
        }
        eprintln!(
            "{}-th loop is done. Length = {}, #RNA = {}",
            loop_count,
            dna.len(),
            self.rna.len()
        );
    }

    fn pattern(&mut self, dna: &mut RichDNA) -> Option<Pattern> {
        let mut p = Pattern::new();
        let mut level = 0;
        loop {
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
                        match self.nat(dna) {
                            Some(n) => p.push(PItem::Skip(n)),
                            None => return None,
                        }
                    }
                    Some('F') => {
                        dna.consume(3); // Consume 3 bases
                        let s = self.consts(dna);
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

    fn template(&mut self, dna: &mut RichDNA) -> Option<Template> {
        let mut t = Template::new();
        loop {
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
                        let l = self.nat(dna);
                        if l.is_none() {
                            return None;
                        }
                        let n = self.nat(dna);
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
                            match self.nat(dna) {
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

    fn match_replace(&mut self, pat: Pattern, t: Template, dna: &mut RichDNA) {
        let mut i = 0;
        let mut e = Environment::new();
        let mut c = Vec::new();
        for p in pat.iter() {
            match p {
                PItem::Base(b) => {
                    if dna.refer(i) == Some(*b) {
                        i += 1;
                    } else {
                        return;
                    }
                }
                PItem::Skip(n) => {
                    i += n;
                    if i > dna.len() {
                        return;
                    }
                }
                PItem::Search(s) => {
                    if let Some(n) = dna.find_postfix(i, s) {
                        i = n
                    } else {
                        return;
                    }
                }
                PItem::GroupBegin => c.push(i),
                PItem::GroupEnd => {
                    let c0 = c.pop().unwrap();
                    let env = dna.get_env(c0, i);
                    e.push(env);
                }
            }
        }
        dna.consume(i);
        self.replace(t, e, dna);
    }

    fn nat(&mut self, dna: &mut RichDNA) -> Option<usize> {
        let mut n = 0usize;
        let mut b = 1;
        for i in 0..dna.len() {
            match dna.refer(i) {
                Some('P') => {
                    dna.consume(i + 1);
                    return Some(n);
                }
                Some('I') | Some('F') => {}
                Some('C') => {
                    n += b;
                }
                _ => return None,
            }
            b *= 2;
        }
        dna.consume(dna.len());
        None
    }

    fn consts(&mut self, dna: &mut RichDNA) -> DNA {
        let mut s = DNA::new();
        let mut n = 0;
        loop {
            match dna.refer(0) {
                Some('C') => {
                    dna.consume(1);
                    s.insert_char(n, 'I');
                }
                Some('F') => {
                    dna.consume(1);
                    s.insert_char(n, 'C');
                }
                Some('P') => {
                    dna.consume(1);
                    s.insert_char(n, 'F');
                }
                Some('I') => match dna.refer(1) {
                    Some('C') => {
                        dna.consume(2);
                        s.insert_char(n, 'P');
                    }
                    _ => break,
                },
                _ => break,
            }
            n += 1;
        }
        s
    }

    fn replace(&mut self, tpl: Template, e: Environment, dna: &mut RichDNA) {
        let mut r = DNA::new();
        for t in tpl.iter() {
            match t {
                TItem::Base(b) => r.insert_char(r.len_chars(), *b),
                TItem::Protect(n, l) => r.append(Self::protect(*l, &e[*n])),
                TItem::Number(n) => r.append(Self::asnat(e[*n].len_chars())),
            }
        }
        dna.prepend(r);
    }

    fn protect(l: usize, d: &DNA) -> DNA {
        Self::rec_protect(l, d)
    }

    fn rec_protect(l: usize, d: &DNA) -> DNA {
        if l == 0 {
            d.clone()
        } else {
            Self::rec_protect(l - 1, &Self::quote(d))
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
        let mut dna = RichDNA::from("CIIC");
        let mut fuun = Fuun::new();
        let actual = fuun.pattern(&mut dna);
        assert!(actual.is_some());
        let actual = actual.unwrap();
        assert_eq!(1, actual.len());
        assert_eq!(PItem::Base('I'), actual[0]);
    }
}
