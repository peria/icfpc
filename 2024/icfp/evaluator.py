#!/usr/bin/env python3

from collections import deque
import copy
import sys
import time
import translator
import unittest

sys.setrecursionlimit(100000)


def evaluate(message, dump=0):
    try:
        node = evaluate_core(message, dump)
        if isinstance(node, AstString):
            return node.value
    except AssertionError as e:
        _e_type, _e_obj, trace = sys.exc_info()
        filename = trace.tb_frame.f_code.co_filename
        line_number = trace.tb_lineno
        print(
            f'Failed to evaluate at {filename}({line_number}). Return the original program.\n{e}', file=sys.stderr)
        raise
        return message
    print(f'Value type is {type(node)}', file=sys.stderr)
    return str(node)


def evaluate_core(message, dump):
    MAX_LOOP = 10000
    tokens = deque(message.split(' '))
    root = build_ast(tokens)
    if dump == 1:
        print(f'{root}\n', file=sys.stderr)
    elif dump == 2:
        root.dump(0)

    for i in range(MAX_LOOP):
        new_root = root.evaluate()
        if isinstance(new_root, AstString) or new_root == root:
            return new_root
        root = new_root
        if dump == 2:
            root.dump(0)
            # print(root, file=sys.stderr)
            time.sleep(1)

    print(f'Can\'t evaluate in {MAX_LOOP} loops.', file=sys.stderr)
    return root


def build_ast(tokens):
    assert isinstance(tokens, deque)
    assert len(tokens) > 0
    token = tokens.popleft()
    indicator, body = token[0], token[1:]
    if indicator == 'T':
        return AstBoolean(True)
    if indicator == 'F':
        return AstBoolean(False)
    if indicator == 'I':
        return AstInteger(translator.icfp_to_int(body))
    if indicator == 'S':
        return AstString(translator.icfp_to_ascii(body))
    if indicator == 'U':
        operand = build_ast(tokens)
        return AstUnaryOperator(body, operand)
    if indicator == 'B':
        left = build_ast(tokens)
        right = build_ast(tokens)
        if body == '$':
            return AstEvaluator(left, right)
        return AstBinaryOperator(body, left, right)
    if indicator == '?':
        condition = build_ast(tokens)
        true_branch = build_ast(tokens)
        false_branch = build_ast(tokens)
        return AstIf(condition, true_branch, false_branch)
    if indicator == 'L':
        id = translator.icfp_to_int(body)
        definition = build_ast(tokens)
        return AstLambdaAbstraction(id, definition)
    if indicator == 'v':
        id = translator.icfp_to_int(body)
        return AstVariable(id)
    print(f'Fail to convert a token: {token}', file=sys.stderr)
    return None


class AstNode(object):
    def __init__(self):
        pass

    def _dump(self, level, text):
        print('| ' * level + text, file=sys.stderr)

    def dump(self, level):
        raise NotImplementedError(f'{type(self)}.dump() is not imlemented')

    def evaluate(self):
        raise NotImplementedError(f'{type(self)}.evaluate() is not imlemented')

    def apply(self, _id, _value):
        raise NotImplementedError(f'{type(self)}.apply() is not imlemented')


class AstBoolean(AstNode):
    def __init__(self, value):
        assert isinstance(value, bool)
        self.value = value

    def dump(self, level):
        self._dump(level, f'Boolean({self.value})')

    def evaluate(self):
        return self

    def apply(self, _id, _value):
        return self


class AstInteger(AstNode):
    def __init__(self, value):
        assert isinstance(value, int)
        self.value = value

    def __str__(self):
        return f'I{self.value}'

    def dump(self, level):
        self._dump(level, f'Integer({self.value})')

    def evaluate(self):
        return self

    def apply(self, _id, _value):
        return self


class AstString(AstNode):
    def __init__(self, value):
        # `body` is ASCII string.
        assert isinstance(value, str)
        self.value = value

    def dump(self, level):
        self._dump(level, f'String("{self.value}")')

    def evaluate(self):
        return self

    def apply(self, _id, _value):
        return self


class AstUnaryOperator(AstNode):
    def __init__(self, operator, operand):
        assert operator in '-!#$'
        assert isinstance(operand, AstNode)
        self.operator = operator
        self.operand = operand

    def dump(self, level):
        self._dump(level, f'UnaryOperator {self.operator}')
        self.operand.dump(level + 1)

    def evaluate(self):
        operator = self.operator
        operand = self.operand.evaluate()
        if operator == '-' and isinstance(operand, AstInteger):
            return AstInteger(-operand.value)
        if operator == '!' and isinstance(operand, AstBoolean):
            return AstBoolean(not operand.value)
        if operator == '#' and isinstance(operand, AstString):
            return AstInteger(translator.ascii_to_int(operand.value))
        if operator == '$' and isinstance(operand, AstInteger):
            return AstString(translator.int_to_ascii(operand.value))
        if operand != self.operand:
            return AstUnaryOperator(self.operator, operand)
        return self

    def apply(self, id, value):
        operand = self.operand.apply(id, value)
        if operand != self.operand:
            return AstUnaryOperator(self.operator, operand)
        return self


class AstBinaryOperator(AstNode):
    def __init__(self, operator, lhs, rhs):
        assert operator in '+-*/%<>=|&.TD'
        assert isinstance(
            lhs, AstNode), f'lhs {type(lhs)} is not AST node for {operator}'
        assert isinstance(
            rhs, AstNode), f'rhs {type(rhs)} is not AST node for {operator}'
        self.operator = operator
        self.lhs = lhs
        self.rhs = rhs

    def __str__(self):
        return f'{self.lhs} {self.operator} {self.rhs}'

    def dump(self, level):
        self._dump(level, f'Binary Operator {self.operator}')
        self.lhs.dump(level + 1)
        self.rhs.dump(level + 1)

    def evaluate(self):
        operator = self.operator
        lhs = self.lhs.evaluate()
        rhs = self.rhs.evaluate()
        if operator == '+' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            return AstInteger(lhs.value + rhs.value)
        if operator == '-' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            return AstInteger(lhs.value - rhs.value)
        if operator == '*' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            return AstInteger(lhs.value * rhs.value)
        if operator == '/' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            q = abs(lhs.value) // abs(rhs.value)
            if lhs.value * rhs.value < 0:
                q = -q
            return AstInteger(q)
        if operator == '%' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            q = abs(lhs.value) // abs(rhs.value)
            if lhs.value * rhs.value < 0:
                q = -q
            return AstInteger(lhs.value - q * rhs.value)
        if operator == '<' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            return AstBoolean(lhs.value < rhs.value)
        if operator == '>' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            return AstBoolean(lhs.value > rhs.value)
        if operator == '=' and isinstance(lhs, AstInteger) and isinstance(rhs, AstInteger):
            return AstBoolean(lhs.value == rhs.value)
        if operator == '=' and isinstance(lhs, AstBoolean) and isinstance(rhs, AstBoolean):
            return AstBoolean(lhs.value == rhs.value)
        if operator == '=' and isinstance(lhs, AstString) and isinstance(rhs, AstString):
            return AstBoolean(lhs.value == rhs.value)
        if operator == '|' and isinstance(lhs, AstBoolean) and isinstance(rhs, AstBoolean):
            return AstBoolean(lhs.value or rhs.value)
        if operator == '&' and isinstance(lhs, AstBoolean) and isinstance(rhs, AstBoolean):
            return AstBoolean(lhs.value and rhs.value)
        if operator == '.' and isinstance(lhs, AstString) and isinstance(rhs, AstString):
            return AstString(lhs.value + rhs.value)
        if operator == 'T' and isinstance(lhs, AstInteger) and isinstance(rhs, AstString):
            return AstString(rhs.value[:lhs.value])
        if operator == 'D' and isinstance(lhs, AstInteger) and isinstance(rhs, AstString):
            return AstString(rhs.value[lhs.value:])
        if lhs != self.lhs or rhs != self.rhs:
            return AstBinaryOperator(self.operator, lhs, rhs)
        return self

    def apply(self, id, value):
        lhs = self.lhs.apply(id, value)
        rhs = self.rhs.apply(id, value)
        if lhs != self.lhs or rhs != self.rhs:
            return AstBinaryOperator(self.operator, lhs, rhs)
        return self


class AstEvaluator(AstNode):
    def __init__(self, lhs, rhs):
        assert isinstance(lhs, AstNode)
        assert isinstance(rhs, AstNode)
        self.lhs = lhs
        self.rhs = rhs

    def __str__(self):
        return f'EVAL ({self.lhs}) <{self.rhs}>'

    def dump(self, level):
        self._dump(level, f'Evaluate')
        self.lhs.dump(level + 1)
        self.rhs.dump(level + 1)

    def evaluate(self):
        lhs = self.lhs
        if isinstance(lhs, AstLambdaAbstraction):
            return lhs.definition.apply(lhs.id, self.rhs)
        lhs = lhs.evaluate()
        if lhs != self.lhs:
            return AstEvaluator(lhs, self.rhs)
        return self

    def apply(self, id, value):
        lhs = self.lhs.apply(id, value)
        rhs = self.rhs.apply(id, value)
        if lhs != self.lhs or rhs != self.rhs:
            return AstEvaluator(lhs, rhs)
        return self


class AstIf(AstNode):
    def __init__(self, condition, true_branch, false_branch):
        assert isinstance(condition, AstNode)
        assert isinstance(true_branch, AstNode)
        assert isinstance(false_branch, AstNode)
        self.condition = condition
        self.true_branch = true_branch
        self.false_branch = false_branch

    def __str__(self):
        return f'\nIF {self.condition}\nTHEN {self.true_branch}\nELSE {self.false_branch}'

    def dump(self, level):
        self._dump(level, f'IF')
        self.condition.dump(level + 1)
        self._dump(level, f'THEN')
        self.true_branch.dump(level + 1)
        self._dump(level, f'ELSE')
        self.false_branch.dump(level + 1)

    def evaluate(self):
        condition = self.condition.evaluate()
        if isinstance(condition, AstBoolean):
            if condition.value:
                return self.true_branch.evaluate()
            else:
                return self.false_branch.evaluate()
        if condition != self.condition:
            return AstIf(condition, self.true_branch, self.false_branch)
        return self

    def apply(self, id, value):
        condition = self.condition.apply(id, value)
        if isinstance(condition, AstBoolean):
            if condition.value:
                return self.true_branch.apply(id, value)
            else:
                return self.false_branch.apply(id, value)

        true_branch = self.true_branch.apply(id, value)
        false_branch = self.false_branch.apply(id, value)
        if condition != self.condition or true_branch != self.true_branch or false_branch != self.false_branch:
            return AstIf(condition, true_branch, false_branch)
        return self


class AstLambdaAbstraction(AstNode):
    def __init__(self, id, definition):
        assert isinstance(id, int)
        assert isinstance(definition, AstNode)
        self.id = id
        self.definition = definition

    def __str__(self):
        return f'LAMBDA x{self.id} {self.definition}'

    def dump(self, level):
        self._dump(level, f'Lambda x{self.id}')
        self.definition.dump(level + 1)

    def evaluate(self):
        definition = self.definition.evaluate()
        if definition != self.definition:
            return AstLambdaAbstraction(self.id, definition)
        return self

    def apply(self, id, value):
        # Do not apply `id` more.
        if id == self.id:
            return self
        definition = self.definition.apply(id, value)
        if definition != self.definition:
            return AstLambdaAbstraction(self.id, definition)
        return self


class AstVariable(AstNode):
    def __init__(self, id):
        assert isinstance(id, int)
        self.id = id

    def __str__(self):
        return f'x{self.id}'

    def dump(self, level):
        self._dump(level, f'\\x{self.id}')

    def evaluate(self):
        return self

    def apply(self, id, value):
        if id == self.id:
            return copy.copy(value)
        return self


class TestBaseICFP(unittest.TestCase):
    def test_evaluate_units(self):
        data = [
            ("T", AstBoolean, "True"),  # Boolean
            ("F", AstBoolean, "False"),
            ("I/6", AstInteger, "1337"),  # Integer
            ("SB%,,/}Q/2,$_", AstString, "Hello World!"),  # String
            ("U- I$", AstInteger, "-3"),  # Unary operator
            ("U! T", AstBoolean, "False"),
            ("U# S4%34", AstInteger, "15818151"),
            ("U$ I4%34", AstString, "test"),
            ("B+ I# I$", AstInteger, "5"),  # Binary operator
            ("B- I$ I#", AstInteger, "1"),
            ("B* I$ I#", AstInteger, "6"),
            ("B/ U- I( I#", AstInteger, "-3"),
            ("B% U- I( I#", AstInteger, "-1"),
            ("B< I$ I#", AstBoolean, "False"),
            ("B> I$ I#", AstBoolean, "True"),
            ("B= I$ I#", AstBoolean, "False"),
            ("B| T F", AstBoolean, "True"),
            ("B& T F", AstBoolean, "False"),
            ("B. S4% S34", AstString, "test"),
            ("BT I$ S4%34", AstString, "tes"),
            ("BD I$ S4%34", AstString, "t"),
            ("B$ L! B+ v! v! I#", AstInteger, "4"),  # (v0 => v0 + v0)(2) == 4
        ]
        for icfp, expect_type, expect_value in data:
            actual = evaluate_core(icfp)
            self.assertEqual(type(actual), expect_type)
            self.assertEqual(str(actual.value), expect_value)

    def test_evaluation(self):
        icfp = 'B$ B$ L# L$ v# B. SB%,,/ S}Q/2,$_ IK'
        actual = evaluate(icfp)
        self.assertEqual(actual, "Hello World!")

    def test_evaluation2(self):
        icfp = 'B$ L# B$ L" B+ v" v" B* I$ I# v8'
        actual = evaluate(icfp)
        self.assertEqual(actual, "12")

    def test_simple_language_test(self):
        icfp = 'B$ B$ B$ B$ L$ L$ L$ L# v$ I" I# I$ I%'
        actual = evaluate(icfp)
        self.assertEqual(actual, "3")


if __name__ == '__main__':
    unittest.main()
