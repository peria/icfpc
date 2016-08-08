#!/usr/bin/python

import json
import os.path
import subprocess
import sys
import time


API_KEY = 'INPUT_YOUR_KEY'
URL = 'http://2016sv.icfpcontest.org/'
STATUS_PATH = 'api/snapshot/list'
BLOB_PATH = 'api/blob/'
SOLUTION_PATH = 'api/solution/submit'

CURL_OPT = ['--compressed', '-L']

# returns a JSON object
def api(path, plain=False, post=[]):
  url = URL + path
  headers = {'Expect': None, 'X-API-Key' : API_KEY}

  command = ['curl']
  command.extend(CURL_OPT)
  for k, v in headers.iteritems():
    command.append('-H')
    if v:
      command.append("'%s: %s'" % (k, v))
    else:
      command.append("%s:" % k)
  for v in post:
    command.append('-F')
    command.append(v)

  command.append(url)
  # print ' '.join(command)
  response = subprocess.check_output(' '.join(command), shell=True)
  if plain:
    return response
  return json.loads(response)


def snapshot():
  return api(STATUS_PATH)


def blob(h, plain=False):
  return api(BLOB_PATH + h, plain)


def solution(problem_id):
  post = [
      'problem_id=%d' % problem_id,
      'solution_spec=@../solutions/%d.txt' % problem_id
  ]
  return api(SOLUTION_PATH, post=post)


if __name__ == "__main__":
  print solution(10)
