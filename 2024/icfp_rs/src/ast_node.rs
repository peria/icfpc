use crate::translator;
use std::any::Any;

type Integer = i64;

fn dump(level: usize, message: &str) {
    for _ in 0..level {
        eprint!("! ");
    }
    eprintln!("{}", message);
}

pub trait AstNode {
    fn as_any(&self) -> &dyn Any;
    fn clone_box(&self) -> Box<dyn AstNode>;
    fn dump(&self, level: usize);
    fn to_string(&self) -> String;
    fn evaluate(self: Box<Self>) -> Box<dyn AstNode>;
    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode>;
}

fn is_same<T: ?Sized>(left: &Box<T>, right: &Box<T>) -> bool {
    let left: *const T = left.as_ref();
    let right: *const T = right.as_ref();
    left == right
}

pub struct AstBoolean {
    value: bool,
}

impl AstNode for AstBoolean {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(self.value))
    }

    fn dump(&self, level: usize) {
        let value = if self.value { "True" } else { "False" };
        dump(level, &format!("Boolean({})", value));
    }

    fn to_string(&self) -> String {
        if self.value {
            "True".to_string()
        } else {
            "False".to_string()
        }
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        self
    }

    fn apply(self: Box<Self>, _id: usize, _value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        self
    }
}

impl AstBoolean {
    pub fn new(value: bool) -> Self {
        AstBoolean { value }
    }
}

pub struct AstInteger {
    value: Integer,
}

impl AstNode for AstInteger {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(self.value))
    }

    fn dump(&self, level: usize) {
        dump(level, &format!("Integer({})", self.value));
    }

    fn to_string(&self) -> String {
        format!("{}", self.value)
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        self
    }

    fn apply(self: Box<Self>, _id: usize, _value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        self
    }
}

impl AstInteger {
    pub fn new(value: Integer) -> Self {
        AstInteger { value }
    }
}

pub struct AstString {
    value: String,
}

impl AstNode for AstString {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(&self.value))
    }

    fn dump(&self, level: usize) {
        dump(level, &format!("String(\"{}\")", &self.value));
    }

    fn to_string(&self) -> String {
        format!("\"{}\"", self.value)
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        self
    }

    fn apply(self: Box<Self>, _id: usize, _value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        self
    }
}

impl AstString {
    pub fn new(value: &str) -> Self {
        AstString {
            value: value.to_string(),
        }
    }
}

pub struct AstUnaryOperator {
    operator: char,
    operand: Box<dyn AstNode>,
}

impl AstNode for AstUnaryOperator {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(self.operator, self.operand.clone_box()))
    }

    fn dump(&self, level: usize) {
        dump(level, &format!("UnaryOperator({})", self.operator));
        self.operand.dump(level + 1);
    }

    fn to_string(&self) -> String {
        format!("{}({})", self.operator, &self.operand.to_string())
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        let operator = self.operator;
        let operand = self.operand.evaluate();
        if operator == '-' {
            if let Some(x) = operand.as_any().downcast_ref::<AstInteger>() {
                return Box::new(AstInteger::new(-x.value));
            }
        }
        if operator == '!' {
            if let Some(x) = operand.as_any().downcast_ref::<AstBoolean>() {
                return Box::new(AstBoolean::new(!x.value));
            }
        }
        if operator == '#' {
            if let Some(x) = operand.as_any().downcast_ref::<AstString>() {
                return Box::new(AstInteger::new(translator::ascii_to_int(&x.value)));
            }
        }
        if operator == '$' {
            if let Some(x) = operand.as_any().downcast_ref::<AstInteger>() {
                return Box::new(AstString::new(&translator::int_to_ascii(x.value)));
            }
        }
        Box::new(AstUnaryOperator::new(self.operator, operand))
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        let operand = self.operand.apply(id, value);
        Box::new(AstUnaryOperator::new(self.operator, operand))
    }
}

impl AstUnaryOperator {
    pub fn new(operator: char, operand: Box<dyn AstNode>) -> Self {
        AstUnaryOperator { operator, operand }
    }
}

pub struct AstBinaryOperator {
    operator: char,
    lhs: Box<dyn AstNode>,
    rhs: Box<dyn AstNode>,
}

impl AstNode for AstBinaryOperator {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(
            self.operator,
            self.lhs.clone_box(),
            self.rhs.clone_box(),
        ))
    }

    fn dump(&self, level: usize) {
        dump(level, &format!("BinaryOperator({})", self.operator));
        self.lhs.dump(level + 1);
        self.rhs.dump(level + 1);
    }

    fn to_string(&self) -> String {
        format!(
            "({} {} {})",
            &self.lhs.to_string(),
            self.operator,
            &self.rhs.to_string()
        )
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        let operator = self.operator;
        let lhs = self.lhs.evaluate();
        let rhs = self.rhs.evaluate();
        if operator == '+' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    return Box::new(AstInteger::new(x.value + y.value));
                }
            }
        }
        if operator == '-' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    return Box::new(AstInteger::new(x.value - y.value));
                }
            }
        }
        if operator == '*' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    return Box::new(AstInteger::new(x.value * y.value));
                }
            }
        }
        if operator == '/' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    let mut q = x.value.abs() / y.value.abs();
                    if x.value * y.value < 0 {
                        q = -q;
                    }
                    return Box::new(AstInteger::new(q));
                }
            }
        }
        if operator == '%' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    let mut q = x.value.abs() / y.value.abs();
                    if x.value * y.value < 0 {
                        q = -q;
                    }
                    return Box::new(AstInteger::new(x.value - q * y.value));
                }
            }
        }
        if operator == '<' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    return Box::new(AstBoolean::new(x.value < y.value));
                }
            }
        }
        if operator == '>' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    return Box::new(AstBoolean::new(x.value > y.value));
                }
            }
        }
        if operator == '=' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstInteger>() {
                    return Box::new(AstBoolean::new(x.value == y.value));
                }
            }
        }
        if operator == '=' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstBoolean>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstBoolean>() {
                    return Box::new(AstBoolean::new(x.value == y.value));
                }
            }
        }
        if operator == '=' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstString>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstString>() {
                    return Box::new(AstBoolean::new(x.value == y.value));
                }
            }
        }
        if operator == '|' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstBoolean>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstBoolean>() {
                    return Box::new(AstBoolean::new(x.value || y.value));
                }
            }
        }
        if operator == '&' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstBoolean>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstBoolean>() {
                    return Box::new(AstBoolean::new(x.value && y.value));
                }
            }
        }
        if operator == '.' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstString>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstString>() {
                    let value = x.value.clone() + &y.value;
                    return Box::new(AstString::new(&value));
                }
            }
        }
        if operator == 'T' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstString>() {
                    return Box::new(AstString::new(&y.value[..(x.value as usize)]));
                }
            }
        }
        if operator == 'D' {
            if let Some(x) = lhs.as_any().downcast_ref::<AstInteger>() {
                if let Some(y) = rhs.as_any().downcast_ref::<AstString>() {
                    return Box::new(AstString::new(&y.value[(x.value as usize)..]));
                }
            }
        }
        Box::new(AstBinaryOperator::new(self.operator, lhs, rhs))
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        let lhs = self.lhs.apply(id, value);
        let rhs = self.rhs.apply(id, value);
        Box::new(AstBinaryOperator::new(self.operator, lhs, rhs))
    }
}

impl AstBinaryOperator {
    pub fn new(operator: char, lhs: Box<dyn AstNode>, rhs: Box<dyn AstNode>) -> Self {
        AstBinaryOperator { operator, lhs, rhs }
    }
}

pub struct AstEvaluator {
    lhs: Box<dyn AstNode>,
    rhs: Box<dyn AstNode>,
}

impl AstNode for AstEvaluator {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn dump(&self, level: usize) {
        dump(level, "Evaluate");
        self.lhs.dump(level + 1);
        self.rhs.dump(level + 1);
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(self.lhs.clone_box(), self.rhs.clone_box()))
    }

    fn to_string(&self) -> String {
        format!(
            "Eval <{}> ({})",
            &self.lhs.to_string(),
            &self.rhs.to_string()
        )
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        let lhs = self.lhs;
        if let Some(x) = lhs.as_any().downcast_ref::<AstLambdaAbstraction>() {
            let id = x.id;
            return x.definition.clone_box().apply(id, &self.rhs);
        }
        let lhs = lhs.evaluate();
        return Box::new(AstEvaluator::new(lhs, self.rhs));
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        let lhs = self.lhs.apply(id, value);
        let rhs = self.rhs.apply(id, value);
        Box::new(AstEvaluator::new(lhs, rhs))
    }
}

impl AstEvaluator {
    pub fn new(lhs: Box<dyn AstNode>, rhs: Box<dyn AstNode>) -> Self {
        AstEvaluator { lhs, rhs }
    }
}

pub struct AstIf {
    condition: Box<dyn AstNode>,
    true_branch: Box<dyn AstNode>,
    false_branch: Box<dyn AstNode>,
}

impl AstNode for AstIf {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(
            self.condition.clone_box(),
            self.true_branch.clone_box(),
            self.false_branch.clone_box(),
        ))
    }

    fn dump(&self, level: usize) {
        dump(level, "IF");
        self.condition.dump(level + 1);
        dump(level, "THEN");
        self.true_branch.dump(level + 1);
        dump(level, "ELSE");
        self.false_branch.dump(level + 1);
    }

    fn to_string(&self) -> String {
        format!(
            "\nIF {}\n  {}\nELSE  {}",
            &self.condition.to_string(),
            &self.true_branch.to_string(),
            &self.false_branch.to_string()
        )
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        let condition = self.condition.evaluate();
        if let Some(c) = condition.as_any().downcast_ref::<AstBoolean>() {
            if c.value {
                return self.true_branch.evaluate();
            } else {
                return self.false_branch.evaluate();
            }
        }
        Box::new(AstIf::new(condition, self.true_branch, self.false_branch))
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        let condition = self.condition.apply(id, value);
        if let Some(c) = condition.as_any().downcast_ref::<AstBoolean>() {
            if c.value {
                return self.true_branch.apply(id, value);
            } else {
                return self.false_branch.apply(id, value);
            }
        }

        let true_branch = self.true_branch.apply(id, value);
        let false_branch = self.false_branch.apply(id, value);
        Box::new(AstIf::new(condition, true_branch, false_branch))
    }
}

impl AstIf {
    pub fn new(
        condition: Box<dyn AstNode>,
        true_branch: Box<dyn AstNode>,
        false_branch: Box<dyn AstNode>,
    ) -> Self {
        AstIf {
            condition,
            true_branch,
            false_branch,
        }
    }
}

pub struct AstLambdaAbstraction {
    pub id: usize,
    pub definition: Box<dyn AstNode>,
}

impl AstNode for AstLambdaAbstraction {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(self.id, self.definition.clone_box()))
    }

    fn dump(&self, level: usize) {
        dump(level, &format!("Lambda \\x{}", self.id));
        self.definition.dump(level + 1);
    }

    fn to_string(&self) -> String {
        format!("\\x{} ({})", self.id, &self.definition.to_string())
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        let definition = self.definition.evaluate();
        Box::new(AstLambdaAbstraction::new(self.id, definition))
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        // Do not apply `id` more.
        if id == self.id {
            return self;
        }
        let definition = self.definition.apply(id, value);
        Box::new(AstLambdaAbstraction::new(self.id, definition))
    }
}

impl AstLambdaAbstraction {
    pub fn new(id: usize, definition: Box<dyn AstNode>) -> Self {
        AstLambdaAbstraction { id, definition }
    }
}

pub struct AstVariable {
    id: usize,
}

impl AstNode for AstVariable {
    fn as_any(&self) -> &dyn Any {
        self
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        Box::new(Self::new(self.id))
    }

    fn dump(&self, level: usize) {
        dump(level, &format!("\\x{}", self.id));
    }

    fn to_string(&self) -> String {
        format!("x{}", self.id)
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        self
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        if id == self.id {
            return value.clone_box();
        }
        self
    }
}

impl AstVariable {
    pub fn new(id: usize) -> Self {
        AstVariable { id }
    }
}

pub struct AstNone {}

impl AstNode for AstNone {
    fn as_any(&self) -> &dyn Any {
        todo!()
    }

    fn clone_box(&self) -> Box<dyn AstNode> {
        todo!()
    }

    fn dump(&self, level: usize) {
        todo!()
    }

    fn to_string(&self) -> String {
        "NONE".to_string()
    }

    fn evaluate(self: Box<Self>) -> Box<dyn AstNode> {
        todo!()
    }

    fn apply(self: Box<Self>, id: usize, value: &Box<dyn AstNode>) -> Box<dyn AstNode> {
        todo!()
    }
}

impl AstNone {
    pub fn new() -> Self {
        AstNone {}
    }
}
