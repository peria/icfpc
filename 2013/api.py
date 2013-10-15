#!/usr/bin/env python

import httplib
import json
import time

HOST = 'icfpc2013.cloudapp.net'

def my_problems():
  return json.loads(query('myproblems'))


# @param Object
#      {'size': int,
#       'operators: [] | ['fold'] | ['tfold']}
def train(param):
  return json.loads(query('train', param))


# @param Object
#      {'id': string,
#       'program': string,
#       'arguments': [string, string, ...]}
def eval(param):
  return json.loads(query('eval', param))


def query(path, param=None):
  f = open('auth_token', 'r')
  auth = f.read()
  f.close()

  while True:
    conn = httplib.HTTPConnection(HOST)
    url = '/%s?auth=%s' % (path, auth)
    if param is None:
      conn.request('POST', url)
    else:
      conn.request('POST', url, body=json.dumps(param))
    response = conn.getresponse()

    body = response.read()
    if response.status == 200:
      return body
    if response.status == 429:
      time.sleep(2)
      continue

    print response.status, body
    return '{}'
      

if __name__ == '__main__':
  param = {'size': 6,
           'operators': []}
  problem = train(param)
  print '%s\t%s\t%s\t%s' % (problem['id'],
                            problem['size'],
                            ','.join(problem['operators']),
                            problem['challenge'])
  
