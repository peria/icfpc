type Integer = i64;

static ICFP_ASCII: &str = &"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`|~ \n";
static mut ICFP_INDEX: [usize; 256] = [0; 256];

// Initialize ICFP_INDEX
fn init_index() {
    unsafe {
        if ICFP_INDEX.len() == 0 {
            for (i, c) in ICFP_ASCII.chars().enumerate() {
                ICFP_INDEX[c as usize] = i;
            }
        }
    }
}

pub fn icfp_to_ascii(icfp: &str) -> String {
    let mut ascii = String::new();
    for c in icfp.chars() {
        ascii.push(ICFP_ASCII.chars().nth(c as usize - '!' as usize).unwrap());
    }
    ascii
}

pub fn ascii_to_icfp(ascii: &str) -> String {
    init_index();
    let mut icfp = String::new();
    unsafe {
        for c in ascii.chars() {
            icfp.push((ICFP_INDEX[c as usize] + '!' as usize) as u8 as char);
        }
    }
    icfp
}

pub fn ascii_to_int(icfp: &str) -> Integer {
    init_index();
    let mut value = 0;
    unsafe {
        for c in icfp.chars() {
            value = value * 94 + ICFP_INDEX[c as usize] as i64;
        }
    }
    value
}

// May be never used.
pub fn int_to_ascii(mut value: Integer) -> String {
    if value == 0 {
        return "!".to_string();
    }

    let mut ascii = String::new();
    while value > 0 {
        ascii.push(ICFP_ASCII.chars().nth((value % 94) as usize).unwrap());
        value /= 94;
    }
    ascii.chars().rev().collect()
}

pub fn icfp_to_int(icfp: &str) -> Integer {
    let mut value = 0;
    for c in icfp.chars() {
        value = value * 94 + c as Integer - '!' as Integer;
    }
    value
}

pub fn int_to_icfp(mut value: Integer) -> String {
    if value == 0 {
        return "!".to_string();
    }

    let mut ascii = String::new();
    while value > 0 {
        ascii.push((value % 94 + '!' as Integer) as u8 as char);
        value /= 94;
    }
    ascii.chars().rev().collect()
}

pub fn translate(message: &str) -> String {
    "S".to_string() + &ascii_to_icfp(message)
}

#[test]
fn test_integer_code() {
    assert_eq!(icfp_to_int("!"), 0);
    assert_eq!(icfp_to_int("/6"), 1337); // from the rules
    assert_eq!(int_to_icfp(0), "!");
    assert_eq!(int_to_icfp(1337), "/6");
}
