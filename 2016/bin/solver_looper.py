#!/usr/bin/python

import api
import json
import os
import os.path
import subprocess
import sys
import time

PROBLEM_DIR = '../problems/'
SOLUTION_DIR = '../solutions/'
SOLVER = '../src/solution_main'
STATUS_FILE = '../src/status.json'

OWN_PROBLEMS = ['5808.txt', '6189.txt' # submitted problem
                '2569.txt', '2267.txt', '2266.txt', '2796.txt', '2777.txt',  # size limit exceeded
                '2225.txt', '2966.txt', '2146.txt', '2569.txt', '2668.txt',  # size limit exceeded
                '2264.txt', '2414.txt', '4628.txt', '6200.txt', '6232.txt',  # size limit exceeded
                '6189.txt']  # size limit exceeded

def dumpStatus(status):
  fp = open(STATUS_FILE, 'w')
  json.dump(status, fp, indent=2)
  fp.close()


def needUpdate(filename, status):
  solution_filepath = os.path.join(SOLUTION_DIR, filename)
  if not os.path.exists(solution_filepath):
    return True
  if not filename in status:
    return True

  stat = os.stat(solution_filepath)
  if status[filename] <= stat.st_mtime:
    return False
  return True


def dumpSolution(filename, solution):
  fp = open(os.path.join(SOLUTION_DIR, filename), 'w')
  fp.write(solution)
  fp.close()


def submitSolution(filename):
  time.sleep(1.2)
  index = filename.index('.')
  prob_id = int(filename[0:index])
  res = api.solution(prob_id)
  return res


def checkStatus(status):
  problems = os.listdir(PROBLEM_DIR)

  updated = 0
  for filename in problems:
    if filename.find('.txt') < 0:
      continue
    if filename in OWN_PROBLEMS:
      continue
    if not needUpdate(filename, status):
      continue

    # Solve locally
    proc = subprocess.Popen([SOLVER, os.path.join(PROBLEM_DIR, filename)],
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    solution, _ = proc.communicate()
    lines = solution.split()
    if lines[0] == '0':
      continue

    # Dump solution locarry
    dumpSolution(filename, solution)
    print >>sys.stderr, 'Solved %s' % filename

    # Submit
    res = submitSolution(filename)
    print filename, res # output the response to log
    print >>sys.stderr, res
    if not res['ok']:
      continue

    # updat state
    solution_filepath = os.path.join(SOLUTION_DIR, filename)
    if os.path.exists(solution_filepath):
      stat = os.stat(solution_filepath)
      status[filename] = stat.st_mtime
      updated = updated + 1
      if updated % 20 == 19:
        dumpStatus(status)


def main():
  status = {}
  try:
    fp = open(STATUS_FILE)
    status = json.load(fp)
    fp.close()
  except IOError:
    pass
  except ValueError:
    pass

  checkStatus(status)

  dumpStatus(status)


if __name__ == '__main__':
  main()
