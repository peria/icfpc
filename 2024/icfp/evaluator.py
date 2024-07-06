#!/usr/bin/env python3

from collections import deque
import sys
import translator


def evaluate(message):
    tokens = deque(message.split(' '))
    root = build_ast(tokens)
    result = root.evaluate()
    if isinstance(result, AstString):
        return result.value
    return result


def build_ast(tokens):
    assert isinstance(tokens, deque)
    assert len(tokens) > 0
    token = tokens.pop()
    indicator, body = token[0], token[1:]
    if indicator == 'T':
        return AstBoolean(True)
    if indicator == 'F':
        return AstBoolean(False)
    if indicator == 'I':
        return AstInteger(body)
    if indicator == 'S':
        return AstString(body)
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
        id = icfp_to_int(body)
        return AstLambdaAbstraction(id)
    if indicator == 'v':
        id = icfp_to_int(body)
        return AstVariable(id)
    print(f'Fail to convert token: {token}', file=sys.stderr)
    return None


def icfp_to_int(icfp):
    value = 0
    for c in icfp:
        value = value * 94 + ord(c) - ord('!')
    return value


class AstNode(object):
    def __init__(self):
        pass

    def evaluate(self):
        raise NotImplementedError


class AstBoolean(AstNode):
    def __init__(self, value):
        assert isinstance(value, bool)
        self.value = value

    def evaluate(self):
        return self


class AstInteger(AstNode):
    def __init__(self, body):
        assert isinstance(body, str)
        self.value = icfp_to_int(body)

    def evaluate(self):
        return self


class AstString(AstNode):
    def __init__(self, body):
        # `body` is an ICFP string.
        assert isinstance(body, str)
        self.value = translator.icfp_to_ascii(body)

    def evaluate(self):
        return self


class AstUnaryOperator(AstNode):
    def __init__(self, operator, operand):
        assert operator in '-!#$'
        assert isinstance(operand, AstNode)
        self.operator = operator
        self.operand = operand

    def evaluate(self):
        operand = self.operand.evaluate()
        if self.operator == '-' and isinstance(operand, AstInteger):
            return AstInteger(-operand.value)
        if self.operator == '!' and isinstance(operand, AstBoolean):
            return AstBoolean(not operand.value)
        if operand != self.operand:
            return AstUnaryOperator(self.operator, operand)
        return self


class AstBinaryOperator(AstNode):
    def __init__(self, operator, lhs, rhs):
        assert operator in '+-*/%<>=|&.TD'
        assert isinstance(lhs, AstNode)
        assert isinstance(rhs, AstNode)
        self.operator = operator
        self.lhs = lhs
        self.rhs = rhs

    def evaluate(self):
        operator = self.operator
        lhs = self.lhs.evaluate()
        rhs = self.rhs.evaluate()
        if operator == '+' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            return AstInteger(lhs.value + rhs.value)
        if operator == '-' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            return AstInteger(lhs.value - rhs.value)
        if operator == '*' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            return AstInteger(lhs.value * rhs.value)
        if operator == '/' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            q = abs(lhs.value) / abs(rhs.value)
            if lhs.value * rhs.value < 0:
                q = -q
            return AstInteger(q)
        if operator == '%' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            q = abs(lhs.value) / abs(rhs.value)
            if lhs.value * rhs.value < 0:
                q = -q
            return AstInteger(lhs.value - q * rhs.value)
        if operator == '<' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            return AstBoolean(lhs.value < rhs.value)
        if operator == '>' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            return AstBoolean(lhs.value > rhs.value)
        if operator == '=' and isinstance(lhs, AstInteger) and isinstance(rhs,  AstInteger):
            return AstBoolean(lhs.value < rhs.value)
        if operator == '=' and isinstance(lhs, AstBoolean) and isinstance(rhs,  AstBoolean):
            return AstBoolean(lhs.value < rhs.value)
        if operator == '=' and isinstance(lhs, AstString) and isinstance(rhs,  AstString):
            return AstBoolean(lhs.value < rhs.value)
        if operator == '|' and isinstance(lhs, AstBoolean) and isinstance(rhs,  AstBoolean):
            return AstBoolean(lhs.value or rhs.value)
        if operator == '&' and isinstance(lhs, AstBoolean) and isinstance(rhs,  AstBoolean):
            return AstBoolean(lhs.value and rhs.value)
        if operator == '.' and isinstance(lhs, AstString) and isinstance(rhs, AstString):
            return AstString(lhs.value + rhs.value)
        if operator == 'T' and isinstance(lhs, AstInteger) and isinstance(rhs, AstString):
            return AstString(rhs.value[:lhs.value])
        if operator == 'D' and isinstance(lhs, AstInteger) and isinstance(rhs, AstString):
            return AstString(rhs.value[lhs.value:])
        return self


class AstEvaluator(AstNode):
    def __init__(self, lhs, rhs):
        assert isinstance(lhs, AstNode)
        assert isinstance(rhs, AstNode)
        self.lhs = lhs
        self.rhs = rhs

    def evaluate(self):
        raise NotImplementedError
        return self


class AstIf(AstNode):
    def __init__(self, condition, true_branch, false_branch):
        assert isinstance(condition, AstNode)
        assert isinstance(true_branch, AstNode)
        assert isinstance(false_branch, AstNode)
        self.condition = condition
        self.true_branch = true_branch
        self.false_branch = false_branch

    def evaluate(self):
        condition = self.condition.evaluate()
        if isinstance(condition, AstBoolean):
            if condition.value:
                return self.true_branch.evaluate()
            else:
                return self.false_branch.evaluate()
        return self


class AstLambdaAbstraction(AstNode):
    def __init__(self, id, definition):
        assert isinstance(id, int)
        self.id = id
        self.definition = definition

    def evaluate(self):
        raise NotImplementedError
        return self


class AstVariable(AstNode):
    def __init__(self, id):
        assert isinstance(id, int)
        self.id = id

    def evaluate(self):
        return self


if __name__ == '__main__':
    print('evaluator.py is not run from the command line.p')
