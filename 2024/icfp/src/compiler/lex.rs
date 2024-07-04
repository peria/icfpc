pub enum IcfpToken {
    Boolean(bool),
    Integer(i64),
    String(String),
    UnaryOperator(char),
    BinaryOperator(char),
    If,
    LambdaAbstraction(i64),
    Variable(i64),
    Unknown,
}

pub fn lexile(code: &str) -> Vec<IcfpToken> {
    let mut tokens = Vec::new();
    for token in code.split(' ') {
        let indicator = token.chars().nth(0).unwrap();
        let body = &token[1..];
        let icfp_token = match indicator {
            'T' => IcfpToken::Boolean(true),
            'F' => IcfpToken::Boolean(false),
            'S' => parse_icfp_token_string(&body),
            'U' => IcfpToken::UnaryOperator(body.chars().next().unwrap()),
            _ => IcfpToken::Unknown,
        };
        tokens.push(icfp_token);
    }
    tokens
}

const ICFP_CHARS: &str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`|~ \n";

fn parse_icfp_token_string(icfp: &str) -> IcfpToken {
    let ascii = icfp
        .chars()
        .map(|x| ICFP_CHARS.chars().nth((x as u32 - 33) as usize).unwrap())
        .collect();
    IcfpToken::String(ascii)
}
