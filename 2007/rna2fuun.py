#!/usr/bin/python

import sys

import Image

OUTPUT_FILE = 'output.png'
WIDTH = 600
HEIGHT = 600

def rna2fuun(rna_filename):
  rna_file = open(rna_filename)
  rnas = rna_file.readlines()
  rna_file.close()

  img = Image.new('RGB', (WIDTH, HEIGHT))
  for rna in rnas:
    rna = rna.rstrip()  # Remove new line code
    # TODO: implement conversion routine
  img.save(OUTPUT_FILE)


def main():
  if len(sys.argv) < 2:
    print "Usage: %s rna_file" % sys.argv[0]
    return
  rna2fuun(sys.argv[1])

if __name__ == "__main__":
  main()
