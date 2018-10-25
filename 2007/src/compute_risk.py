#!/usr/bin/python

import argparse
from PIL import Image


def parse_args():
  parser = argparse.ArgumentParser(description='Compute risk of generated image.')
  parser.add_argument('filename', type=str, help='filename to analyze')
  parser.add_argument('--prefix', '-p', dest='prefix', action='store',
                      help='filename of prefix')
  parser.add_argument('--target', '-t', dest='target', action='store',
                      default='../image/target.png', help='reference file')

  return parser.parse_args()

def main():
  args = parse_args()

  prefix = ''
  if args.prefix:
    try:
      with open(args.prefix) as f:
        prefix = f.readline().rstrip()  # read only the first line
    except:
      pass

  target = Image.open(args.target).getdata()
  image = Image.open(args.filename).getdata()

  diff = len(filter(lambda x: x[0] != x[1], zip(target, image)))
  risk = 10 * diff + len(prefix)

  if prefix != '':
    print 'risk for %s is %d (10*%d+%d).' % (prefix, risk, diff, len(prefix))
  else:
    print 'risk for %s is %d + len(prefix)' % (args.filename, risk)


if __name__ == '__main__':
  main()
