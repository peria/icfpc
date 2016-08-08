#!/usr/bin/python

import api
import json
import os.path
import subprocess
import sys
import time


def get_snapshot():
  hashes = api.snapshot()['snapshots']
  snap_hash = {'snapshot_time': 0, 'snapshot_hash': ''}
  for h in hashes:
    if h['snapshot_time'] > snap_hash['snapshot_time']:
      snap_hash = h
  return snap_hash


def get_blob(h, plain=False):
  return api.blob(h, plain)


def main():
  # Get snapshot hash set
  h = get_snapshot()['snapshot_hash']
  print >> sys.stderr, "hash: " + h
  time.sleep(1);

  # Get status of problem sets
  res = get_blob(h)
  fp = open('current.txt', 'w')
  json.dump(res, fp, indent=2)
  fp.close()


if __name__ == '__main__':
  main()
