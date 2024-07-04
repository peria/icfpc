mod lex;

pub fn compile(code: &str) -> String {
    let tokens = lex::lexile(code);
    let root = build_ast(tokens.iter().next().unwrap());
    root.to_str().to_string()
}

fn build_ast(token: &lex::IcfpToken) -> Box<dyn AstNode> {
    match token {
        lex::IcfpToken::String(x) => Box::new(AstString { value: x.clone() }),
        _ => Box::new(AstNone {}),
    }
}

trait AstNode {
    fn to_str(&self) -> &str;
}

struct AstString {
    value: String,
}

impl AstNode for AstString {
    fn to_str(&self) -> &str {
        &self.value
    }
}

struct AstNone {}

impl AstNode for AstNone {
    fn to_str(&self) -> &str {
        &""
    }
}
