#!/usr/bin/python3

import argparse
import os
import subprocess

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))  # 2021/
PROBLEMS_DIR = os.path.join(ROOT_DIR, 'data', 'problems')
SOLUTIONS_DIR = os.path.join(ROOT_DIR, 'solutions')
EXEC_DIR = os.path.join(ROOT_DIR, 'src')


def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('problems_dir', default=PROBLEMS_DIR, help='Path to the directory of problem JSON files')
    parser.add_argument('solutions_dir', default=SOLUTIONS_DIR, help='Path to the directory to write solutions')
    return parser.parse_args()


def main():
    args = parse_arguments()
    solver_filepath = os.path.join(EXEC_DIR, 'solver')
    problems_dir = os.path.abspath(args.problems_dir)
    solutions_dir = os.path.abspath(args.solutions_dir)
    os.makedirs(solutions_dir, exist_ok=True)

    for filename in os.listdir(problems_dir):
        problem_filepath = os.path.join(problems_dir, filename)
        id, _, ext = filename.split('.')
        solution_filepath = os.path.join(solutions_dir, '{}.pose.json'.format(id))
        print('Try to run {}.'.format(id))
        try:
            subprocess.run([solver_filepath, problem_filepath, solution_filepath], timeout=10)
            print('Run {} is done.'.format(id))
        except subprocess.TimeoutExpired:
            print('Run {} is Time out.'.format(id))
            pass
        except:
            raise


if __name__ == '__main__':
    main()
