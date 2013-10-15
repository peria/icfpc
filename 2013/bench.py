#!/usr/bin/env python

import json
import random
import time

import api

def random_input(size):
  ret = []
  for i in xrange(size):
    ret.append(random.randint(0, 0xffffffffffffffff))
  return map((lambda x: hex(x).rstrip('L')), ret)


def main():
  j = 0
  for problem in open('trials/middle.tsv', 'r'):
    id, size, operators, answer = problem.split('\t')
    operators = operators.split(',')
    
    # assert(len(problem) == 4)
    arguments = random_input(100)
    eval_input = {
      'id': id,
      'program': answer,
      'arguments': arguments
    }
    eval_output = api.eval(eval_input)

    solver_input = '%s\n%d\n%s\n%d\n' % (
      size, len(operators), '\n'.join(operators), len(arguments))
    for i in xrange(len(arguments)):
      solver_input += '%s %s\n' % (int(arguments[i], 16),
                                   int(eval_output['outputs'][i], 16))
    fp = open('middle_%d.in' % j, 'w')
    print >> fp, solver_input
    fp.close()
    
    j += 1

if __name__ == '__main__':
  main()
