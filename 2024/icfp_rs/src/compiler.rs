mod lex;

use std::collections::VecDeque;

use lex::IcfpToken;

pub fn compile(code: &str) -> String {
    let tokens = lex::lexile(code);
    let root = build_ast(&mut VecDeque::from(tokens));
    root.to_string()
}

fn build_ast(tokens: &mut VecDeque<IcfpToken>) -> Box<dyn AstNode> {
    match tokens.pop_front().unwrap() {
        IcfpToken::Boolean(x) => Box::new(AstBoolean { value: x }),
        IcfpToken::Integer(x) => Box::new(AstInteger { value: x }),
        IcfpToken::String(x) => Box::new(AstString { value: x }),
        IcfpToken::UnaryOperator(x) => build_unary_operator(x, build_ast(tokens)),
        IcfpToken::BinaryOperator(x) => {
            build_binary_operator(x, build_ast(tokens), build_ast(tokens))
        }
        IcfpToken::If => Box::new(AstIf {
            condition: build_ast(tokens),
            true_branch: build_ast(tokens),
            false_branch: build_ast(tokens),
        }),
        IcfpToken::LambdaAbstraction(x) => Box::new(AstLambda {
            id: x,
            argument: build_ast(tokens),
        }),
        IcfpToken::Variable(x) => Box::new(AstVariable { id: x }),
        _ => Box::new(AstNone {}),
    }
}

fn build_unary_operator(operator: char, operand: Box<dyn AstNode>) -> Box<dyn AstNode> {
    Box::new(AstUnaryOperator { operator, operand })
}

fn build_binary_operator(
    operator: char,
    lhs: Box<dyn AstNode>,
    rhs: Box<dyn AstNode>,
) -> Box<dyn AstNode> {
    // TODO: Make Eval node independently.
    Box::new(AstBinaryOperator { operator, lhs, rhs })
}

trait AstNode {
    fn to_string(&self) -> String;
}

struct AstBoolean {
    value: bool,
}

impl AstNode for AstBoolean {
    fn to_string(&self) -> String {
        if self.value {
            "True".to_string()
        } else {
            "False".to_string()
        }
    }
}

struct AstInteger {
    value: i64,
}

impl AstNode for AstInteger {
    fn to_string(&self) -> String {
        self.value.to_string()
    }
}

struct AstString {
    value: String,
}

impl AstNode for AstString {
    fn to_string(&self) -> String {
        self.value.clone()
    }
}

struct AstUnaryOperator {
    operator: char,
    operand: Box<dyn AstNode>,
}

impl AstNode for AstUnaryOperator {
    fn to_string(&self) -> String {
        format!("({} {})", self.operator, self.operand.to_string())
    }
}

struct AstBinaryOperator {
    operator: char,
    lhs: Box<dyn AstNode>,
    rhs: Box<dyn AstNode>,
}

impl AstNode for AstBinaryOperator {
    fn to_string(&self) -> String {
        format!(
            "({} {} {})",
            self.lhs.to_string(),
            self.operator,
            self.rhs.to_string()
        )
    }
}

struct AstIf {
    condition: Box<dyn AstNode>,
    true_branch: Box<dyn AstNode>,
    false_branch: Box<dyn AstNode>,
}

impl AstNode for AstIf {
    fn to_string(&self) -> String {
        format!(
            "\nIF {}\nTHEN {}\nELSE {}",
            self.condition.to_string(),
            self.true_branch.to_string(),
            self.false_branch.to_string()
        )
    }
}

struct AstLambda {
    id: i64,
    argument: Box<dyn AstNode>,
}

impl AstNode for AstLambda {
    fn to_string(&self) -> String {
        format!("\\x{} ({})", self.id, self.argument.to_string())
    }
}

struct AstVariable {
    id: i64,
}

impl AstNode for AstVariable {
    fn to_string(&self) -> String {
        format!("x{}", self.id)
    }
}

struct AstNone {}

impl AstNode for AstNone {
    fn to_string(&self) -> String {
        "NONE".to_string()
    }
}

#[cfg(test)]
mod test {
    use super::compile;

    #[test]
    fn parse_ast_test() {
        let result = compile("B$ B$ L# L$ v# B. SB%,,/ S}Q/2,$_ IK");
        assert_eq!("Hello, world", result);
    }
}
