#!/usr/bin/python3

import argparse
import json
import os
import shutil
import subprocess
import tempfile


def ParseArgs():
  parser = argparse.ArgumentParser()
  parser.add_argument('--solver', required=True)
  parser.add_argument('--model_dir', required=True)
  parser.add_argument('--trace_dir', required=True)
  return parser.parse_args()


def GetProblems(model_dir_name):
  problems = set()
  model_dir = os.fsencode(model_dir_name)
  for file in os.listdir(model_dir):
    filename = os.fsdecode(file)
    if filename.endswith('.mdl'):
      problems.add(filename[0:5])
  return sorted(list(problems))


def RunSolver(problem, opts, tmp_dir):
  if problem[1] != 'A':
    return

  args = [opts.solver]

  src_file = os.path.join(opts.model_dir, problem + '_src.mdl')
  if os.path.isfile(src_file):
    args.extend(['--src', src_file])

  tgt_file = os.path.join(opts.model_dir, problem + '_tgt.mdl')
  if os.path.isfile(tgt_file):
    args.extend(['--tgt', tgt_file])

  trace_file = os.path.join(tmp_dir, problem + '.nbt')
  info_file = os.path.join(tmp_dir, problem + '.info')
  args.extend(['--trace', trace_file,
               '--info', info_file,
               '--label', problem])

  # Execute |args|
  proc = subprocess.call(args)
  print(problem + ' done')


def CompareAndUpdate(tmp_dir, problems, trace_dir):
  if not os.path.isdir(trace_dir):
    os.mkdir(trace_dir)
  all_info_file = os.path.join(trace_dir, 'info.json')
  try:
    with open(all_info_file, 'r') as f:
      all_info = json.load(f)
  except:
    all_info = {}

  for problem in problems:
    info_file = os.path.join(tmp_dir, problem + '.info')
    trace_file = os.path.join(tmp_dir, problem + '.nbt')
    try:
      with open(info_file, 'r') as f:
        info = json.load(f)
        if problem not in all_info or all_info[problem].energy > info.energy:
          all_info[problem] = info
          shutil.copyfile(trace_file, os.path.join(trace_dir, problem + '.nbt'))
    except:
      pass

  with open(all_info_file, 'w') as f:
    json.dump(all_info, f, sort_keys=True)


def main():
  opts = ParseArgs()
  tmp_dir = tempfile.mkdtemp()

  # Run solvers
  problems = GetProblems(opts.model_dir)
  for problem in problems:
    RunSolver(problem, opts, tmp_dir)

  # Compare the result with past results
  CompareAndUpdate(tmp_dir, problems, opts.trace_dir)


if __name__ == '__main__':
   main()
