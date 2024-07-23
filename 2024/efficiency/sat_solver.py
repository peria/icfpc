#!/usr/bin/env python3

import argparse
import sys
import re
import z3


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--sudoku',
        action='store_true', dest='sudoku')
    args = parser.parse_args()

    lines = []
    for line in sys.stdin:
        lines.append(line)
    sat = ''.join(lines).strip()
    solve(sat, args.sudoku)


def solve(sat, sudoku=False):
    solver, vars = parse(sat, sudoku)
    if solver.check() == z3.unsat:
        print('Fail. Unsatisfied.', file=sys.stderr)
    assignments = fullfill(solver, vars, sudoku)
    print(assignments)
    number = 0
    if sudoku:
        for v in vars:
            digit = assignments[str(v)] - 1
            number = number * 9 + digit
    else:
        for v in reversed(vars):
            bit = assignments[str(v)]
            number = number * 2 + bit
    print('Answer: ', number)


def parse(sat, sudoku):
    def txt2var(txt):
        name = txt[1:] if txt[0] == '!' else txt
        if name[0] != 'x':
            return int(name)
        for v in vars:
            if str(v) == name:
                return z3.Not(v) if txt[0] == '!' and not sudoku else v
        return None

    terms = [re.sub('[^!|=x0123456789]', '', x.strip())
             for x in sat.split('&')]

    vars = []
    for t in terms:
        splitter = '==' if sudoku else '!'
        vs = re.sub('!', '', t).split(splitter)
        for v in vs:
            if v[0] == 'x':
                vars.append(v)
    vars = list(set(vars))
    vars.sort(key=lambda x: int(x[1:]))
    if sudoku:
        vars = [z3.Int(v) for v in vars]
    else:
        vars = [z3.Bool(v) for v in vars]

    solver = z3.Solver()
    if sudoku:
        for x in vars:
            ps = z3.Or([x == v for v in (1, 2, 3, 4, 5, 6, 7, 8, 9)])
            solver.add(ps)
        for t in terms:
            vs = list(map(txt2var, t.split('==')))
            solver.add(vs[0] != vs[1] if t[0] == '!' else vs[0] == vs[1])
    else:
        for t in terms:
            vs = list(map(txt2var, t.split('|')))
            ps = z3.Or(vs)
            solver.add(ps)

    return solver, vars


def fullfill(solver, vars, sudoku):
    values = (1, 2, 3, 4, 5, 6, 7, 8, 9) if sudoku else (False, True)
    assign = {}
    index = 0
    for x in vars:
        a = None
        for v in values:
            solver.push()
            solver.add(x == v)
            if solver.check() == z3.sat:
                a = v
                solver.pop(1)
                break
            solver.pop(1)
        if a is None:
            print(f"Error for {x}")
            return None
        solver.add(x == a)
        if sudoku:
            assign[str(x)] = a
        else:
            assign[str(x)] = 1 if a else 0
    return assign


if __name__ == '__main__':
    main()
