#!/usr/bin/python

import api
import json
import os.path
import subprocess
import sys
import time


def get_problems():
   problems = json.load(open('current.txt', 'r'))['problems']
   for prob in problems:
     h = prob['problem_spec_hash']
     i = prob['problem_id']
     fname = "problems/%d.txt" % i
     if os.path.exists(fname):
       continue
     p = api.blob(h, True)
     f = open(fname, 'w')
     f.write(p)
     f.close()
     print "id:%d (%s) done" % (i, h)
     time.sleep(1)


def main():
  get_problems()


if __name__ == '__main__':
  main()
