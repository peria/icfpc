#!/usr/bin/python3

import flask
import json
import os
import math

ROOT_DIR = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))  # 2021/
DATA_DIR = os.path.join(ROOT_DIR, 'data')

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
        json_filepath = os.path.join(DATA_DIR, 'problems', '{}.problem.json'.format(id))
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
    return problem


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