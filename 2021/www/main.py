#!/usr/bin/python3

import flask
import json
import os
import math
import copy
import visualize

ROOT_DIR = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))  # 2021/
WWW_DIR = os.path.join(ROOT_DIR, 'www')
DATA_DIR = os.path.join(ROOT_DIR, 'data')
PROBLEMS_DIR = os.path.join(DATA_DIR, 'problems')
SOLUTIONS_DIR = os.path.join(ROOT_DIR, 'solutions')

MAX_PROBLEM_ID = 132

app = flask.Flask(__name__)


@app.route('/')
def index():
    problems = get_problems()

    context = {
        'title': 'Problems',
        'problems': [get_problem_context(problem) for problem in problems],
    }
    context.update(get_common_context())
    return flask.render_template('index.html', **context)


def get_problems():
    '''Load problems and add other informmation on problems'''
    def get_problem(id):
        json_filepath = os.path.join(PROBLEMS_DIR, '{}.problem.json'.format(id))
        with open(json_filepath, 'r') as f:
            problem = json.load(f)
        # Add some more information
        problem['id'] = id
        problem['score'] = get_score(problem)
        return problem

    problems = [get_problem(id) for id in range(1, MAX_PROBLEM_ID + 1)]
    return problems


def get_problem_context(problem):
    '''Set up jinja context for a problem.'''
    solutions = get_solutions(problem['id'])
    problem['solutions'] = [get_solution_context(problem, solution) for solution in solutions]
    return problem


def get_solutions(id):
    '''Load solutions information and add additional information'''
    def get_solution(id, bonus=None):
        json_filepath = os.path.join(SOLUTIONS_DIR, '{}.pose.json'.format(id))
        if not os.path.exists(json_filepath):
            return None
        with open(json_filepath, 'r') as f:
            solution = json.load(f)
        # Add some more information
        solution['id'] = id
        solution['json_path'] = json_filepath
        return solution

    # TODO: Iterate over bonuses
    solutions = [get_solution(id)]
    return solutions


def get_solution_context(problem, solution):
    if solution is None:
        return None
    id = problem['id']
    img_filepath = os.path.join(WWW_DIR, 'static', 'images', 'solutions', '{}.pose.png'.format(id))
    json_filepath = solution['json_path']
    if (not os.path.exists(img_filepath) or os.path.getctime(json_filepath) > os.path.getctime(img_filepath)):
        visualize.visualize(problem, solution, img_filepath)
    context = copy.deepcopy(solution)
    context.update({
        'img_src':'/static/images/solutions/{}.pose.png'.format(id),
    })
    return context


def get_common_context():
    navigations = [
        { 'path': path, 'title': title } for path, title in [
            ['/', 'Dashboard'],
            ['https://github.com/peria/icfpc/tree/master/2021', 'Repository'],
            ['https://icfpcontest2021.github.io/', 'Contest'],
            ['https://poses.live/problems', 'Problems'],
            ['https://poses.live/teams', 'Ranking'],
        ]
    ]

    return {
        'g_navigations': navigations,
    }


def get_score(problem):
    vertices = len(problem['figure']['vertices'])
    edges = len(problem['figure']['edges'])
    hole = len(problem['hole'])
    base = 1000 * math.log2(vertices * edges * hole / 6.0)
    return int(math.ceil(base))

if __name__ == "__main__":
    app.run(debug=True)