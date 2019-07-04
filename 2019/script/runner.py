#!/usr/bin/python3

import argparse
import concurrent
import os
import subprocess
import sys
import time


_ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '../'))
_DEFAULT_DESC_DIR = os.path.join(_ROOT_DIR, 'problems')
_DEFAULT_SOL_BASE_DIR = os.path.join(_ROOT_DIR, 'solutions')

TIMEOUT = 60  # sec
INFINITY = 1e+9


def parse_arguments():
    parser = argparse.ArgumentParser(description='Runs a solver for all desc files.')
    parser.add_argument('solver', help='binary to run',
                        type=str)
    parser.add_argument('--desc-dir',
                        help='path to the directory that contains .desc files',
                        default=_DEFAULT_DESC_DIR)
    parser.add_argument('--sol-dir',
                        help='path to the directory to output sol files and used buy files')
    parser.add_argument('--buy-dir',
                        help='path to the directory that contains .buy files')
    parser.add_argument('--json',
                        help='path to a JSON file that contains all solvers\' results') 

    args = parser.parse_args()
    if args.sol_dir is None:
        solver_name = os.path.basename(args.solver)
        args.sol_dir = os.path.join(_DEFAULT_SOL_BASE_DIR, solver_name)

    return args


def eval_solution(sol_file):
    if not os.path.isfile(sol_file):
        return INFINITY

    with open(sol_file, 'r') as f:
        solution = str(f.read()).strip()

    score = max([len([c for c in w if c.isupper()])
                 for w in solution.split('#')])
    return score


def run_solver(x, args):
    problem_id = 'prob-{:03d}'.format(x)
    desc_file = os.path.join(args.desc_dir, '{}.desc'.format(problem_id))
    sol_file = os.path.join(args.sol_dir, '{}.sol'.format(problem_id))
    # TODO: Support using .buy files

    command = [args.solver, '--desc', desc_file, '--sol', sol_file]
    start_time = time.time()
    try:
        process = subprocess.run(command, stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE, timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        print('Solver timeout for {problem_id}'.format(problem_id=problem_id), flush=True)
        return (problem_name, INFINITE)
    end_time = time.time()

    # In this game, smaller is better
    score = eval_solution(sol_file)
    print('{} {:5d} {:.2f}sec'.format(problem_id, score, end_time - start_time))
    return (problem_id, score)

    
def main():
    args = parse_arguments()

    os.makedirs(args.sol_dir, exist_ok=True)

    with concurrent.futures.ThreadPoolExecutor(max_workers=2) as executor:
        futures = [executor.submit(run_solver, i, args) for i in range(1, 301)]
    results = [future.result() for future in concurrent.futures.as_completed(futures)]
    results.sort(key=lambda x: x[0])

    # |total| doesn't make sense to evaluate totally, but can be a metric.
    total = sum(r[1] for r in results)
    print('Total: {:7d}'.format(total))

if __name__ == '__main__':
    main()
