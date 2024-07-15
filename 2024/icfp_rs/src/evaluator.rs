use std::collections::VecDeque;

use crate::ast_node::*;
use crate::translator;

pub fn evaluate(message: String) -> String {
    let node = evaluate_core(message);
    // eprintln!("Value type is {}", node.get_type());
    node.to_string()
}

fn evaluate_core(message: String) -> Box<dyn AstNode> {
    const MAX_LOOP: usize = 100000;
    let mut tokens: VecDeque<_> = message.split(' ').collect();
    let mut root = build_ast(&mut tokens);
    for i in 0..MAX_LOOP {
        let new_root = root.evaluate();
        if let Some(x) = new_root.as_any().downcast_ref::<AstString>() {
            return new_root;
        }
        root = new_root;
    }
    eprintln!("Can't evaluate in {} loops.", MAX_LOOP);
    root
}

fn build_ast(tokens: &mut VecDeque<&str>) -> Box<dyn AstNode> {
    let token = tokens.pop_front().unwrap();
    let (indicator, body) = (token.chars().nth(0).unwrap(), &token[1..]);
    if indicator == 'T' {
        return Box::new(AstBoolean::new(true));
    }
    if indicator == 'F' {
        return Box::new(AstBoolean::new(false));
    }
    if indicator == 'I' {
        return Box::new(AstInteger::new(translator::icfp_to_int(body)));
    }
    if indicator == 'S' {
        return Box::new(AstString::new(&translator::icfp_to_ascii(body)));
    }
    if indicator == 'U' {
        let operator = body.chars().nth(0).unwrap();
        let operand = build_ast(tokens);
        return Box::new(AstUnaryOperator::new(operator, operand));
    }
    if indicator == 'B' {
        let operator = body.chars().nth(0).unwrap();
        let left = build_ast(tokens);
        let right = build_ast(tokens);
        return if operator == '$' {
            Box::new(AstEvaluator::new(left, right))
        } else {
            Box::new(AstBinaryOperator::new(operator, left, right))
        };
    }
    if indicator == '?' {
        let condition = build_ast(tokens);
        let true_branch = build_ast(tokens);
        let false_branch = build_ast(tokens);
        return Box::new(AstIf::new(condition, true_branch, false_branch));
    }
    if indicator == 'L' {
        let id = translator::icfp_to_int(body) as usize;
        let definition = build_ast(tokens);
        return Box::new(AstLambdaAbstraction::new(id, definition));
    }
    if indicator == 'v' {
        let id = translator::icfp_to_int(body) as usize;
        return Box::new(AstVariable::new(id));
    }

    eprintln!("Fail to convert a token: {}", token);
    Box::new(AstNone::new())
}
