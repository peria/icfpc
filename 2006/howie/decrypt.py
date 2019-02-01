#!/usr/bin/python

import subprocess


def main():
  # 1. setup UM(UMIX) to ommunicate in standard I/O.
  umix = subprocess.Popen(['../um/um', '../um/umix.um'],
                          stdin=subprocess.PIPE,
                          stdout=subprocess.PIPE)

  # 2. Get upload to be ready. (stdin < input.txt and uploader.adv)
  for filename in ['input.txt', 'uploader.adv']:
    with open(filename, 'r') as f:
      for l in f:
        umix.stdin.write(l)
  for _ in xrange(1383):
    umix.stdout.readline()


  rml = None
  with open('gc2.rml') as f:
    rml = f.read().split('\n')

  room = 'Rotunda'  # blueprint
  #room = 'Room With a Door'  # manifesto

  print 'Ready...'

  #plain = 'most enlightening.'
  plain = ''
  for i in xrange(len(plain), 1000):
    rml[502] = 'let index = %d.' % i

    # upload edited gc2.rml
    umix.stdin.write('\n'.join(rml) + '\n')
    print '\n'.join(rml)
    while umix.stdout.readline().strip() != '':
      pass
    while True:
      l = umix.stdout.readline().strip()
      if l == '':
        break

    p = None
    for c in 'abcdefghijklmnopqrstuvwxyz.#':
      umix.stdin.write('speak "%s"\n' % c)
      output = ''
      while output.find('You speak') < 0 and output.find('your thoughts') < 0:
        output = umix.stdout.readline().strip()
      print c, '#', output
      if output.find('You speak') >= 0:
        plain += p
        break
      p = c
    if p == '#':
      plain += ' '

    print i, plain
    umix.stdin.write('get uploader\n')


if __name__ == '__main__':
  main()
