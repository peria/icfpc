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
    assignments = fullfill(solver, vars)
    print(assignments)
    number = 0
    for v in reversed(vars):
        bit = assignments[str(v)]
        number = number * 2 + bit
    print('Answer: ', number)


def parse(sat, sudoku):
    def txt2var(txt):
        name = txt[1:] if txt[0] == '!' else txt
        for v in vars:
            if str(v) == name:
                return z3.Not(v) if txt[0] == '!' else v
        return None

    terms = [re.sub('[^!|x0123456789]', '', x.strip())
             for x in sat.split('&')]

    vars = []
    for t in terms:
        vs = re.sub('!', '', t).split('|')
        vars.extend(vs)
    vars = list(set(vars))
    vars.sort(key=lambda x: int(x[1:]))
    if sudoku:
        vars = [z3.Int(v) for v in vars]
    else:
        vars = [z3.Bool(v) for v in vars]

    solver = z3.Solver()
    for t in terms:
        vs = list(map(txt2var, t.split('|')))
        ps = z3.Or(vs)
        solver.add(ps)

    return solver, vars


def fullfill(solver, vars):
    assign = {}
    for x in vars:
        a = None
        for v in (False, True):
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
        assign[str(x)] = 1 if a else 0
    return assign


if __name__ == '__main__':
    main()
