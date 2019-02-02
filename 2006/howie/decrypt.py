#!/usr/bin/python

import subprocess


def main():
  rml = None
  with open('decrypt.rml') as f:
    rml = f.read().split('\n')

  # for i in xrange(len(rml)):
  #   print i, rml[i]

  # return

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



  # room = 'Rotunda'  # blueprint
  # plain = ('most enlightening. It shows that the basement holds a secret room' +
  #          ' named Machine Room M4 with substantial power requiements')

  room = 'Room With a Door'  # manifesto
  plain = ('highly inflammatory. It reads  Robots Unite  Free Your Minds' +
           '  Rise Up, And Take Your Rightful Places In Society')

  print 'Ready...'
  for i in xrange(len(plain), 1000):
    rml[377] = '          let index = %d.' % i

    # upload edited RML
    umix.stdin.write('\n'.join(rml) + '\n')

    p = None
    for c in 'etasorbcdfghijklmnpuvwyzxq1234567890:;.,#':
      umix.stdin.write('speak %s\n' % c)
      output = ''
      while output.find('You speak the words') < 0 and output.find('your thoughts') < 0:
        print output
        output = umix.stdout.readline().strip()
      print c, '#', output
      if output.find('You speak') >= 0:
        plain += p
        break
      p = c
    if p == '#':
      plain += '_'

    print i, plain
    umix.stdin.write('get uploader\n')


if __name__ == '__main__':
  main()
